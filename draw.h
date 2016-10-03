#ifndef DRAW_H_INCLUDED
#define DRAW_H_INCLUDED
/*
//#define _FULLCSREAN
#ifdef _FULLCSREAN
#define xrad 1024
#define yrad 768
#else
#define xrad 800
#define yrad 480
#endif // _FULLCSREAN
*/
extern int xrad,yrad;
void setdisplay(int x,int y);
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
void Draw_Pixel(int x, int y, Uint32 pixel);
void initSDL(void);
void clrscreen(void);
void updatescreen(void);
void exitSDL(void);
void SDLloop(bool*,bool*);
uint32_t* getkey(void);

#endif // DRAW_H_INCLUDED
