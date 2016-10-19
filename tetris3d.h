#ifndef TETRIS3D_H
#define TETRIS3D_H
#include "ledblock.h"
#include <stdio.h>
typedef struct
{
    int numofbk;
    point points[8];
    int color;
} tetrisbk;

class tetris3d;


class block
{
public:

    int bkdown();
    int bkmove(uint8_t x);
    int bkrote(uint8_t x);
    int bknew();
    int bkclear();
    void setbuff(uint8_t (*buff)[8][8][16])
    {
        blocks=buff;
    }
    ledemu* display;
    tetris3d * parent;
private:

    int ckpos(point pos);///Check if there is a block
    point rotatepoint(point res);
    void bkdrawsingle(int x,int y,int z,int color)
    {
        if(z<0||z>7||x<0||x>3||y>3||y<0)
            return;
        x*=2;
        y*=2;
        z*=2;
        this->display->setled(x,y,z,color);
        this->display->setled(x,y,z+1,color);
        this->display->setled(x+1,y,z,color);
        this->display->setled(x+1,y,z+1,color);
        this->display->setled(x,y+1,z,color);
        this->display->setled(x,y+1,z+1,color);
        this->display->setled(x+1,y+1,z,color);
        this->display->setled(x+1,y+1,z+1,color);
    }
    int bkdraw();
    int posx,posy,posz;
    int type;
    int rox,roy,roz;
    tetrisbk nowblock;
    int intsin(int x)
    {
        switch(x)
        {
        case 0:
            return 0;
            break;
        case 1:
            return 1;
            break;
        case 2:
            return 0;
            break;
        case 3:
            return -1;
            break;
        default:
            break;
        }
        return 0;
    }
    int intcos(int x)
    {
        switch(x)
        {
        case 0:
            return 1;
            break;
        case 1:
            return 0;
            break;
        case 2:
            return -1;
            break;
        case 3:
            return 0;
            break;
        default:
            break;
        }
        return 0;
    }
    uint8_t (*blocks)[8][8][16];
};


class tetris3d
{
public:
    ledemu led;
    tetris3d(void)
    {}

    void init()
    {
        this->led.init(8,8,16);
        this->blocks=(uint8_t (*) [8][8][16])led.getbuffbk();
        memset(this->blocks,0,8*8*16*sizeof(uint8_t));
        this->nowblock.setbuff((uint8_t (*) [8][8][16])led.getbuffbk());
        this->nowblock.display=&led;
        this->nowblock.parent=this;
    }
    void pause()
    {
        if(status==0)
        {
            status=1;
            timerup=0;
        }
        else if(status==1)
        {
            status=0;
            timerup=1;
        }
    }
    int input(int mv,int rot);
    void newgame();///-----------------------------------------TODO
    int gameloop();///----------------------------------------TODO
    int gamedraw()///----------------------------------------TODO
    {
        this->led.displayleds();
        return 0;
    }
    uint8_t (* gamectl)();
    char * getmsg()
    {
        switch(status)
        {
        case -1:
            sprintf(output,"Welcome to Tetris3D ! Press \"R\" to begin.");
            break;
        case 0:
            sprintf(output,"Gaming...Scores:%010d",scores);
            break;
        case 2:
            sprintf(output,"Failed...Scores:%010d",scores);
            break;
        case 1:
            sprintf(output,"Paused...Scores:%010d",scores);
            break;
        }
        return output;
    }
    void runstept()
    {
        if(timerup)
            time++;
        if(time%10==0)
            if(timerup)
                gameloop();
    }
    float getangle()///return the look from head clock angle
    {
        return led.getangle();
    }
private:
    uint32_t time=0;
    int timerup=0;
    char output[30];
    int lock=0;
    int lock2=0;
    int cleanlay(int x);
    uint8_t (*blocks)[8][8][16];
    int scores=0;
    block nowblock;
    int isingame=0;
    int nextnew=0;
    int status=-1;
};
#endif // TETRIS3D_H
