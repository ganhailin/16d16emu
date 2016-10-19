#include <SDL2/SDL.h>
#include <pthread.h>
#include "touch.h"
#include <memory.h>
#include <time.h>
#include <math.h>
touch_t touch_now;
item_t items[MAX_ITEM];
int numofitem=0;

int touchdown(SDL_TouchFingerEvent finger)
{
    int i;
    for(i=0; i<MAX_POINT; i++)
    {
        if(touch_now.tpoint[i].is==0)
            break;
    }
    if(i==10)
        return -1;
    touch_now.tpoint[i].is=1;
    touch_now.tpoint[i].id=finger.fingerId;
    touch_now.tpoint[i].x=finger.x;
    touch_now.tpoint[i].y=finger.y;
    touch_now.tpoint[i].ry=0;
    touch_now.tpoint[i].rx=0;
    touch_now.tpoint[i].presure=finger.pressure;
    touch_now.tpoint[i].time=clock();
    touch_now.num++;
    marchpoint(i);
    return 0;
}
int touchup(SDL_TouchFingerEvent finger)
{
    int i;
    for(i=0; i<MAX_POINT; i++)
    {
        if(touch_now.tpoint[i].id==finger.fingerId)
            break;
    }
    if(i==10)
        return -1;
    touch_now.tpoint[i].is=0;
    touch_now.num--;
    unmarchpoint(i);
    return 0;
}
int touchmo(SDL_TouchFingerEvent finger)
{
    if(fabs(finger.dx)<OFFSET&&fabs(finger.dy)<OFFSET)
        return -2;
    int i;
    for(i=0; i<MAX_POINT; i++)
    {
        if(touch_now.tpoint[i].id==finger.fingerId)
            break;
    }
    if(i==10)
        return -1;
    touch_now.tpoint[i].x=finger.x;
    touch_now.tpoint[i].y=finger.y;
    touch_now.tpoint[i].ry=finger.dy;
    touch_now.tpoint[i].rx=finger.dx;
    touch_now.tpoint[i].presure=finger.pressure;
    statusupdata(i);
    return 0;
}

void * pthtouch(void* args)
{
    while(true)
    {
        SDL_Delay(2);
    }
    return NULL;
}
pthread_t thread;

int getfingernum(void)
{
    return touch_now.num;
}

int Gan_touch_init(SDL_Window * window)
{
    memset(&touch_now,0,sizeof(touch_t));
    pthread_create(&thread,NULL,pthtouch,NULL);
    return 0;
}


int marchpoint(int p)
{
    return 0;
}

int unmarchpoint (int p)
{
    return 0;
}

int statusupdata(int p)
{
    return 0;
}


