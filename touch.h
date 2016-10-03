#define MAX_POINT 10
#define MAX_ITEM 20
#define MAXLARY 10
#define OFFSET 0.01f


typedef struct
{
    float x,y,rx,ry,presure;
    int id,is,time,itemid;
} touchpoint;

typedef struct
{
    int num;
    touchpoint tpoint[MAX_POINT];
} touch_t;

typedef struct
{
    int type,status,lary;
    float x,y,sizex,sizey;
    touchpoint *tpoint;
    void* press();
    void* longpress();
    void* moves();
} item_t;





int getfingernum(void);
int touchdown(SDL_TouchFingerEvent finger);
int touchup(SDL_TouchFingerEvent finger);
int touchmo(SDL_TouchFingerEvent finger);
int Gan_touch_init(SDL_Window * window);
int marchpoint(int p);
int unmarchpoint (int p);
int statusupdata(int p);
