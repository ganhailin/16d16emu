#ifndef _3D_H
#define _3D_H
#define PI 3.14159265358979323846
#define BIG 6
#define TANR 500
#define _DEEPBUFF

typedef struct
{
    int x;
    int y;
} Point;

typedef struct
{
    float x1, x2, x3, y1, y2, y3, z1, z2, z3;
} M_R3D;

typedef struct
{
    float x;
    float y;
    float z;
    int c;
} point3D;

typedef struct
{
    float rx;
    float ry;
    float rz;
} pointR3D;

typedef struct
{
    int x;
    int y;
    int c;
} point;

typedef struct
{
    float x;
    float y;
    int c;
} pointf;
float D2R(float D);
float R2D(float D);
void drawchar(int x,int y,unsigned char ch,int c);
void drawstring(int x,int y,unsigned char* ch,int c);
void DrawPixel(int x, int y, int c);
void fillthr(Point p1, Point p2, Point p3, int color);
int Drawline(int x1, int y1, int x2, int y2, int color);
void R3Dx(point3D * p3d, float rx);
void R3Dy(point3D * p3d, float ry);
void R3Dz(point3D * p3d, float rz);
void R3D(point3D * p3d, pointR3D * p3dr);
int point3to2(point3D p3d, point * p2d);
void drawline3D(point3D p1, point3D p2, int color);
void Drawbox(float x, float y, float z, float r, pointR3D rd, int color);
void DrawboxM(float x, float y, float z, float r, pointR3D rd, int color, int is);
void Drawbox2(float x, float y, float z, float r, pointR3D rd, pointR3D rd0, int color);
void DrawboxM2(float x, float y, float z, float r, pointR3D rd,pointR3D rd0, int color, int is);
void Printc3D(int x, int y, int z, unsigned char c_dat, int color,int is);
void Prints3D(int x, int y, int z, unsigned char *s_dat, int color);
void Printc3D2(int x, int y, int z, unsigned char c_dat, int color, int is);
void Prints3D2(int x, int y, int z, unsigned char *s_dat, int color);
void setcam(point3D p3d, pointR3D r3d);
void seteye(point3D p3d);

#ifdef _DEEPBUFF
void clrdeepbuff(void);
void DrawPixel3D(int x, int y, float z, int c);
int Drawline3D(int x1, int y1, float z1, int x2, int y2, float z2, int color);
void fillthr3D(point3D p1, point3D p2, point3D p3, int color);
int Draw_Trangle_2D(int x0, int y0, float z0,
                    int x1, int y1, float z1, int x2, int y2, float z2, int color);
int point3to3(point3D p3d, point3D * p2d);

#endif
void fillthr3D(point3D p1, point3D p2, point3D p3, int color);
int movecamR(float x,float y,float z);
int movecam(float x,float y,float z);

pointR3D setpointR(float x, float y, float z);
point3D setpoint(float x, float y, float z, int c);
void getrm(M_R3D * rm, pointR3D * p3dr);
void R3DM(point3D * p3d, M_R3D rm, int count);
void setrd0(pointR3D rd);
void setrd1(pointR3D rd);
void drawcer(int x,int y,int r,int c);
#endif // _3D_H
