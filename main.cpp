#include <time.h>
#include <math.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "3D.h"
#include "draw.h"
#include "touch.h"
#include "tetris3d.h"
//#include "rs232.h"
int fps=0,fpss;
bool done = false;
bool drawdone = false;
pointR3D zero= {.rx=0,.ry=0,.rz=0};
extern char sharebuff[100];
tetris3d tet;
SDL_GameController *controller = NULL;
int axis[4]= {0,0,0,0};


void * pth1(void* args)///---------------------------此乃刷屏线程
{
    tet.init();
    char str[30];
    char str2[30];
    while(!done)
    {
        usleep(200);
        if(done)
            break;
        clrscreen();
        clrdeepbuff();
        sprintf(str,"FPS:%d",fpss);
        //sprintf(str2,"Num of Gamepad:%d",SDL_NumJoysticks());
        sprintf(sharebuff,"Number of Finger(s):%d",getfingernum());
        drawstring(0,0,(unsigned char*)str,0xffffff);
        //drawstring(0,10,(unsigned char*)str2,0xffffff);
        //drawstring(0,20,(unsigned char*)sharebuff,0xffffff);
        drawstring(0,30,(unsigned char*)tet.getmsg(),0xffffff);
       /* for(int i=0;i<4;i++)
        {
        sprintf(str2,"Key%d:%d",i,axis[i]);

        drawstring(0,40+i*10,(unsigned char*)str2,0xffffff);

        }*/

        tet.gamedraw();
        updatescreen();
        fps++;
    }
    drawdone=true;
    return NULL;
}

void * pth2(void* args)///-----------------------------------这是fps计数线程
{
    while(1)
    {

        fps=0;
        for(int i=0; i<10; i++)
            usleep(100000);
        fpss=fps;
    }
    return NULL;
}

