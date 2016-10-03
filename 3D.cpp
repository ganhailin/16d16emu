#include "SDL2/SDL.h"
#include <math.h>
#include <stdlib.h>
#include "3D.h"
#include "draw.h"
#include "ASICC.h"
#include <string.h>
#include <stdio.h>

#define TEST_
#define xmax 1920
#define ymax 1080

//#define TEST2_

extern unsigned char Font_code[][6];
pointR3D rd0,rd1;
pointR3D camR0;
point3D cam0;
point3D eye0;
void Swap(int &x, int &y)
{
    int temp = y;
    y = x;
    x = temp;
}

void Swapf(float &x, float &y)
{
    float temp = y;
    y = x;
    x = temp;
}

int point3to3(point3D p3d, point3D * p2d)
{
    point3D temp;
    temp.x = -p3d.x - cam0.x;
    temp.y = p3d.y - cam0.y;
    temp.z = p3d.z - cam0.z;
    if (!(camR0.rx == 0 && camR0.ry == 0 && camR0.rz == 0))
        R3D(&temp, &camR0);
    p2d->x = (float)xrad / 2 + (temp.x - eye0.x) * ((eye0.z) / temp.z);
    p2d->y = (float)yrad / 2 + (temp.y - eye0.y) * ((eye0.z) / temp.z);
    p2d->z = temp.z;
    p2d->c = p3d.c;
    return temp.z > 1 ? 1 : 0;
}

#ifdef _DEEPBUFF
static float deepbuff[xmax][ymax];


void DrawPixel3D(int x, int y, float z, int c)
{
    if (x >= 0 && x < xrad && y >= 0 && y < yrad)
        if (z <= deepbuff[x][y])
        {
            deepbuff[x][y] = z;
            Draw_Pixel(x, y, c);
        }
}
void clrdeepbuff(void)
{
    for (int x = 0; x < xrad; x++)
        for (int y = 0; y < yrad; y++)
            deepbuff[x][y] = 4.2E38;
}
#ifdef TEST2_
void Testline(point3D p1,point3D p2,int color);
#endif // TEST2_

void Draw_Simple_Line(float x1, float x2, float z1, float z2, float y, int color)
{
#ifdef TEST2_
    Testline(setpoint(x1,y,z1,0),setpoint(x2,y,z2,0),color);
#else
    Drawline3D(x1, y, z1, x2, y, z2, color);
#endif // TEST2_
}

int Draw_Top_Trangle(float x0, float y0, float z0,
                     float x1, float y1, float z1,
                     float x2, float y2, float z2, int color)
{

    if (y0 == y1)
    {
    }
    else if (y0 == y2)
    {
        Swapf(x2, x1);
        Swapf(y2, y1);
        Swapf(z2, z1);
    }
    else if (y1 == y2)
    {
        Swapf(x0, x2);
        Swapf(y0, y2);
        Swapf(z0, z2);
    }
    else
    {
        return 1;
    }

    if (x1 < x0)
    {
        Swapf(x1, x0);
        Swapf(y1, y0);
        Swapf(z1, z0);
    }
    else if (x1 == x0)
    {
        return 1;
    }

    float dxy_left = (x2 - x0)/(y2 - y0);
    float dxy_right = (x1 - x2) / (y1 - y2);
    float dzy_left = (z2 - z0)/ (y2 - y0);
    float dzy_right = (z2 - z1)/ (y2 - y1);

    float xs = x0, xe = x1, zs = z0, ze = z1;
#ifndef TEST2_
    for (float y = y0; y <= y2; y++)
    {
        //Draw_Simple_Line(xs, xe, zs ,ze , y, color);
        Draw_Simple_Line(x0+(y-y0)*dxy_left, x1+(y-y0)*dxy_right, z0+(y-y0)*dzy_left,z1+(y-y0)*dzy_right, y, color);
        zs += dzy_left;
        ze += dzy_right;

        xs += dxy_left;
        xe += dxy_right;
    }
#else
    for (float y = y0; y <= y2; y+=0.25)
    {
        Draw_Simple_Line(xs, xe, zs,ze, y, color);
        zs += dzy_left/4;
        ze += dzy_right/4;

        xs += dxy_left/4;
        xe += dxy_right/4;
    }
#endif // TEST2_
    return 0;
}
int Draw_Bottom_Trangle(float x0, float y0, float z0,
                        float x1, float y1, float z1,
                        float x2, float y2, float z2, int color)
{
    if (y2 == y1)
    {
    }
    else if (y2 == y0)
    {
        Swapf(x0, x1);
        Swapf(y0, y1);
        Swapf(z0, z1);
    }
    else if (y0 == y1)
    {
        Swapf(x0, x2);
        Swapf(y0, y2);
        Swapf(z0, z2);
    }
    else
    {
        return 1;
    }

    if (x1 < x2)
    {
        Swapf(x1, x2);
        Swapf(y1, y2);
        Swapf(z1, z2);
    }
    else if (x1 == x2)
    {
        return 1;
    }

    float dxy_left = (x2 - x0)/(y2 - y0);
    float dxy_right = (x1 - x0)/(y1 - y0);
    float dzy_left = (z2 - z0)/(y2 - y0);
    float dzy_right = (z1 - z0)/(y1 - y0);

    float xs = x0, xe = x0, zs = z0, ze = z0;
#ifndef TEST2_
    for (float y = y0; y <= y2; y++)
    {
        Draw_Simple_Line(xs, xe, zs,ze, y, color);
        //Draw_Simple_Line(x0+(y-y0)*dxy_left, x0+(y-y0)*dxy_right, z0+(y-y0)*dzy_left ,z0+(y-y0)*dzy_right , y, color);
        zs += dzy_left;
        ze += dzy_right;
        xs += dxy_left;
        xe += dxy_right;
    }
#else
    for (float y = y0; y <= y2; y+=0.25)
    {
        Draw_Simple_Line(xs, xe, zs,ze, y, color);
        zs += dzy_left/4;
        ze += dzy_right/4;
        xs += dxy_left/4;
        xe += dxy_right/4;
    }
#endif // TEST2_
    return 0;
}

