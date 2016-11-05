// SDLTTF.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "SDL.h"
#pragma comment(lib, "SDL.LIB")

#include "sdl_ttf.h"
#pragma comment(lib, "SDL_ttf.LIB")

int _tmain(int argc, _TCHAR* argv[])
{
	SDL_Window* pSDLWindow = NULL;  //sdl窗口
	SDL_Surface * pTextSurface = NULL;//文本表面
	SDL_Texture* pTextTexture = NULL;//文本纹理
	//文本显示区域
	SDL_Rect rcText;
	rcText.x = 100;
	rcText.y = 100;
	rcText.w = 100;
	rcText.h = 20;

	//初始化SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) return false;
	//创建窗口
	pSDLWindow = SDL_CreateWindow("SDLWnd", 200, 200, 640, 480, 0);
	if (NULL == pSDLWindow) return false;
	//创建窗口渲染器
	 pWndRenderer = SDL_CreateRenderer(pSDLWindow, -1, 0);


	//初始化字体库
	if( TTF_Init() == -1 )
		return -1;
	//打开字体
	TTF_Font *font;
	font=TTF_OpenFont("simkai.ttf", 16);
	if(!font)
	{
		printf("TTF_OpenFont: Open simsun.ttf %s\n", TTF_GetError());
		return -1;
	}
	//创建文本表面
	SDL_Color color2={255,255,255};
	pTextSurface = TTF_RenderText_Blended(font,"00:00:15",color2);
	//创建文本纹理
	pTextTexture = SDL_CreateTextureFromSurface(pWndRenderer, pTextSurface);

	//清理窗口渲染器
	SDL_RenderClear(pWndRenderer);
	//将文本纹理复制到窗口渲染器
	SDL_RenderCopy(pWndRenderer, pTextTexture, NULL, &rcText);  // NULL means that use all texture( and renderer)
	//刷新窗口渲染器的显示
	SDL_RenderPresent(pWndRenderer);

	//消息循环
	bool bQuit = false;
	while(!bQuit)
	{
		SDL_Event sdl_event;
		if (SDL_PollEvent(&sdl_event))
		{
			switch(sdl_event.type)
			{
			case SDL_QUIT:
				bQuit = true;
				break;
			default:
				break;
			}
		}
	}

	//清理
<span style="white-space:pre">	</span>TTF_CloseFont(font);
<span style="white-space:pre">	</span>TTF_Quit();
	SDL_DestroyWindow(pSDLWindow);
	SDL_DestroyRenderer(pWndRenderer);
	SDL_DestroyTexture(pTextTexture);
	SDL_FreeSurface(pTextSurface);
	SDL_Quit();
	return 0;
}
