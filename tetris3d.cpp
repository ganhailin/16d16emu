#include "tetris3d.h"
#include <time.h>
#include <unistd.h>
void tetris3d::newgame()
{
    lock=1;
    this->isingame=1;
    this->led.clearled();
    this->led.flashled();
    scores=0;
    this->nowblock.bknew();
    lock=0;
    lock2=0;
}
int tetris3d::gameloop()
{
    while(lock2)usleep(10);
    lock=1;
    for(int z=0; z<8; z++)
    {
        int cnt=0;
        for(int x=0; x<4; x++)
            for(int y=0; y<4; y++)
            {
                cnt+=this->led.getled(x*2,y*2,z*2,0)?1:0;
            }
        if(cnt==16)
            this->cleanlay(z);
    }

    if(this->isingame)
        if(this->nowblock.bkdown())
            if(this->nowblock.bknew())
                this->isingame=0;
    this->led.flashled();
    lock=0;
    return 0;
}

int tetris3d::cleanlay(int x)
{
    int color;
    for (int z=x; z<7; z++)
    {
        for(int x=0; x<4; x++)
            for(int y=0; y<4; y++)
            {
                color=this->led.getled(x*2,y*2,(z+1)*2);
                this->led.setled(x*2,y*2,z*2,color);
                this->led.setled(x*2,y*2,z*2+1,color);
                this->led.setled(x*2+1,y*2,z*2,color);
                this->led.setled(x*2+1,y*2,z*2+1,color);
                this->led.setled(x*2,y*2+1,z*2,color);
                this->led.setled(x*2,y*2+1,z*2+1,color);
                this->led.setled(x*2+1,y*2+1,z*2,color);
                this->led.setled(x*2+1,y*2+1,z*2+1,color);
            }
    }
    return 0;
}

int tetris3d::input(int mv,int rot)
{
    lock2=1;
    while(lock)usleep(10);
    switch(mv)
    {
    case 1:///左移
    case 2:///右移
    case 3:///前移
    case 4:///后移
        this->nowblock.bkmove(mv);
        break;
    default:
        break;
    }
    switch(rot)
    {
    case 1:///x+
    case 2:///x-
    case 3:///y+
    case 4:///y-
    case 5:///z+
    case 6:///z-
        this->nowblock.bkrote(rot);
        break;
    default:
        break;
    }
    lock2=0;
    return 0;

}

int block::bkmove(uint8_t x)
{
    int error=0;
    switch(x)
    {
    case 1:///左移
        this->bkclear();
        for(int i=0; i<this->nowblock.numofbk; i++)
        {
            point buffpoint=this->nowblock.points[i];
            buffpoint.x-=1;
            if(this->ckpos(buffpoint))
                error++;
            //this->bkdrawsingle(buffpoint.x+this->posx+2,buffpoint.y+2+this->posy,buffpoint.c+this->posz,0);
        }
        if(error)
        {
            this->bkdraw();
            return -1;
        }
        this->posx-=1;
        this->bkdraw();
        return 0;

        break;
    case 2:///右移
        this->bkclear();
        for(int i=0; i<this->nowblock.numofbk; i++)
        {
            point buffpoint=this->nowblock.points[i];
            buffpoint.x+=1;
            if(this->ckpos(buffpoint))
                error++;
            //this->bkdrawsingle(buffpoint.x+this->posx+2,buffpoint.y+2+this->posy,buffpoint.c+this->posz,0);
        }
        if(error)
        {
            this->bkdraw();
            return -1;
        }
        this->posx+=1;
        this->bkdraw();
        return 0;

        break;
    case 3:///前移
        this->bkclear();
        for(int i=0; i<this->nowblock.numofbk; i++)
        {
            point buffpoint=this->nowblock.points[i];
            buffpoint.y-=1;
            if(this->ckpos(buffpoint))
                error++;
            //this->bkdrawsingle(buffpoint.x+this->posx+2,buffpoint.y+2+this->posy,buffpoint.c+this->posz,0);
        }
        if(error)
        {
            this->bkdraw();
            return -1;
        }
        this->posy-=1;
        this->bkdraw();
        return 0;

        break;
    case 4:///后移
        this->bkclear();
        for(int i=0; i<this->nowblock.numofbk; i++)
        {
            point buffpoint=this->nowblock.points[i];
            buffpoint.y+=1;
            if(this->ckpos(buffpoint))
                error++;
            //this->bkdrawsingle(buffpoint.x+this->posx+2,buffpoint.y+2+this->posy,buffpoint.c+this->posz,0);
        }
        if(error)
        {
            this->bkdraw();
            return -1;
        }
        this->posy+=1;
        this->bkdraw();
        return 0;

        break;
    default:
        break;
    }

    return 0;
}