int Draw_Trangle_2D(float x0, float y0, float z0,
                    float x1, float y1, float z1,
                    float x2, float y2, float z2,
                    int color)
{
    if (!((x0 >= -100 && x0 < xrad+100 && y0 >= -100 && y0 < yrad+100)&&(x1 >= -100 && x1 < xrad+100 && y1 >= -100 && y1 < yrad+100)&&(x2 >= -100 && x2 < xrad+100 && y2 >= -100 && y2 < yrad+100)))return 0;

    if ((x0 == x1 && x1 == x2) || (y0 == y1 && y1 == y2))
    {
        return 1;
    }

    if (y0 > y1)
    {
        Swapf(x0, x1);
        Swapf(y0, y1);
        Swapf(z0, z1);
    }

    if (y0 > y2)
    {
        Swapf(x0, x2);
        Swapf(y0, y2);
        Swapf(z0, z2);
    }

    if (y1 > y2)
    {
        Swapf(y1, y2);
        Swapf(x1, x2);
        Swapf(z1, z2);
    }

    if (y0 == y1)
    {
        Draw_Top_Trangle(x0, y0, z0, x1, y1, z1, x2, y2, z2, color);
    }
    else if (y1 == y2)
    {
        Draw_Bottom_Trangle(x0, y0, z0, x1, y1, z1, x2, y2, z2, color);
    }
    else
    {
        float new_x = x0 + (y1 - y0) * (x2 - x0) / (y2 - y0);
        float new_z =z0+(y1 - y0)*(z2-z0)  / (y2 - y0);
        Draw_Bottom_Trangle(x0, y0, z0, new_x, y1, new_z, x1, y1, z1, color);
        Draw_Top_Trangle(new_x, y1, new_z, x1, y1, z1, x2, y2, z2, color);
    }


    return 0;
}



int Drawline3D(int x1, int y1, float z1, int x2, int y2, float z2, int color)
{
    if (!((x1 >= -300 && x1 < xrad+300 && y1 >= -300 && y1 < yrad+300)&&(x2 >= -300 && x2 < xrad+300 && y2 >= -300 && y2 < yrad+300)))return 0;
    int dx = x2 - x1, dy = y2 - y1;
    float dz = z2 - z1;
    float dmax = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    for (int i = 0; i < dmax; i++)
    {
        // float k1=(x2-x1)/(z2-z1),k2=(-eye0.x)/(0-eye0.z);
        DrawPixel3D(x1 + dx * i / dmax, y1 + dy * i / dmax, z1 + dz *1.0*( i / dmax),
                    color);

    }
    DrawPixel3D(x2, y2, z2, color);
    return 0;
}
#else
int Draw_Top_Trangle(int x0,int y0,
                     int x1,int y1,
                     int x2,int y2,
                     int color)
{
    //先判断下输入的三角形
    if(y0==y1)
    {
    }
    else if(y0==y2)
    {
        Swap(x2,x1);
        Swap(y2,y1);
    }
    else if(y1==y2)
    {
        Swap(x0,x2);
        Swap(y0,y2);
    }
    else
    {
        return 1 ; //error \brief 不是平顶三角形
    }

    if(x1<x0)
    {
        Swap(x1,x0);
        Swap(y1,y0);
    }
    else if(x1 == x0)
    {
        return 1 ;// error \brief不是三角形
    }

    //计算左右误差
    float dxy_left = (x2-x0)*1.0/(y2-y0) ;
    float dxy_right = (x1-x2)*1.0/(y1-y2);

    //开始进行填充
    float xs = x0,xe = x1 ;
    for(int y=y0 ; y <=y2 ; y++)
    {
        Drawline(int(xs+0.5),y,int(xe+0.5),y,color);

        xs += dxy_left ;
        xe += dxy_right ;
    }
    return 0;
} // end Draw_Top_Trangle


