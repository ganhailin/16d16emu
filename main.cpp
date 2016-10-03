#include <time.h>
#include <math.h>
#include "SDL2/SDL.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "3D.h"
#include "draw.h"
#include "touch.h"
#include "tetris3d.h"
#include "rs232.h"
int fps=0,fpss;
bool done = false;
bool drawdone = false;
pointR3D zero= {.rx=0,.ry=0,.rz=0};
extern char sharebuff[100];
tetris3d tet;

typedef struct
{
    int x,y,z,rz,keys;
} gamepad_t;
gamepad_t getgamepad();

void * pth1(void* args)
{

    tet.init();

    char str[30];
    while(!done)
    {
        usleep(200);
        if(done)
            break;
        clrscreen();
        clrdeepbuff();
        sprintf(str,"FPS:%d",fpss);
        sprintf(sharebuff,"Number of Finger(s):%d",getfingernum());
        drawstring(0,0,(unsigned char*)str,0xffffff);
        drawstring(0,10,(unsigned char*)sharebuff,0xffffff);
        tet.gamedraw();
        updatescreen();
        fps++;
    }
    drawdone=true;
    return NULL;
}

void * pth2(void* args)
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

void * pth3(void* args)
{
    int keybefor[6]= {0,0,0,0,0,0};
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
        uint32_t *key=getkey();
        if(*key&0x01)
            movecam(0,0,1);
        if(*key&0x02)
            movecam(0,0,-1);
        if(*key&0x04)
            movecam(1,0,0);
        if(*key&0x08)
            movecam(-1,0,0);
        if(*key&0x10)
            movecam(0,-1,0);
        if(*key&0x20)
            movecam(0,1,0);
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

    }

    return NULL;
}

void * pth4(void* args)
{
    while(!done)
    {
        if(done)
            break;
        for(int i=0; i<3; i++)
            usleep(100000);
        tet.gameloop();
    }
    return NULL;
}

gamepad_t getgamepad()
{
    uint8_t read[20];
    while(1)
    {
        usleep(5);
        RS232_PollComport(0,read,1);
        if(read[0]==0xff)break;
    }
    RS232_PollComport(0,read,5);
    gamepad_t pad;
    pad.x=read[0];
    pad.y=read[1];
    pad.z=read[2];
    pad.rz=read[3];
    pad.keys=read[4];
    return pad;
}


int main(int argc, char *argv[])
{
    RS232_OpenComport(0, 115200);
    pthread_t thread,thread2,thread3,thread4;
    setdisplay(1280,720);
    initSDL();
    setcam(setpoint(0,0,-150,0),setpointR(0,0,0));
    seteye(setpoint(0,0,-500,0));
    pthread_create(&thread,NULL,pth1,NULL);
    pthread_create(&thread2,NULL,pth2,NULL);
    pthread_create(&thread3,NULL,pth3,NULL);
    pthread_create(&thread4,NULL,pth4,NULL);
    SDLloop(&drawdone,&done);
    exit(0);
}

