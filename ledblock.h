/********************************************//**
 *
 *
 *这是一个led对我的3D库的一个适配
 *
 *
 *
 ***********************************************/

#ifndef LEDBLOCK_H
#define LEDBLOCK_H
#include <stdint.h>
#include "3D.h"
#include <mem.h>
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
        this->rd=this->zero;
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
        this->rd=this->zero;
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
        int i=1;
        M_R3D rm;
        point3D arrow[9]= {setpoint(0,78,30,0xffffff),
                           setpoint(30,78,0,0xffffff   ),
                           setpoint(-30,78,0,0xffffff   ),
                           setpoint(14,78,0,0xffffff   ),
                           setpoint(14,78,-30,0xffffff  ),
                           setpoint(-14,78,-30,0xffffff  ),
                           setpoint(-14,78,-30,0xffffff ),
                           setpoint(-14,78,0,0xffffff   ),
                           setpoint(14,78,0,0xffffff  )
                          };

        point3D point[24]= {setpoint(37,76,37,0xffffff),     setpoint(37,-76,37,0xffffff),
                            setpoint(-37,76,37,0xffffff),    setpoint(-37,-76,37,0xffffff),
                            setpoint(37,76,-37,0xffffff),    setpoint(37,-76,-37,0xffffff),
                            setpoint(-37,76,-37,0xffffff),   setpoint(-37,-76,-37,0xffffff),
                            setpoint(-37,-76,-37,0xffffff),  setpoint(-37,-76,37,0xffffff),
                            setpoint(-37,-76,-37,0xffffff),  setpoint(37,-76,-37,0xffffff),
                            setpoint(37,-76,-37,0xffffff),   setpoint(37,-76,37,0xffffff),
                            setpoint(37,-76,37,0xffffff),    setpoint(-37,-76,37,0xffffff),
                            setpoint(-37,76,-37,0xffffff),   setpoint(-37,76,37,0xffffff),
                            setpoint(-37,76,-37,0xffffff),   setpoint(37,76,-37,0xffffff),
                            setpoint(37,76,-37,0xffffff),    setpoint(37,76,37,0xffffff),
                            setpoint(37,76,37,0xffffff),     setpoint(-37,76,37,0xffffff)
                           };
        getrm(&rm,&rd);
        R3DM(point,rm,24);
        R3DM(arrow,rm,9);

        for(int i=0; i<12; i++)
            drawline3D(point[2*i],point[2*i+1],0xffffff);
        for(int i=0; i<9; i+=3)
            fillthr3D(arrow[i],arrow[i+1],arrow[i+2],0x8080ff);
        for (int x=0; x<8; x++)
            for(int y=0; y<8; y++)
                for(int z=0; z<16; z++)
                    if(getled(x,y,z,0))
                        DrawboxM((x-this->x/2)*10+5,(z-this->z/2)*10+5, (y-this->y/2)*10+5, 1, rd,getled(x,y,z),i--);
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
    pointR3D rd;
private:
    uint8_t * buff;
    uint8_t * buffbk;
    uint8_t x,y,z;
    pointR3D zero= {.rx=0,.ry=0,.rz=0};
};
#endif // LEDBLOCK_H