int Draw_Bottom_Trangle(int x0,int y0,
                        int x1,int y1,
                        int x2,int y2,
                        int color)
{
    //先判断下输入的三角形
    if(y2==y1)
    {
    }
    else if(y2==y0)
    {
        Swap(x0,x1);
        Swap(y0,y1);
    }
    else if(y0==y1)
    {
        Swap(x0,x2);
        Swap(y0,y2);
    }
    else
    {
        return 1 ; //error \brief 不是平顶三角形
    }

    if(x1<x2)
    {
        Swap(x1,x2);
    }
    else if(x1 == x2)
    {
        return 1 ;// error \brief不是三角形
    }

    //计算左右误差
    float dxy_left = (x2-x0)*1.0/(y2-y0) ;
    float dxy_right = (x1-x0)*1.0/(y1-y0);

    //开始进行填充
    float xs = x0,xe = x0 ;
    for(int y=y0 ; y <=y2 ; y++)
    {
        Drawline(int(xs+0.5),y,int(xe+0.5),y,color);

        xs += dxy_left ;
        xe += dxy_right ;
    }
    return 0;
}// end Draw_Bottom_Trangle

int Draw_Trangle_2D(int x0,int y0,
                    int x1,int y1,
                    int x2,int y2,
                    int color)
{
    if((x0==x1&&x1==x2)
            ||(y0==y1&&y1==y2))
    {
        return 1 ; //error \brief传进来的点无法构成三角形
    }

    //\brief 将三个顶点按照从上到下排序
    if(y0>y1)
    {
        Swap(x0,x1);
        Swap(y0,y1);
    }

    if(y0>y2)
    {
        Swap(x0,x2);
        Swap(y0,y2);
    }

    if(y1>y2)
    {
        Swap(y1,y2);
        Swap(x1,x2);
    }


    //\brief 进行绘制
    if(y0 == y1) //\brief 平顶三角形
    {
        Draw_Top_Trangle(x0,y0,x1,y1,x2,y2,color);
    }
    else if(y1 == y2)
    {
        Draw_Bottom_Trangle(x0,y0,x1,y1,x2,y2,color);
    }
    else
    {
        int new_x = x0+0.5+(float)1.0*(y1-y0)*(x2-x0)/(y2-y0);
        Draw_Bottom_Trangle(x0,y0,new_x,y1,x1,y1,color);
        Draw_Top_Trangle(new_x,y1,x1,y1,x2,y2,color);
    }


    return 0 ; //\brief 成功画出三角形
}// end Draw_Trangle_2D
#endif

void fillthr3D(point3D p1, point3D p2, point3D p3, int color)
{
#ifdef _DEEPBUFF
#ifndef TEST2_
    if (point3to3(p1, &p1) && point3to3(p2, &p2) && point3to3(p3, &p3))
#endif // TEST2_
    {
        Draw_Trangle_2D(p1.x,p1.y,p1.z,p2.x,p2.y,p2.z,p3.x,p3.y,p3.z,color);
    }
#else
    if (point3to3(p1, &p1) && point3to3(p2, &p2) && point3to3(p3, &p3))
    {
        Draw_Trangle_2D(p1.x,p1.y,p2.x,p2.y,p3.x,p3.y,color);/*
        Drawline3D(p1.x,p1.y,p1.z,p2.x,p2.y,p2.z,color)
        Drawline3D(p3.x,p3.y,p3.z,p2.x,p2.y,p2.z,color)
        Drawline3D(p1.x,p1.y,p1.z,p3.x,p3.y,p3.z,color)*/
    }

#endif
}



void DrawPixel(int x, int y, int c)
{
    if (x >= 0 && x < xrad && y >= 0 && y < yrad)
        Draw_Pixel(x, y, c);
}


int Drawline(int x1, int y1, int x2, int y2, int color)
{
    int y, x;

    if (y1 == y2)
    {
        y = y1;
        if (x1 >= x2)
            for (x = x2; x < x1; x++)
                DrawPixel(x, y, color);
        else
            for (x = x1; x < x2; x++)
                DrawPixel(x, y, color);
    }
    if (x1 == x2)
    {
        x = x1;
        if (y1 <= y2)
            for (y = y1; y < y2; y++)
                DrawPixel(x, y, color);
        else
            for (y = y2; y < y1; y++)
                DrawPixel(x, y, color);


    }
    else
    {
        int dx = abs(x1 - x2), dy = abs(y1 - y2);

        if (dy > dx)
        {
            if (y1 < y2)
            {
                for (y = y1; y <= y2; y++)
                    DrawPixel(x1 + (x2 - x1) * (y - y1) / dy, y, color);
            }
            else
                Drawline(x2, y2, x1, y1, color);
        }
        else
        {
            if (x1 < x2)
            {
                for (x = x1; x <= x2; x++)
                    DrawPixel(x, y1 + (y2 - y1) * (x - x1) / dx, color);
            }
            else
                Drawline(x2, y2, x1, y1, color);

        }
    }
    return 0;
}


