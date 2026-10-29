#include <kipr/wombat.h>
#include <stdlib.h>
#include <stdio.h>

/* Wombat motor API: mav is ticks/s (-1500..1500; best encoder accuracy within ±1000).
   motor is PWM percent (-100..100). Calibrate the fitted robot before relying on distance. */
#define LM 0
#define RM 3
#define LS 1
#define RS 1
#define TICKS_PER_CM 79.57747
#define LEFT_SCALE 1.0000
#define RIGHT_SCALE 1.0000
#define LEFT_TURN_SCALE 1.3000
#define RIGHT_TURN_SCALE 1.3000
#define TRACK_CM 14.50
#define MIN_MAV 119
#define MAV_LIMIT 1500
#define PWM_LIMIT 100
#define MAX_CM_S 10.00
#define SYNC_GAIN 1.000
#define ARM_PORT 2
#define CLAW_PORT 3
#define ARM_UP 150
#define ARM_DOWN 1150
#define CLAW_OPEN 1150
#define CLAW_CLOSE 1900
#define ARM_WAIT_MS 800
#define CLAW_WAIT_MS 500
#define FWD 1
#define REV 2
#define LEFT 3
#define RIGHT 4
static int ci(int x,int a,int b){return x<a?a:(x>b?b:x);}
static void servo_wait(int p,int v,int ms){set_servo_position(p,ci(v,0,2047));msleep(ms);}
static int motion(int mode,int lt,int rt,int req){
    int ls,rs,ld=0,rd=0,lastl=0,lastr=0;
    int lo=10,cmd=0;
    double start,ll,rr,limit;
    lt=abs(lt);rt=abs(rt);req=ci(req,lo,PWM_LIMIT);
    if(mode==FWD){ls=LS;rs=RS;}else if(mode==REV){ls=-LS;rs=-RS;}else if(mode==LEFT){ls=-LS;rs=RS;}else if(mode==RIGHT){ls=LS;rs=-RS;}else return 0;
    cmpc(LM);cmpc(RM);
    start=seconds();ll=start;rr=start;
    limit=5.0+(double)(lt>rt?lt:rt)/(TICKS_PER_CM*MAX_CM_S*req/100.0)*3.0;
    while(!ld||!rd){
        int l=abs(gmpc(LM)),r=abs(gmpc(RM)),v,c;
        double lf=lt?(double)l/lt:1.0,rf=rt?(double)r/rt:1.0;
        double lp=lf<0.0?0.0:(lf>1.0?1.0:lf),rp=rf<0.0?0.0:(rf>1.0?1.0:rf),rem=1.0;
        if(!lt||l>=lt){if(!ld)freeze(LM);ld=1;}
        if(!rt||r>=rt){if(!rd)freeze(RM);rd=1;}
        if(!ld&&lt){double q=1.0-lp;if(q<rem)rem=q;}
        if(!rd&&rt){double q=1.0-rp;if(q<rem)rem=q;}
        if(rem<0.0)rem=0.0;else if(rem>1.0)rem=1.0;
        v=rem<0.25?lo+(int)((req-lo)*rem/0.25):req;
        if(v>cmd+2)cmd+=2;else if(v<cmd-2)cmd-=2;else cmd=v;
        if(cmd<lo)cmd=lo;
        c=(int)(SYNC_GAIN*(lp-rp)*cmd);c=ci(c,-cmd/2,cmd/2);
        if(!ld)motor(LM,ls*ci(cmd-c,lo,req));
        if(!rd)motor(RM,rs*ci(cmd+c,lo,req));
        if(l>lastl+2){lastl=l;ll=seconds();}if(r>lastr+2){lastr=r;rr=seconds();}
        if(seconds()-start>limit||(!ld&&seconds()-ll>1.5)||(!rd&&seconds()-rr>1.5)){ao();return 0;}
        msleep(10);
    }
    freeze(LM);freeze(RM);return 1;
}static int wait_digital(int p,int val,double timeout){double t=seconds();while(digital(p)!=val){if(seconds()-t>timeout)return 0;msleep(10);}return 1;}
static int wait_analog(int p,int greater,int val,double timeout){double t=seconds();while(greater?analog12(p)<=val:analog12(p)>=val){if(seconds()-t>timeout)return 0;msleep(10);}return 1;}
static int run_program(void){int ok=1,camera_opened=0;set_servo_position(ARM_PORT,ci(ARM_UP,0,2047));set_servo_position(CLAW_PORT,ci(CLAW_OPEN,0,2047));enable_servos();msleep(ARM_WAIT_MS);msleep(CLAW_WAIT_MS);
    for(int loop_square=0;loop_square<4;++loop_square){
        if(!motion(FWD, 2387, 2387, 70)){ok=0;goto cleanup;}
        msleep(1000);
        if(!motion(LEFT, 1178, 1178, 70)){ok=0;goto cleanup;}
    }
ao();
cleanup:if(!ok)ao();disable_servos();return ok;}
int main(void){int ok=run_program();if(!ok)ao();disable_servos();return ok?0:1;}
