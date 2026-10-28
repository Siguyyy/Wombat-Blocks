#include <kipr/wombat.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Wombat autonomous program generated for the KISS IDE.
 *
 * This file is intentionally self-contained: paste the entire file into
 * main.c in a KISS C project, compile, and run.
 *
 * The servo positions are valid Wombat positions (0..2047).  The safer
 * Wheel diameter and wheel track below are explicit estimates, not dimensions
 * supplied by the 2020 build guide. Measure the assembled robot and replace
 * them. KIPR documents approximately 1800 encoder ticks per wheel revolution.
 * Calibrate wheel scales with measured straight runs on the actual field surface.
 * Servo endpoints are robot-specific; move them cautiously to avoid binding.
 */

#define LEFT_MOTOR 3
#define RIGHT_MOTOR 0
#define LEFT_FORWARD_SIGN 1
#define RIGHT_FORWARD_SIGN 1

#define WHEEL_DIAMETER_CM 6.0 /* ESTIMATE: measure the fitted wheel */
#define TICKS_PER_WHEEL_REV 1800.0 /* KIPR nominal; calibrate on your controller */
#define TICKS_PER_CM (TICKS_PER_WHEEL_REV / (3.141592653589793 * WHEEL_DIAMETER_CM))
#define LEFT_DISTANCE_SCALE 1.0000
#define RIGHT_DISTANCE_SCALE 1.0000
#define TURN_SCALE 1.0000
#define WHEEL_TRACK_CM 14.0 /* ESTIMATE: measure wheel centers */

#define DRIVE_SPEED_CM_S 6.0 /* full-speed measured travel; 30 cm ~= 5 s */
#define MIN_SPEED_CM_S 1.5
#define MAX_SPEED ((int)(DRIVE_SPEED_CM_S * TICKS_PER_CM))
#define MIN_SPEED ((int)(MIN_SPEED_CM_S * TICKS_PER_CM))
#define SYNC_GAIN 1.00
#define MAX_SYNC_CORRECTION 300
#define STALL_TIMEOUT_SECONDS 1.5
#define EXTRA_TIMEOUT_SECONDS 4.0

#define ARM_PORT 2
#define CLAW_PORT 3
#define ARM_UP_POSITION 150
#define ARM_DOWN_POSITION 1150
#define CLAW_OPEN_POSITION 1150
#define CLAW_CLOSED_POSITION 1900
#define ARM_SETTLE_MS 800
#define CLAW_SETTLE_MS 500

#define STATE_FORWARD 1
#define STATE_LEFT 2
#define STATE_RIGHT 3
#define STATE_BACKWARD 4

static int clamp_int(int value, int low, int high) {
    if (value < low) return low;
    if (value > high) return high;
    return value;
}

static int safe_servo_position(int position) {
    return clamp_int(position, 0, 2047);
}

static void stop_drive(void) {
    freeze(LEFT_MOTOR);
    freeze(RIGHT_MOTOR);
    ao();
}

static void move_servo_and_wait(int port, int position, int settle_ms) {
    set_servo_position(port, safe_servo_position(position));
    msleep(settle_ms);
}