void fillthr(Point p1, Point p2, Point p3, int color)
{

    float dec1, dec2, d1, d2;
    int y;
    Point p4;

    if (p1.y == p2.y)
    {
        dec1 = (float)(((float)(p1.y - p3.y)) / (float)((p1.x - p3.x)));
        d1 = p1.y - dec1 * p1.x;
        dec2 = (float)(((float)(p2.y - p3.y)) / (float)((p2.x - p3.x)));
        d2 = p2.y - dec2 * p2.x;

        if (p3.y > p2.y)
            if (p1.x == p3.x)
                for (y = p2.y; y <= p3.y; y++)
                    Drawline(p3.x, y, (int)((y - d2) / dec2), y, color);
            else if (p2.x == p3.x)
                for (y = p2.y; y <= p3.y; y++)
                    Drawline((int)((y - d1) / dec1), y, p3.x, y, color);
            else
                for (y = p2.y; y <= p3.y; y++)
                    Drawline((int)((y - d1) / dec1), y, (int)((y - d2) / dec2),
                             y, color);
        else if (p1.x == p3.x)
            for (y = p3.y; y <= p2.y; y++)
                Drawline(p3.x, y, (int)((y - d2) / dec2), y, color);
        else if (p2.x == p3.x)
            for (y = p3.y; y <= p2.y; y++)
                Drawline((int)((y - d1) / dec1), y, p3.x, y, color);
        else
            for (y = p3.y; y <= p2.y; y++)
                Drawline((y - d1) / dec1, y, (y - d2) / dec2, y, color);
    }
    else
    {
        if (p2.y == p3.y)
            fillthr(p2, p3, p1, color);
        else
        {
            if (p1.y == p3.y)
                fillthr(p1, p3, p2, color);
            else
            {
                if (((p2.y < p1.y) && (p2.y > p3.y))
                        || ((p2.y > p1.y) && (p2.y < p3.y)))
                {
                    p4.y = p2.y;
                    dec1 =
                        (float)(((float)(p1.y - p3.y)) /
                                ((float)(p1.x - p3.x)));
                    d1 = (float)(((float)p1.y) - ((float)(dec1 * p1.x)));
                    p4.x = (p4.y - d1) / dec1;
                    fillthr(p1, p4, p2, color);
                    fillthr(p3, p4, p2, color);
                }
                else
                {
                    if (((p1.y < p2.y) && (p1.y > p3.y))
                            || ((p2.y < p1.y) && (p1.y < p3.y)))
                        fillthr(p3, p1, p2, color);
                    else
                    {
                        if (((p3.y < p1.y) && (p3.y > p2.y))
                                || ((p3.y > p1.y) && (p3.y < p2.y)))
                            fillthr(p1, p3, p2, color);
                    }
                }
            }
        }
    }
}




void R3D(point3D * p3d, pointR3D * p3dr)
{

    float cosrx = cosf(p3dr->rx),
          cosry = cosf(-p3dr->ry),
          cosrz = cosf(-p3dr->rz),
          sinrx = sinf(p3dr->rx),
          sinry = sinf(-p3dr->ry),
          sinrz = sinf(-p3dr->rz);
    float x =
        (cosry * cosrz) * p3d->x +
        (-cosry * sinrz) * p3d->y + (sinry) * p3d->z;
    float y =
        (cosrz * sinrx * sinry + cosrx * sinrz) * p3d->x +
        (cosrx * cosrz - sinrx * sinry * sinrz) * p3d->y +
        (-cosry * sinrx) * p3d->z;
    float z =
        (-cosrx * cosrz * sinry + sinrx * sinrz) * p3d->x +
        (cosrz * sinrx + cosrx * sinry * sinrz) * p3d->y +
        (cosrx * cosry) * p3d->z;
    p3d->x = x;
    p3d->y = y;
    p3d->z = z;
}

void R3DM(point3D * p3d, M_R3D rm, int count)
{
    for (int i = 0; i < count; i++)
    {
        float x = rm.x1 * p3d[i].x + p3d[i].y * rm.x2 + rm.x3 * p3d[i].z;
        float y = rm.y1 * p3d[i].x + p3d[i].y * rm.y2 + rm.y3 * p3d[i].z;
        float z = rm.z1 * p3d[i].x + p3d[i].y * rm.z2 + rm.z3 * p3d[i].z;
        p3d[i].x = x;
        p3d[i].y = y;
        p3d[i].z = z;
    }
}


void getrm(M_R3D * rm, pointR3D * p3dr)
{

    float cosrx = cosf(p3dr->rx),
          cosry = cosf(-p3dr->ry),
          cosrz = cosf(-p3dr->rz),
          sinrx = sinf(p3dr->rx),
          sinry = sinf(-p3dr->ry),
          sinrz = sinf(-p3dr->rz);

    rm->x1 = (cosry * cosrz);
    rm->x2 = (-cosry * sinrz);
    rm->x3 = (sinry);
    rm->y1 = (cosrz * sinrx * sinry + cosrx * sinrz);
    rm->y2 = (cosrx * cosrz - sinrx * sinry * sinrz);
    rm->y3 = (-cosry * sinrx);
    rm->z1 = (-cosrx * cosrz * sinry + sinrx * sinrz);
    rm->z2 = (cosrz * sinrx + cosrx * sinry * sinrz);
    rm->z3 = (cosrx * cosry);
}

int point3to2(point3D p3d, point * p2d)
{
    point3D temp;
    temp.x = -p3d.x - cam0.x;
    temp.y = p3d.y - cam0.y;
    temp.z = p3d.z - cam0.z;
    if (!(camR0.rx == 0 && camR0.ry == 0 && camR0.rz == 0))
        R3D(&temp, &camR0);
    p2d->x = xrad / 2 + (temp.x - eye0.x) * ((eye0.z) / temp.z);
    p2d->y = yrad / 2 + (temp.y - eye0.y) * ((eye0.z) / temp.z);
    p2d->c = p3d.c;
    return temp.z > 1 ? 1 : 0;
}


