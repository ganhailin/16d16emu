#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "draw.h"
#include "touch.h"
#include "3D.h"
SDL_Surface *screen;
SDL_Window* window = NULL;
int xrad,yrad,xradf,yradf;
Uint32 videoflags;
uint32_t key=0;
bool candraw=true;
pointR3D rd3;
bool isfirst=true,isfullscreen=false;
char  sharebuff[100];
TTF_Font* displayfont;
void setdisplay(int x,int y)
{
    xrad=x;
    yrad=y;

}


int setfullscreen(void)
{

    if(isfullscreen)
    {
        candraw=false;
        SDL_SetWindowFullscreen(window,0);
        screen = SDL_GetWindowSurface( window );
        SDL_GetWindowSize(window,&xrad,&yrad);
        candraw=true;
        isfullscreen=false;
    }
    else
    {
        candraw=false;
        SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN_DESKTOP);
        screen = SDL_GetWindowSurface( window );
        SDL_GetWindowSize(window,&xrad,&yrad);
        candraw=true;
        isfullscreen=true;
    }
    return 0;
}


void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    if(!candraw)return;
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp)
    {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
        {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        }
        else
        {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

void Draw_Pixel(int x, int y, Uint32 c)
{

    putpixel(screen, x, y, c);
}



void initSDL(void)
{
    if ( SDL_Init(SDL_INIT_EVERYTHING) < 0||TTF_Init()<0 )
    {
        exit(1);
    }
    else
    {
        displayfont = TTF_OpenFont("sketchy.ttf",64);
        window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, xrad, yrad,SDL_WINDOW_SHOWN );
        if( window == NULL )
        {
            exit(1);
        }
        else
        {
            screen = SDL_GetWindowSurface( window );
            SDL_FillRect( screen, NULL, SDL_MapRGB( screen->format, 0, 0, 0 ) );
            //SDL_Delay( 2000 );
        }
    }
    SDL_WarpMouseInWindow(window,xrad/2,yrad/2);
    //setfullscreen();
    Gan_touch_init(window);
    atexit(exitSDL);
}


void clrscreen(void)
{
    SDL_Rect r = {0, 0,(Uint16)xrad,(Uint16)yrad};
    SDL_FillRect(screen, &r, 0);

}


void updatescreen(void )
{
/*
    SDL_Color color={255,255,255};
    SDL_Surface *text_surface;
if(!(text_surface=TTF_RenderUTF8_Solid(displayfont,"Hello World!",color))) {
    //handle error here, perhaps print TTF_GetError at least
} else {
    SDL_BlitSurface(text_surface,NULL,screen,NULL);
    //perhaps we can reuse it, but I assume not for simplicity.
    SDL_FreeSurface(text_surface);
}

*/
    SDL_UpdateWindowSurface( window );

}


void exitSDL(void)
{
    SDL_FreeSurface(screen);
    SDL_Quit();
}

void SDLloop(bool * drawdone,bool * done)
{
    while (!*done || !*drawdone)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                *done = true;
                break;
            /*  case SDL_VIDEORESIZE:
                  setdisplay(event.resize.w,event.resize.h);
                  candraw=false;
                  screen = SDL_SetVideoMode(event.resize.w,event.resize.h, 32, videoflags);
                  candraw=true;
                  break;/*/
            case SDL_FINGERDOWN:
                touchdown(event.tfinger);
                if(getfingernum()==4)
                    *done=true;
                break;
            case SDL_FINGERUP:
                touchup(event.tfinger);
                if(getfingernum()==2)
                    setfullscreen();
                break;
            case SDL_FINGERMOTION:
                touchmo(event.tfinger);
                break;

            /*  case SDL_FINGERMOTION:
                  movecamR(event.tfinger.dy*90,event.tfinger.dx*90,0);
                  fingery=event.tfinger.y*yrad;
                  fingerx=event.tfinger.x*xrad;
                  sprintf(sharebuff,"%d",fingerx);
                  drawcer(fingerx,fingery,20,0xffffff);
                  break;
              case SDL_MULTIGESTURE:
                  if(event.mgesture.numFingers==4)
                      *done = true;
                  //      if(event.mgesture.numFingers==3)
                  //          setfullscreen();
                  //        usleep(1000000);
                  break;
               case SDL_FINGERMOTION:

                 //  sprintf(sharebuff,"num:%d,x:%f,y:%f,ID:%d",event.mgesture.type,event.mgesture.dDist,event.mgesture.dTheta,event.mgesture.touchId);
                   sprintf(sharebuff,"num:%d",event.tfinger.fingerId);
                   break;
              */
#ifndef DISABLE_MOUSE
               case SDL_MOUSEMOTION:
                   int x,y;
                   SDL_GetMouseState(&x,&y);
                   movecamR(-D2R(y-yrad/2)*3,-D2R(x-xrad/2)*3,0);
                   SDL_WarpMouseInWindow(window,xrad/2,yrad/2);
#endif // DISABLE_MOUSE
                break;

            case SDL_KEYDOWN :
                switch(event.key.keysym.sym)
                {
                case SDLK_w:
                    key|=0x01;
                    break;
                case SDLK_s:
                    key|=0x02;
                    break;
                case SDLK_a:
                    key|=0x04;
                    break;
                case SDLK_d:
                    key|=0x08;
                    break;
                case SDLK_LSHIFT:
                case SDLK_RSHIFT:
                    key|=0x10;
                    break;
                case SDLK_SPACE:
                    key|=0x20;
                    break;
                case SDLK_q:
                case SDLK_ESCAPE:
                    *done = true;
                    break;
                case SDLK_F10:
                    setfullscreen();
                    break;

                default:
                    break;
                }
                break;
            case SDL_KEYUP :
                switch(event.key.keysym.sym)
                {
                case SDLK_w:
                    key&=0xfe;
                    break;
                case SDLK_s:
                    key&=0xfd;
                    break;
                case SDLK_a:
                    key&=0xfb;
                    break;
                case SDLK_d:
                    key&=0xf7;
                    break;
                case SDLK_f:
                    key|=0x0100;
                    break;
                case SDLK_h:
                    key|=0x0200;
                    break;
                case SDLK_t:
                    key|=0x0400;
                    break;
                case SDLK_g:
                    key|=0x0800;
                    break;
                case SDLK_r:
                    key|=0x1000;
                    break;
                case SDLK_u:
                    key|=0x2000;
                    break;
                case SDLK_j:
                    key|=0x4000;
                    break;
                case SDLK_i:
                    key|=0x8000;
                    break;
                case SDLK_k:
                    key|=0x10000;
                    break;
                case SDLK_o:
                    key|=0x20000;
                    break;
                case SDLK_l:
                    key|=0x40000;
                    break;
                case SDLK_v:
                    key|=0x80000;
                    break;
                case SDLK_b:
                    key|=0x100000;
                    break;

                case SDLK_LSHIFT:
                case SDLK_RSHIFT:
                    key&=0xef;
                    break;
                case SDLK_SPACE:
                    key&=0xdf;
                    break;
                default:
                    break;
                }
                break;
            }
        }
    }

}

uint32_t *getkey(void)
{
    return &key;
}
