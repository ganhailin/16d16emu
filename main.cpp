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
int fps=0,fpss;
bool done = false;
bool drawdone = false;
pointR3D zero= {.rx=0,.ry=0,.rz=0};
extern char sharebuff[100];
tetris3d tet;


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
    while(1)
    {

        usleep(10000);
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
        for(int i=0; i<10; i++)
            usleep(100000);
        tet.gameloop();
    }
    return NULL;
}

int main(int argc, char *argv[])
{
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