#ifdef _DEEPBUFF
void drawline3D(point3D p1, point3D p2, int color)
{
#ifdef DEBUG

    printf("%d,%d,%d,%d,%d\n", p1.x, p1.y, p2.x, p2.y, color);
#endif
    if (point3to3(p1, &p1))
        if (point3to3(p2, &p2))
        {
            Drawline3D(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, color);
        }
}
#else
void drawline3D(point3D p1, point3D p2, int color)
{
#ifdef DEBUG

    printf("%d,%d,%d,%d,%d\n", p1.x, p1.y, p2.x, p2.y, color);
#endif
    point p21,p22;
    if (point3to2(p1, &p21))
        if (point3to2(p2, &p22))
        {
            Drawline(p21.x, p21.y, p22.x, p22.y, color);
        }
}
#endif // _DEEPBUFF

void Drawbox(float x, float y, float z, float r, pointR3D rd, int color)
{
    point3D box[8];
    M_R3D rm;
    box[0].x = x-r;
    box[0].y = y-r;
    box[0].z = z-r;
    box[1].x = x+r;
    box[1].y = y-r;
    box[1].z = z-r;
    box[2].x = x-r;
    box[2].y = y+r;
    box[2].z = z-r;
    box[3].x = x+r;
    box[3].y = y+r;
    box[3].z = z-r;
    box[4].x = x-r;
    box[4].y = y-r;
    box[4].z = z+r;
    box[5].x = x+r;
    box[5].y = y-r;
    box[5].z = z+r;
    box[6].x = x-r;
    box[6].y = y+r;
    box[6].z = z+r;
    box[7].x = x+r;
    box[7].y = y+r;
    box[7].z = z+r;
    getrm(&rm, &rd);
    R3DM(box, rm, 8);
#ifdef TEST_
//    int color2=(((color>>4)&0xff)*2/3)<<4|(((color>>2)&0xff)*2/3)<<2|((color&0xff)*2/3);
    fillthr3D(box[0],box[1],box[3],color);
    fillthr3D(box[0],box[2],box[3],color);
    fillthr3D(box[0],box[1],box[5],color);
    fillthr3D(box[0],box[4],box[5],color);
    fillthr3D(box[2],box[3],box[7],color);
    fillthr3D(box[2],box[6],box[7],color);
    fillthr3D(box[6],box[7],box[5],color);
    fillthr3D(box[6],box[4],box[5],color);
    fillthr3D(box[0],box[2],box[6],color);
    fillthr3D(box[0],box[4],box[6],color);
    fillthr3D(box[1],box[5],box[7],color);
    fillthr3D(box[1],box[3],box[7],color);/*
    drawline3D(box[4], box[6], color2);
    drawline3D(box[4], box[5], color2);
    drawline3D(box[5], box[7], color2);
    drawline3D(box[6], box[7], color2);
    drawline3D(box[0], box[1], color2);
    drawline3D(box[0], box[2], color2);
    drawline3D(box[0], box[4], color2);
    drawline3D(box[1], box[5], color2);
    drawline3D(box[1], box[3], color2);
    drawline3D(box[2], box[3], color2);
    drawline3D(box[2], box[6], color2);
    drawline3D(box[3], box[7], color2);*/

#else
    drawline3D(box[4], box[6], color);
    drawline3D(box[4], box[5], color);
    drawline3D(box[5], box[7], color);
    drawline3D(box[6], box[7], color);
    drawline3D(box[0], box[1], color);
    drawline3D(box[0], box[2], color);
    drawline3D(box[0], box[4], color);
    drawline3D(box[1], box[5], color);
    drawline3D(box[1], box[3], color);
    drawline3D(box[2], box[3], color);
    drawline3D(box[2], box[6], color);
    drawline3D(box[3], box[7], color);
#endif // TEST_
}
void Drawbox2(float x, float y, float z, float r, pointR3D rd,pointR3D rd0, int color)
{
    point3D box[8];
    M_R3D rm;
    box[0].x = -r;
    box[0].y = -r;
    box[0].z = -r;
    box[1].x = +r;
    box[1].y = -r;
    box[1].z = -r;
    box[2].x = -r;
    box[2].y = +r;
    box[2].z = -r;
    box[3].x = +r;
    box[3].y = +r;
    box[3].z = -r;
    box[4].x = -r;
    box[4].y = -r;
    box[4].z = +r;
    box[5].x = +r;
    box[5].y = -r;
    box[5].z = +r;
    box[6].x = -r;
    box[6].y = +r;
    box[6].z = +r;
    box[7].x = +r;
    box[7].y = +r;
    box[7].z = +r;
    getrm(&rm, &rd);
    R3DM(box, rm, 8);
    box[0].x += x;
    box[0].y += y;
    box[0].z += z;
    box[1].x += x;
    box[1].y += y;
    box[1].z += z;
    box[2].x += x;
    box[2].y += y;
    box[2].z += z;
    box[3].x += x;
    box[3].y += y;
    box[3].z += z;
    box[4].x += x;
    box[4].y += y;
    box[4].z += z;
    box[5].x += x;
    box[5].y += y;
    box[5].z += z;
    box[6].x += x;
    box[6].y += y;
    box[6].z += z;
    box[7].x += x;
    box[7].y += y;
    box[7].z += z;
    getrm(&rm, &rd0);
    R3DM(box, rm, 8);
#ifdef TEST_
    fillthr3D(box[0],box[1],box[3],color);
    fillthr3D(box[0],box[2],box[3],color);
    fillthr3D(box[0],box[1],box[5],color);
    fillthr3D(box[0],box[4],box[5],color);
    fillthr3D(box[2],box[3],box[7],color);
    fillthr3D(box[2],box[6],box[7],color);
    fillthr3D(box[6],box[7],box[5],color);
    fillthr3D(box[6],box[4],box[5],color);
    fillthr3D(box[0],box[2],box[6],color);
    fillthr3D(box[0],box[4],box[6],color);
    fillthr3D(box[1],box[5],box[7],color);
    fillthr3D(box[1],box[3],box[7],color);/*
    int color2=(((color/65535)&0xff)*2/3)<<4|(((color=256)&0xff)*2/3)<<2|((color&0xff)*2/3);
    drawline3D(box[4], box[6], color2);
    drawline3D(box[4], box[5], color2);
    drawline3D(box[5], box[7], color2);
    drawline3D(box[6], box[7], color2);
    drawline3D(box[0], box[1], color2);
    drawline3D(box[0], box[2], color2);
    drawline3D(box[0], box[4], color2);
    drawline3D(box[1], box[5], color2);
    drawline3D(box[1], box[3], color2);
    drawline3D(box[2], box[3], color2);
    drawline3D(box[2], box[6], color2);
    drawline3D(box[3], box[7], color2);*/

#else
    drawline3D(box[4], box[6], color);
    drawline3D(box[4], box[5], color);
    drawline3D(box[5], box[7], color);
    drawline3D(box[6], box[7], color);
    drawline3D(box[0], box[1], color);
    drawline3D(box[0], box[2], color);
    drawline3D(box[0], box[4], color);
    drawline3D(box[1], box[5], color);
    drawline3D(box[1], box[3], color);
    drawline3D(box[2], box[3], color);
    drawline3D(box[2], box[6], color);
    drawline3D(box[3], box[7], color);
#endif // TEST_
}

