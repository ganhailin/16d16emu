// SDLTTF.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "SDL.h"
#pragma comment(lib, "SDL.LIB")

#include "sdl_ttf.h"
#pragma comment(lib, "SDL_ttf.LIB")

int _tmain(int argc, _TCHAR* argv[])
{
	SDL_Window* pSDLWindow = NULL;  //sdl����
	SDL_Surface * pTextSurface = NULL;//�ı�����
	SDL_Texture* pTextTexture = NULL;//�ı�����
	//�ı���ʾ����
	SDL_Rect rcText;
	rcText.x = 100;
	rcText.y = 100;
	rcText.w = 100;
	rcText.h = 20;

	//��ʼ��SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) return false;
	//��������
	pSDLWindow = SDL_CreateWindow("SDLWnd", 200, 200, 640, 480, 0);
	if (NULL == pSDLWindow) return false;
	//����������Ⱦ��
	 pWndRenderer = SDL_CreateRenderer(pSDLWindow, -1, 0);


	//��ʼ�������
	if( TTF_Init() == -1 )
		return -1;
	//������
	TTF_Font *font;
	font=TTF_OpenFont("simkai.ttf", 16);
	if(!font)
	{
		printf("TTF_OpenFont: Open simsun.ttf %s\n", TTF_GetError());
		return -1;
	}
	//�����ı�����
	SDL_Color color2={255,255,255};
	pTextSurface = TTF_RenderText_Blended(font,"00:00:15",color2);
	//�����ı�����
	pTextTexture = SDL_CreateTextureFromSurface(pWndRenderer, pTextSurface);

	//��������Ⱦ��
	SDL_RenderClear(pWndRenderer);
	//���ı������Ƶ�������Ⱦ��
	SDL_RenderCopy(pWndRenderer, pTextTexture, NULL, &rcText);  // NULL means that use all texture( and renderer)
	//ˢ�´�����Ⱦ������ʾ
	SDL_RenderPresent(pWndRenderer);

	//��Ϣѭ��
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

	//����
<span style="white-space:pre">	</span>TTF_CloseFont(font);
<span style="white-space:pre">	</span>TTF_Quit();
	SDL_DestroyWindow(pSDLWindow);
	SDL_DestroyRenderer(pWndRenderer);
	SDL_DestroyTexture(pTextTexture);
	SDL_FreeSurface(pTextSurface);
	SDL_Quit();
	return 0;
}