void * pth3(void* args)///-----------------------------------这是模拟按键扫描中断
{
//    int keybefor[6]= {0,0,0,0,0,0};
    while(1)
    {

        usleep(100);

        /*        gamepad_t nowpad=getgamepad();
                if(nowpad.x>191)
                    nowpad.x=1;
                else if(nowpad.x<63)
                    nowpad.x=2;
                else
                    nowpad.x=0;
                if(nowpad.y>191)
                    nowpad.y=1;
                else if(nowpad.y<63)
                    nowpad.y=2;
                else
                    nowpad.y=0;
                if(nowpad.z>191)
                    nowpad.z=1;
                else if(nowpad.z<63)
                    nowpad.z=2;
                else
                    nowpad.z=0;
                if(nowpad.rz>191)
                    nowpad.rz=1;
                else if(nowpad.rz<63)
                    nowpad.rz=2;
                else
                    nowpad.rz=0;
                if(nowpad.x!=keybefor[0])
                    if(nowpad.x==1)
                        tet.input(1,0);
                    else if(nowpad.x==2)
                        tet.input(2,0);
                keybefor[0]=nowpad.x;
                if(nowpad.y!=keybefor[1])
                    if(nowpad.y==1)
                        tet.input(3,0);
                    else if(nowpad.y==2)
                        tet.input(4,0);
                keybefor[1]=nowpad.y;
                if(nowpad.z!=keybefor[2])
                    if(nowpad.z==1)
                        tet.input(0,1);
                    else if(nowpad.z==2)
                        tet.input(0,2);
                keybefor[2]=nowpad.z;
                if(nowpad.rz!=keybefor[3])
                    if(nowpad.rz==1)
                        tet.input(0,3);
                    else if(nowpad.rz==2)
                        tet.input(0,4);
                keybefor[3]=nowpad.rz;

                if((nowpad.keys&0x01)!=keybefor[4])
                    if(nowpad.keys&0x01)
                        tet.input(0,5);
                keybefor[4]=nowpad.keys&0x01;
                if((nowpad.keys&0x02)!=keybefor[5])
                    if(nowpad.keys&0x02)
                        tet.input(0,6);
                keybefor[5]=nowpad.keys&0x02;

        */
        //padx=SDL_GameControllerGetAxis(controller,SDL_CONTROLLER_AXIS_LEFTX )

        static int padx,padx_old,pady,pady_old,padrx,padrx_old,padry,padry_old,padrz,padrz_old,B_start,B_start_old,B_back,B_back_old,B_B,B_B_old,B_A,B_A_old;
        float padcamrz;
        padcamrz=SDL_GameControllerGetAxis(controller,SDL_CONTROLLER_AXIS_TRIGGERRIGHT)/32768.0/2.0 -SDL_GameControllerGetAxis(controller,SDL_CONTROLLER_AXIS_TRIGGERLEFT)/32768.0/2.0;
        tet.led.rd.ry+=D2R(padcamrz);
        B_start=SDL_GameControllerGetButton(controller,SDL_CONTROLLER_BUTTON_START );
        if(!B_start)
            if(B_start_old)
                tet.newgame();
        B_start_old=B_start;
        B_back=SDL_GameControllerGetButton(controller,SDL_CONTROLLER_BUTTON_BACK);
        if(!B_back)
            if(B_back_old)
                tet.pause();
        B_back_old=B_back;
        padrz=SDL_GameControllerGetButton(controller,SDL_CONTROLLER_BUTTON_LEFTSHOULDER)-SDL_GameControllerGetButton(controller,SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
        if(!padrz)
            if(padrz_old==1)
                tet.input(0,4);
            else if(padrz_old==-1)
                tet.input(0,3);
        padrz_old=padrz;
        padx=axis[0];
        if(!padx)
            if(padx_old==1)
                tet.input(2,0);
            else if(padx_old==-1)
                tet.input(1,0);
        padx_old=padx;
        pady=axis[1];
        if(!pady)
            if(pady_old==1)
                tet.input(3,0);
            else if(pady_old==-1)
                tet.input(4,0);
        pady_old=pady;
        padrx=axis[2];
        if(!padrx)
            if(padrx_old==1)
                tet.input(0,5);
            else if(padrx_old==-1)
                tet.input(0,6);
        padrx_old=padrx;
        padry=axis[3];
        if(!padry)
            if(padry_old==1)
                tet.input(0,1);
            else if(padry_old==-1)
                tet.input(0,2);
        padry_old=padry;
        B_A=SDL_GameControllerGetButton(controller,SDL_CONTROLLER_BUTTON_A);
        if(!B_A)
            if(B_A_old)
                tet.gameloop();
        B_A_old=B_A;
        B_B=SDL_GameControllerGetButton(controller,SDL_CONTROLLER_BUTTON_B);
        if(!B_B)
            if(B_B_old)
                done=1;
        B_B_old=B_B;



        /********************************************//**
        tet.newgame();
        tet.gameloop();
        tet.input(2,0);
         ***********************************************/


        uint32_t *key=getkey();
        //if(*key&0x01)
        //movecam(0,0,1);
        //if(*key&0x02)
        //movecam(0,0,-1);
        if(*key&0x04)
            tet.led.rd.ry-=D2R(0.5f);
        //     movecam(1,0,0);
        if(*key&0x08)
            tet.led.rd.ry+=D2R(0.5f);
        //     movecam(-1,0,0);
        // if(*key&0x10)
        //     movecam(0,-1,0);
        // if(*key&0x20)
        //     movecam(0,1,0);
        if(*key&0x0100)
        {
            *key&=~0x0100;
            tet.input(1,0);
        }
        if(*key&0x0200)
        {
            *key&=~0x0200;
            tet.input(2,0);
        }
        if(*key&0x0400)
        {
            *key&=~0x0400;
            tet.input(4,0);
        }
        if(*key&0x0800)
        {
            *key&=~0x0800;
            tet.input(3,0);
        }
        if(*key&0x1000)
        {
            *key&=~0x1000;
            tet.newgame();
        }
        if(*key&0x2000)
        {
            *key&=~0x2000;
            tet.input(0,1);
        }
        if(*key&0x4000)
        {
            *key&=~0x4000;
            tet.input(0,2);
        }
        if(*key&0x8000)
        {
            *key&=~0x8000;
            tet.input(0,3);
        }
        if(*key&0x10000)
        {
            *key&=~0x10000;
            tet.input(0,4);
        }
        if(*key&0x20000)
        {
            *key&=~0x20000;
            tet.input(0,5);
        }
        if(*key&0x40000)
        {
            *key&=~0x40000;
            tet.input(0,6);
        }
        if(*key&0x80000)
        {
            *key&=~0x80000;
            tet.gameloop();
        }
        if(*key&0x100000)
        {
            *key&=~0x100000;
            tet.pause();
        }

    }

    return NULL;
}





void * pth4(void* args)///-----------------------------模拟一个计时器中断，100毫秒执行一次。
{
    while(!done)
    {
        tet.runstept();
        usleep(100000);
    }
    return NULL;
}
void getgamepad();

void * pth5(void* args)
{
    while(!done)
    {
        getgamepad();
        usleep(5000);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t thread,thread2,thread3,thread4,thread5;
    setdisplay(480,720);
    initSDL();
    for (int i = 0; i < SDL_NumJoysticks(); ++i)
        if (SDL_IsGameController(i))
        {
            controller = SDL_GameControllerOpen(i);
            if (controller)
                break;
        }

    setcam(setpoint(0,0,-200,0),setpointR(0,0,0));
    seteye(setpoint(0,0,-500,0));
    pthread_create(&thread,NULL,pth1,NULL);
    pthread_create(&thread2,NULL,pth2,NULL);
    pthread_create(&thread3,NULL,pth3,NULL);
    pthread_create(&thread4,NULL,pth4,NULL);
    pthread_create(&thread5,NULL,pth5,NULL);
    SDLloop(&drawdone,&done);
    exit(0);
}

void getgamepad( )
{
    for(int j=0; j<4; j++)
    {
        int i=SDL_GameControllerGetAxis(controller,(SDL_GameControllerAxis)j);
        if(axis[j])
        {
            if(abs(i)<32767/4)
                axis[j]=0;
            else if(abs(i)>32767/2)
                if(i>0)
                    axis[j]=1;
                else
                    axis[j]=-1;
        }
        else
        {
            if(abs(i)<32767/4)
                axis[j]=0;
            else if(abs(i)>32767/2)
                if(i>0)
                    axis[j]=1;
                else
                    axis[j]=-1;

        }
    }


}