int block::bkrote(uint8_t x)
{
    rox=roy=roz=0;
    int error=0;
    switch(x)
    {
    case 1:///x+
        this->bkclear();///clear
        rox=1;
        for(int i=0; i<this->nowblock.numofbk; i++)
        {
            point buffpoint=this->rotatepoint(this->nowblock.points[i]);
            if(this->ckpos(buffpoint))
                error++;
        }
        if(error)
        {
            rox=0;
            this->bkdraw();
            return -1;
        }
        for(int i=0; i<this->nowblock.numofbk; i++)
            this->nowblock.points[i]=this->rotatepoint(this->nowblock.points[i]);
        this->bkdraw();
        return 0;
        break;
    case 2:///x-
        this->bkclear();///clear
        rox=3;
        for(int i=0; i<this->nowblock.numofbk; i++)
        {
            point buffpoint=this->rotatepoint(this->nowblock.points[i]);
            if(this->ckpos(buffpoint))
                error++;
        }
        if(error)
        {
            rox=0;
            this->bkdraw();
            return -1;
        }
        for(int i=0; i<this->nowblock.numofbk; i++)
            this->nowblock.points[i]=this->rotatepoint(this->nowblock.points[i]);
        this->bkdraw();
        return 0;
        break;
    case 3:///y+
        this->bkclear();///clear
        roy=1;
        for(int i=0; i<this->nowblock.numofbk; i++)
        {
            point buffpoint=this->rotatepoint(this->nowblock.points[i]);
            if(this->ckpos(buffpoint))
                error++;
        }
        if(error)
        {
            roy=0;
            this->bkdraw();
            return -1;
        }
        for(int i=0; i<this->nowblock.numofbk; i++)
            this->nowblock.points[i]=this->rotatepoint(this->nowblock.points[i]);
        this->bkdraw();
        return 0;
        break;
    case 4:///y-
        this->bkclear();///clear
        roy=3;
        for(int i=0; i<this->nowblock.numofbk; i++)
        {
            point buffpoint=this->rotatepoint(this->nowblock.points[i]);
            if(this->ckpos(buffpoint))
                error++;
        }
        if(error)
        {
            roy=0;
            this->bkdraw();
            return -1;
        }
        for(int i=0; i<this->nowblock.numofbk; i++)
            this->nowblock.points[i]=this->rotatepoint(this->nowblock.points[i]);
        this->bkdraw();
        return 0;
        break;
    case 5:///z+
        this->bkclear();///clear
        roz=1;
        for(int i=0; i<this->nowblock.numofbk; i++)
        {
            point buffpoint=this->rotatepoint(this->nowblock.points[i]);
            if(this->ckpos(buffpoint))
                error++;
        }
        if(error)
        {
            roz=0;
            this->bkdraw();
            return -1;
        }
        for(int i=0; i<this->nowblock.numofbk; i++)
            this->nowblock.points[i]=this->rotatepoint(this->nowblock.points[i]);
        this->bkdraw();
        return 0;
        break;
    case 6:///z-
        this->bkclear();///clear
        roz=3;
        for(int i=0; i<this->nowblock.numofbk; i++)
        {
            point buffpoint=this->rotatepoint(this->nowblock.points[i]);
            if(this->ckpos(buffpoint))
                error++;
        }
        if(error)
        {
            roz=0;
            this->bkdraw();
            return -1;
        }
        for(int i=0; i<this->nowblock.numofbk; i++)
            this->nowblock.points[i]=this->rotatepoint(this->nowblock.points[i]);
        this->bkdraw();
        return 0;
        break;
    default:
        break;
    }

    return 0;
}

