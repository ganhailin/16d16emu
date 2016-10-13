/********************************************//**
 *
 *
 *����һ��led���ҵ�3D���һ������
 *
 *
 *
 ***********************************************/

#ifndef LEDBLOCK_H
#define LEDBLOCK_H
#include <stdint.h>
#include "3D.h"
#include <memory.h>
#include <stdlib.h>
class ledemu
{
public:

    ledemu(void)
    {

    }
    uint8_t Color24to8(int color)
    {
        int R,G,B;
        R=(color>>16)&0xff;
        G=(color>>8)&0xff;
        B=color&0xff;
        float Rf=R,Gf=G,Bf=B;
        Rf=Rf*8/256;
        Gf=Gf*8/256;
        Bf=Bf*4/256;
        R=Rf;
        G=Gf;
        B=Bf;
        return (R<<5)+((G<<2)&0x1c)+(B&0x3);
    }
    int Color8to24(uint8_t color)
    {
        int R,G,B;
        R=(color>>5)&0b111;
        G=(color>>2)&0b111;
        B=color&0b11;
        float Rf=R,Gf=G,Bf=B;
        Rf=Rf*256/8;
        Gf=Gf*256/8;
        Bf=Bf*256/4;
        R=Rf;
        G=Gf;
        B=Bf;
        return (R<<16)+(G<<8&0xff00)+(B&0xff);
    }

    ledemu(uint8_t xi,uint8_t yi,uint8_t zi)
    {
        x=xi;
        y=yi;
        z=zi;
        buff=(uint8_t*)malloc(x*y*z*sizeof(uint8_t));
        buffbk=(uint8_t*)malloc(x*y*z*sizeof(uint8_t));
        clearled();
        memset(buff,0,x*y*z*sizeof(uint8_t));
        flashled();
    }
    void init(uint8_t xi,uint8_t yi,uint8_t zi)
    {
        x=xi;
        y=yi;
        z=zi;
        buff=(uint8_t*)malloc(x*y*z*sizeof(uint8_t));
        buffbk=(uint8_t*)malloc(x*y*z*sizeof(uint8_t));
        clearled();
        memset(buff,0,x*y*z*sizeof(uint8_t));
        flashled();
    }
    uint8_t * getbuffbk()
    {
        return buff;
    }
    void clearled(void)
    {
        memset(buffbk,0,x*y*z*sizeof(uint8_t));
    }
    void flashled(void)
    {
        memcpy(buff,buffbk,x*y*z*sizeof(uint8_t));
    }
    void displayleds(void)
    {
        drawline3D(setpoint(31,70,31,0xffffff),setpoint(31,-81,31,0xffffff),0xffffff);
        drawline3D(setpoint(-41,70,31,0xffffff),setpoint(-41,-81,31,0xffffff),0xffffff);
        drawline3D(setpoint(31,70,-41,0xffffff),setpoint(31,-81,-41,0xffffff),0xffffff);
        drawline3D(setpoint(-41,70,-41,0xffffff),setpoint(-41,-81,-41,0xffffff),0xffffff);
        drawline3D(setpoint(-41,-81,-41,0xffffff),setpoint(-41,-81,31,0xffffff),0xffffff);
        drawline3D(setpoint(-41,-81,-41,0xffffff),setpoint(31,-81,-41,0xffffff),0xffffff);
        drawline3D(setpoint(31,-81,-41,0xffffff),setpoint(31,-81,31,0xffffff),0xffffff);
        drawline3D(setpoint(31,-81,31,0xffffff),setpoint(-41,-81,31,0xffffff),0xffffff);
        drawline3D(setpoint(-41,71,-41,0xffffff),setpoint(-41,71,31,0xffffff),0xffffff);
        drawline3D(setpoint(-41,71,-41,0xffffff),setpoint(31,71,-41,0xffffff),0xffffff);
        drawline3D(setpoint(31,71,-41,0xffffff),setpoint(31,71,31,0xffffff),0xffffff);
        drawline3D(setpoint(31,71,31,0xffffff),setpoint(-41,71,31,0xffffff),0xffffff);
        for (int x=0; x<8; x++)
            for(int y=0; y<8; y++)
                for(int z=0; z<16; z++)
                    if(getled(x,y,z,0))
                    Drawbox2((x-this->x/2)*10,(z-this->z/2)*10, (y-this->y/2)*10, 1, zero, zero,getled(x,y,z) );
    }
    uint32_t getled(uint8_t x,uint8_t y,uint8_t z,bool i=1)
    {
        uint32_t color=*(buffbk+z*64+y*8+x);
        if(i)
            color =Color8to24(color)|(0x1f1f1f);
        else
            color =Color8to24(color);
        return color;
    }
    void setled(uint8_t x,uint8_t y,uint8_t z,uint8_t color)
    {
        *(buffbk+z*64+y*8+x)=color;
    }
    void ledfree()
    {
        free(buff);
        free(buffbk);

    }
private:
    uint8_t * buff;
    uint8_t * buffbk;
    uint8_t x,y,z;
    pointR3D zero= {.rx=0,.ry=0,.rz=0};
};
#endif // LEDBLOCK_H