void DrawboxM(float x, float y, float z, float r, pointR3D rd, int color, int is)
{

    point3D box[8];
    M_R3D rm;
    box[0].x = x-r;
    box[0].y = y-r;
    box[0].z = z-r;
    box[1].x = x+r;
    box[1].y = y-r;
    box[1].z = z-r;
    box[2].x = x-r;
    box[2].y = y+r;
    box[2].z = z-r;
    box[3].x = x+r;
    box[3].y = y+r;
    box[3].z = z-r;
    box[4].x = x-r;
    box[4].y = y-r;
    box[4].z = z+r;
    box[5].x = x+r;
    box[5].y = y-r;
    box[5].z = z+r;
    box[6].x = x-r;
    box[6].y = y+r;
    box[6].z = z+r;
    box[7].x = x+r;
    box[7].y = y+r;
    box[7].z = z+r;
    if (is > 0)
        getrm(&rm, &rd);
    R3DM(box, rm, 8);
#ifdef TEST_

    fillthr3D(box[0],box[1],box[3],color);
    fillthr3D(box[0],box[2],box[3],color);
    fillthr3D(box[0],box[1],box[5],color);
    fillthr3D(box[0],box[4],box[5],color);
    fillthr3D(box[2],box[3],box[7],color);
    fillthr3D(box[2],box[6],box[7],color);
    fillthr3D(box[6],box[7],box[5],color);
    fillthr3D(box[6],box[4],box[5],color);
    fillthr3D(box[0],box[2],box[6],color);
    fillthr3D(box[0],box[4],box[6],color);
    fillthr3D(box[1],box[5],box[7],color);
    fillthr3D(box[1],box[3],box[7],color);
    fillthr3D(box[0],box[1],box[2],color);
    fillthr3D(box[1],box[2],box[3],color);
    fillthr3D(box[0],box[1],box[4],color);
    fillthr3D(box[1],box[4],box[5],color);
    fillthr3D(box[2],box[3],box[6],color);
    fillthr3D(box[3],box[6],box[7],color);
    fillthr3D(box[6],box[7],box[4],color);
    fillthr3D(box[7],box[4],box[5],color);
    fillthr3D(box[0],box[2],box[4],color);
    fillthr3D(box[2],box[4],box[6],color);
    fillthr3D(box[1],box[5],box[3],color);
    fillthr3D(box[5],box[3],box[7],color);/*
    int color2=(((color/65535)&0xff)*2/3)<<4|(((color=256)&0xff)*2/3)<<2|((color&0xff)*2/3);
    drawline3D(box[4], box[6], color2);
    drawline3D(box[4], box[5], color2);
    drawline3D(box[5], box[7], color2);
    drawline3D(box[6], box[7], color2);
    drawline3D(box[0], box[1], color2);
    drawline3D(box[0], box[2], color2);
    drawline3D(box[0], box[4], color2);
    drawline3D(box[1], box[5], color2);
    drawline3D(box[1], box[3], color2);
    drawline3D(box[2], box[3], color2);
    drawline3D(box[2], box[6], color2);
    drawline3D(box[3], box[7], color2);*/

#else
    drawline3D(box[4], box[6], color);
    drawline3D(box[4], box[5], color);
    drawline3D(box[5], box[7], color);
    drawline3D(box[6], box[7], color);
    drawline3D(box[0], box[1], color);
    drawline3D(box[0], box[2], color);
    drawline3D(box[0], box[4], color);
    drawline3D(box[1], box[5], color);
    drawline3D(box[1], box[3], color);
    drawline3D(box[2], box[3], color);
    drawline3D(box[2], box[6], color);
    drawline3D(box[3], box[7], color);
#endif // TEST_

}