int block::bknew()
{
    int error=0;
    srand(clock());
    this->type=rand()%7;
    this->posx=0;               ///####
    this->posy=0;               ///####
    this->posz=7;               ///##0#--------左上角为（-2，-2），O为原点。
    this->rox=rand()%4;         ///####
    this->roy=rand()%4;
    this->roz=rand()%4;
    switch(this->type)
    {
    case 0:///I
        this->nowblock.numofbk=3;
        this->nowblock.points[0]= {.x=1,.y=0,.c=0};
        this->nowblock.points[1]= {.x=0,.y=0,.c=0};
        this->nowblock.points[2]= {.x=-1,.y=0,.c=0};
        this->nowblock.color=0xffffff;
        break;
    case 1:///L
        this->nowblock.numofbk=3;
        this->nowblock.points[0]= {.x=1,.y=0,.c=0};
        this->nowblock.points[1]= {.x=0,.y=0,.c=0};
        this->nowblock.points[2]= {.x=0,.y=0,.c=1};
        this->nowblock.color=0xffffff;
        break;
    case 2:///O
        this->nowblock.numofbk=4;
        this->nowblock.points[0]= {.x=1,.y=0,.c=0};
        this->nowblock.points[1]= {.x=1,.y=0,.c=-1};
        this->nowblock.points[2]= {.x=0,.y=0,.c=0};
        this->nowblock.points[3]= {.x=0,.y=0,.c=-1};
        this->nowblock.color=0xffffff;
        break;
    case 3:///V
        this->nowblock.numofbk=4;
        this->nowblock.points[0]= {.x=0,.y=0,.c=0};
        this->nowblock.points[1]= {.x=0,.y=0,.c=1};
        this->nowblock.points[2]= {.x=0,.y=1,.c=0};
        this->nowblock.points[3]= {.x=1,.y=0,.c=0};
        this->nowblock.color=0xffffff;
        break;
    case 4:///Z
        this->nowblock.numofbk=4;
        this->nowblock.points[0]= {.x=0,.y=0,.c=0};
        this->nowblock.points[1]= {.x=0,.y=1,.c=-1};
        this->nowblock.points[2]= {.x=0,.y=0,.c=-1};
        this->nowblock.points[3]= {.x=-1,.y=0,.c=0};
        this->nowblock.color=0xffffff;
        break;
    case 5:///S
        this->nowblock.numofbk=4;
        this->nowblock.points[0]= {.x=0,.y=0,.c=0};
        this->nowblock.points[1]= {.x=0,.y=1,.c=0};
        this->nowblock.points[2]= {.x=0,.y=0,.c=-1};
        this->nowblock.points[3]= {.x=-1,.y=0,.c=-1};
        this->nowblock.color=0xffffff;
        break;
    case 6:///D
        this->nowblock.numofbk=1;
        this->nowblock.points[0]= {.x=0,.y=0,.c=0};
        this->nowblock.color=0xffffff;
        break;
    default:
        break;
    }
    for(int i=0; i<this->nowblock.numofbk; i++)
    {
        this->nowblock.points[i]=this->rotatepoint(this->nowblock.points[i]);
        if(this->ckpos(this->nowblock.points[i]))
            error++;
        //this->bkdrawsingle(buffpoint.x+this->posx+2,buffpoint.y+2+this->posy,buffpoint.c+this->posz,0);
    }
    if(error)
    {
        //this->bkdraw();
        return -1;
    }
    this->bkdraw();
    this->display->flashled();
    return 0;
}
int block::bkdraw()
{
    for(int i=0; i<this->nowblock.numofbk; i++)
    {
        point buffpoint=this->nowblock.points[i];
        this->bkdrawsingle(buffpoint.x+this->posx+2,buffpoint.y+2+this->posy,buffpoint.c+this->posz,this->nowblock.color);
    }
    //this->display->flashled();
    return 0;
}
int block::bkclear()
{
    for(int i=0; i<this->nowblock.numofbk; i++)
    {
        point buffpoint=this->nowblock.points[i];
        this->bkdrawsingle(buffpoint.x+this->posx+2,buffpoint.y+2+this->posy,buffpoint.c+this->posz,0);
    }
    //this->display->flashled();
    return 0;
}

int block::bkdown()
{
    int error=0;
    this->bkclear();
    for(int i=0; i<this->nowblock.numofbk; i++)
    {
        point buffpoint=this->nowblock.points[i];
        buffpoint.c-=1;
        if(this->ckpos(buffpoint))
            error++;
        //this->bkdrawsingle(buffpoint.x+this->posx+2,buffpoint.y+2+this->posy,buffpoint.c+this->posz,0);
    }
    if(error)
    {
        this->bkdraw();
        return -1;
    }
    this->posz-=1;
    this->bkdraw();
    return 0;
}

int block::ckpos(point pos)
{
    pos.x+=this->posx+2;
    pos.y+=2+this->posy;
    pos.c+=this->posz;
    if(pos.c<0||pos.x<0||pos.x>3||pos.y>3||pos.y<0)
        return -1;
    else if(pos.c>7)
        return 0;
    else
        return this->display->getled(pos.x*2,pos.y*2,pos.c*2,0);
}

point block::rotatepoint(point res)
{
    point change;
    change.x=res.x*intcos(roy)*intcos(roz)-\
             res.y*intcos(roy)*intsin(roz)+\
             res.c*intsin(roy);
    change.y=res.x*(intcos(roz)*intsin(rox)*intsin(roy)+intcos(rox)*intsin(roz))+\
             res.y*(intcos(rox)*intcos(roz)-intsin(rox)*intsin(roy)*intsin(roz))+\
             res.c*-intcos(roy)*intsin(rox);
    change.c=res.x*(-intcos(rox)*intcos(roz)*intsin(roy)+intsin(rox)*intsin(roz))+\
             res.y*(intcos(roz)*intsin(rox)+intcos(rox)*intsin(roy)*intsin(roz))+\
             res.c*intcos(rox)*intcos(roy);
    return change;
}


/**-------------------------一些备份代码    switch(this->type)
    {
    case 0:///I
        break;
    case 1:///L
        break;
    case 2:///O
        break;
    case 3:///V
        break;
    case 4:///Z
        break;
    case 5:///S
        break;
    case 6:///D
        break;
    default:
        break;
    }
    return 0;
------------------**/