/* Replay one recorded-style motion using both wheel encoders. */
static int run_motion_ticks(int state, int left_target, int right_target) {
    int left_sign;
    int right_sign;
    int left_done = 0;
    int right_done = 0;
    int last_left = 0;
    int last_right = 0;
    double last_left_time;
    double last_right_time;
    double start_time;
    double timeout_seconds;

    if (left_target < 0) left_target = -left_target;
    if (right_target < 0) right_target = -right_target;
    if (left_target == 0 && right_target == 0) return 1;

    if (state == STATE_FORWARD) {
        left_sign = LEFT_FORWARD_SIGN;
        right_sign = RIGHT_FORWARD_SIGN;
    } else if (state == STATE_BACKWARD) {
        left_sign = -LEFT_FORWARD_SIGN;
        right_sign = -RIGHT_FORWARD_SIGN;
    } else if (state == STATE_LEFT) {
        left_sign = LEFT_FORWARD_SIGN;
        right_sign = -RIGHT_FORWARD_SIGN;
    } else if (state == STATE_RIGHT) {
        left_sign = -LEFT_FORWARD_SIGN;
        right_sign = RIGHT_FORWARD_SIGN;
    } else {
        return 0;
    }

    cmpc(LEFT_MOTOR);
    cmpc(RIGHT_MOTOR);
    start_time = seconds();
    last_left_time = start_time;
    last_right_time = start_time;
    timeout_seconds = EXTRA_TIMEOUT_SECONDS +
        (double)(left_target > right_target ? left_target : right_target) /
        (double)MAX_SPEED * 2.0;

    while (!left_done || !right_done) {
        int left_progress = abs(gmpc(LEFT_MOTOR));
        int right_progress = abs(gmpc(RIGHT_MOTOR));
        int base_speed;
        int correction;
        int left_speed;
        int right_speed;
        double left_fraction = left_target > 0 ?
            (double)left_progress / (double)left_target : 1.0;
        double right_fraction = right_target > 0 ?
            (double)right_progress / (double)right_target : 1.0;
        double remaining_fraction;
        double elapsed = seconds() - start_time;

        if (left_target == 0 || left_progress >= left_target) {
            if (!left_done) freeze(LEFT_MOTOR);
            left_done = 1;
        }
        if (right_target == 0 || right_progress >= right_target) {
            if (!right_done) freeze(RIGHT_MOTOR);
            right_done = 1;
        }

        remaining_fraction = 1.0;
        if (left_fraction < remaining_fraction) {
            remaining_fraction = 1.0 - left_fraction;
        }
        if (right_fraction < 1.0 && 1.0 - right_fraction < remaining_fraction) {
            remaining_fraction = 1.0 - right_fraction;
        }

        if (remaining_fraction < 0.25) {
            base_speed = MIN_SPEED + (int)((MAX_SPEED - MIN_SPEED) *
                          remaining_fraction / 0.25);
        } else {
            base_speed = MAX_SPEED;
        }
        base_speed = clamp_int(base_speed, MIN_SPEED, MAX_SPEED);

        correction = (int)(SYNC_GAIN * (left_fraction - right_fraction) *
                           (double)base_speed);
        correction = clamp_int(correction, -MAX_SYNC_CORRECTION,
                               MAX_SYNC_CORRECTION);
        left_speed = clamp_int(base_speed - correction, MIN_SPEED, MAX_SPEED);
        right_speed = clamp_int(base_speed + correction, MIN_SPEED, MAX_SPEED);

        if (!left_done) mav(LEFT_MOTOR, left_sign * left_speed);
        if (!right_done) mav(RIGHT_MOTOR, right_sign * right_speed);

        if (left_progress > last_left + 2) {
            last_left = left_progress;
            last_left_time = seconds();
        }
        if (right_progress > last_right + 2) {
            last_right = right_progress;
            last_right_time = seconds();
        }

        if (elapsed > timeout_seconds ||
            (!left_done && seconds() - last_left_time > STALL_TIMEOUT_SECONDS) ||
            (!right_done && seconds() - last_right_time > STALL_TIMEOUT_SECONDS)) {
            printf("Motion timeout/stall: L=%d/%d R=%d/%d\n",
                   left_progress, left_target, right_progress, right_target);
            stop_drive();
            return 0;
        }

        msleep(10);
    }

    stop_drive();
    return 1;
}

static int run_autonomous_routine(void) {
    double turn_arc_cm;
    int turn_ticks;

    /* Preset before enabling so the servos do not first travel to center. */
    set_servo_position(ARM_PORT, safe_servo_position(ARM_UP_POSITION));
    set_servo_position(CLAW_PORT, safe_servo_position(CLAW_OPEN_POSITION));
    enable_servos();
    msleep(ARM_SETTLE_MS);
    msleep(CLAW_SETTLE_MS);

    /* Sample routine. Replace these actions with the blocks in the HTML tool. */
    if (!run_motion_ticks(STATE_FORWARD,
            (int)(30.0 * TICKS_PER_CM * LEFT_DISTANCE_SCALE),
            (int)(30.0 * TICKS_PER_CM * RIGHT_DISTANCE_SCALE))) {
        disable_servos();
        return 0;
    }

    turn_arc_cm = 3.141592653589793 * WHEEL_TRACK_CM * 90.0 / 360.0;
    turn_ticks = (int)(turn_arc_cm * TICKS_PER_CM * TURN_SCALE);
    if (!run_motion_ticks(STATE_LEFT,
            (int)(turn_ticks * LEFT_DISTANCE_SCALE),
            (int)(turn_ticks * RIGHT_DISTANCE_SCALE))) {
        disable_servos();
        return 0;
    }

    if (!run_motion_ticks(STATE_FORWARD,
            (int)(20.0 * TICKS_PER_CM * LEFT_DISTANCE_SCALE),
            (int)(20.0 * TICKS_PER_CM * RIGHT_DISTANCE_SCALE))) {
        disable_servos();
        return 0;
    }

    move_servo_and_wait(CLAW_PORT, CLAW_CLOSED_POSITION, CLAW_SETTLE_MS);
    msleep(500);
    move_servo_and_wait(CLAW_PORT, CLAW_OPEN_POSITION, CLAW_SETTLE_MS);

    stop_drive();
    disable_servos();
    return 1;
}

int main(void) {
    int ok = run_autonomous_routine();
    stop_drive();
    disable_servos();
    return ok ? 0 : 1;
}