void DrawboxM2(float x, float y, float z, float r, pointR3D rd,pointR3D rd2, int color, int is)
{

    point3D box[8];
    M_R3D rm,rm2;
    box[0].x = -r;
    box[0].y = -r;
    box[0].z = -r;
    box[1].x = +r;
    box[1].y = -r;
    box[1].z = -r;
    box[2].x = -r;
    box[2].y = +r;
    box[2].z = -r;
    box[3].x = +r;
    box[3].y = +r;
    box[3].z = -r;
    box[4].x = -r;
    box[4].y = -r;
    box[4].z = +r;
    box[5].x = +r;
    box[5].y = -r;
    box[5].z = +r;
    box[6].x = -r;
    box[6].y = +r;
    box[6].z = +r;
    box[7].x = +r;
    box[7].y = +r;
    box[7].z = +r;
    if (is > 0)
        getrm(&rm, &rd);
    R3DM(box, rm, 8);
    box[0].x += x;
    box[0].y += y;
    box[0].z += z;
    box[1].x += x;
    box[1].y += y;
    box[1].z += z;
    box[2].x += x;
    box[2].y += y;
    box[2].z += z;
    box[3].x += x;
    box[3].y += y;
    box[3].z += z;
    box[4].x += x;
    box[4].y += y;
    box[4].z += z;
    box[5].x += x;
    box[5].y += y;
    box[5].z += z;
    box[6].x += x;
    box[6].y += y;
    box[6].z += z;
    box[7].x += x;
    box[7].y += y;
    box[7].z += z;
    if (is > 0)
        getrm(&rm2, &rd2);
    R3DM(box, rm2, 8);
#ifdef TEST_

    fillthr3D(box[0],box[1],box[3],color);
    fillthr3D(box[0],box[2],box[3],color);
    fillthr3D(box[0],box[1],box[5],color);
    fillthr3D(box[0],box[4],box[5],color);
    fillthr3D(box[2],box[3],box[7],color);
    fillthr3D(box[2],box[6],box[7],color);
    fillthr3D(box[6],box[7],box[5],color);
    fillthr3D(box[6],box[4],box[5],color);
    fillthr3D(box[0],box[2],box[6],color);
    fillthr3D(box[0],box[4],box[6],color);
    fillthr3D(box[1],box[5],box[7],color);
    fillthr3D(box[1],box[3],box[7],color);
    fillthr3D(box[0],box[1],box[2],color);
    fillthr3D(box[1],box[2],box[3],color);
    fillthr3D(box[0],box[1],box[4],color);
    fillthr3D(box[1],box[4],box[5],color);
    fillthr3D(box[2],box[3],box[6],color);
    fillthr3D(box[3],box[6],box[7],color);
    fillthr3D(box[6],box[7],box[4],color);
    fillthr3D(box[7],box[4],box[5],color);
    fillthr3D(box[0],box[2],box[4],color);
    fillthr3D(box[2],box[4],box[6],color);
    fillthr3D(box[1],box[5],box[3],color);
    fillthr3D(box[5],box[3],box[7],color);/*
    int color2=(((color/65535)&0xff)*2/3)<<4|(((color=256)&0xff)*2/3)<<2|((color&0xff)*2/3);
    drawline3D(box[4], box[6], color2);
    drawline3D(box[4], box[5], color2);
    drawline3D(box[5], box[7], color2);
    drawline3D(box[6], box[7], color2);
    drawline3D(box[0], box[1], color2);
    drawline3D(box[0], box[2], color2);
    drawline3D(box[0], box[4], color2);
    drawline3D(box[1], box[5], color2);
    drawline3D(box[1], box[3], color2);
    drawline3D(box[2], box[3], color2);
    drawline3D(box[2], box[6], color2);
    drawline3D(box[3], box[7], color2);*/

#else
    drawline3D(box[4], box[6], color);
    drawline3D(box[4], box[5], color);
    drawline3D(box[5], box[7], color);
    drawline3D(box[6], box[7], color);
    drawline3D(box[0], box[1], color);
    drawline3D(box[0], box[2], color);
    drawline3D(box[0], box[4], color);
    drawline3D(box[1], box[5], color);
    drawline3D(box[1], box[3], color);
    drawline3D(box[2], box[3], color);
    drawline3D(box[2], box[6], color);
    drawline3D(box[3], box[7], color);
#endif // TEST_

}


void Printc3D(int x, int y, int z, unsigned char c_dat, int color, int is)
{
    unsigned char i, j;
    c_dat -= 32;
    for (i = 0; i < 6; i++)
        for (j = 0; j < 8; j++)
            if ((Font_code[c_dat][i] << j) & 0x80)
                DrawboxM(x + i * BIG, (y + j * BIG), z, BIG/2 -0.5, rd0, color,
                         is--);

}

