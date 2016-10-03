#ifndef TETRIS3D_H
#define TETRIS3D_H
#include "ledblock.h"

typedef struct
{
    int numofbk;
    point points[8];
    int color;
} tetrisbk;


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
    tetris3d(void)
    {}
    void init()
    {
        this->led.init(8,8,16);
        this->blocks=(uint8_t (*) [8][8][16])led.getbuffbk();
        memset(this->blocks,0,8*8*16*sizeof(uint8_t));
        this->nowblock.setbuff((uint8_t (*) [8][8][16])led.getbuffbk());
        this->nowblock.display=&led;
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
private:
    int lock=0;
    int cleanlay(int x);
    uint8_t (*blocks)[8][8][16];
    ledemu led;
    int scores=0;
    block nowblock;
    int isingame=0;
};
#endif // TETRIS3D_H