void Prints3D(int x, int y, int z, unsigned char *s_dat, int color)
{
    int i = 1;
    x -= strlen((const char *)s_dat) / 2 * 6 * BIG + 6 * BIG;
    x -= (strlen((const char *)s_dat) % 2) ? 3 * BIG : 0;
    while (*s_dat)
    {
        Printc3D(x += 6 * BIG, y, z - BIG / 2, *s_dat, color, i);
        s_dat++;
        i--;
    }
}
void Printc3D2(int x, int y, int z, unsigned char c_dat, int color, int is)
{
    unsigned char i, j;
    c_dat -= 32;
    for (i = 0; i < 6; i++)
        for (j = 0; j < 8; j++)
            if ((Font_code[c_dat][i] << j) & 0x80)
                DrawboxM2(x + i * BIG, (y + j * BIG), z, BIG/2 -0.5, rd1,rd0, color,
                          is--);

}

void Prints3D2(int x, int y, int z, unsigned char *s_dat, int color)
{
    int i = 1;
    x -= strlen((const char *)s_dat) / 2 * 6 * BIG + 6 * BIG;
    x -= (strlen((const char *)s_dat) % 2) ? 3 * BIG : 0;
    while (*s_dat)
    {
        Printc3D2(x += 6 * BIG, y, z - BIG / 2, *s_dat, color, i);
        s_dat++;
        i--;
    }
}

void setcam(point3D p3d, pointR3D r3d)
{
    cam0.z = p3d.z;
    camR0.rz = r3d.rz;
    cam0.y = p3d.y;
    camR0.ry = -r3d.ry;
    cam0.x = p3d.x;
    camR0.rx = -r3d.rx;
}

void seteye(point3D p3d)
{
    eye0.x = p3d.x;
    eye0.y = p3d.y;
    eye0.z = p3d.z;
}



point3D setpoint(float x, float y, float z, int c)
{
    point3D p1;
    p1.x = x;
    p1.y = y;
    p1.z = z;
    p1.c = c;
    return p1;
}

pointR3D setpointR(float x, float y, float z)
{
    pointR3D p1;
    p1.rx = x;
    p1.ry = y;
    p1.rz = z;
    return p1;
}


void setrd0(pointR3D rd)
{
    rd0=rd;
}
void setrd1(pointR3D rd)
{
    rd1=rd;
}


////////////////////////////////////////////////////////////////////////////////////////
#ifdef TEST2_
void Testline(point3D p1,point3D p2,int color);
void Testpoint(float x,float y,float z,int color);
void Testline(point3D p1,point3D p2,int color)
{
    float dx = p2.x - p1.x, dy = p2.y - p1.y, dz = p2.z - p1.z;
    float dmax = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    dmax = dmax > abs(dz) ? dmax : abs(dz);
    for (float i = 0; i < dmax; i += 0.25)
    {
        Testpoint(p1.x + dx * i / dmax, p1.y + dy * i / dmax, p1.z + dz * i / dmax, color);
        // printf("(%.0f,%.0f,%.0f)(%.0f,%.0f,%.0f)\r\n",p1.x,p1.y,p1.z,p2.x,p2.y,p2.z);

    }

}

void Testpoint(float x,float y,float z,int color)
{
    point temp;
    if(point3to2(setpoint(x,y,z,color),&temp))
        if (temp.x >= 0 && temp.x < xrad && temp.y >= 0 && temp.y < yrad)
            if (z <= deepbuff[(int)temp.x][(int)temp.y])
            {
                deepbuff[(int)temp.x][(int)temp.y]=z;
                Draw_Pixel(temp.x, temp.y, color);
            }
}
#endif // TEST2_

void drawchar(int x,int y,unsigned char ch,int c)
{
    y+=8;
    unsigned char i, j;
    ch -= 32;
    for (i = 0; i < 6; i++)
        for (j = 0; j < 8; j++)
            if ((Font_code[ch][i] << j) & 0x80)
                DrawPixel(x + i, y - j,c);


}
void drawstring(int x,int y,unsigned char* ch,int c)
{
    x-=6;
    while (*ch)
    {
        drawchar(x += 6, y, *ch, c);
        ch++;
    }

}

float D2R(float D)
{
    return 3.14159265358979323846/180*D;

}
float R2D(float D)
{
    return D/3.14159265358979323846*180;

}

int movecamR(float x,float y,float z)
{
    float iftoobig=R2D(camR0.rx);
    if(iftoobig>90&&x>0)return 0;
    if(iftoobig<-90&&x<0)return 0;
    camR0.rx+=D2R(x);
    camR0.ry+=D2R(y);
    camR0.rz+=D2R(z);
    return 1;
}
int movecam(float x,float y,float z)
{
    cam0.x+=z*sinf(camR0.ry);
    cam0.z+=z*cosf(camR0.ry);
    cam0.z-=x*sinf(camR0.ry);
    cam0.x+=x*cosf(camR0.ry);
    cam0.y+=y;
    return 0;
}

void drawcer(int x,int y,int r,int c)
{
    for (int i=x-r; i<=x; i++)
    {
        int y1=sqrt(r*r-(i-x)*(i-x));
        Drawline(i,y+y1,i,y-y1,c);
        Drawline(2*x-i,y+y1,2*x-i,y-y1,c);

    }
}
