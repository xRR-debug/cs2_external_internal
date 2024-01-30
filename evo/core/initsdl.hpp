#include "../../SDL3/include/SDL3/SDL.h"
#pragma comment(lib, "SDL3.lib")

inline static SDL_Surface* sdl_surface;
inline static SDL_DisplayMode* sdl_display;

namespace initsdl
{
		inline bool init_sdl()
		{
			if (SDL_Init(SDL_INIT_VIDEO) < 0)
			{
				return 0;
			}

			SDL_Window* window = SDL_CreateWindow(X("angeldancy"), 640, 480, SDL_WINDOW_TRANSPARENT | SDL_WINDOW_FULLSCREEN);
			if (window == NULL)
			{
				return 0;
			}

			SDL_DisplayID id = SDL_GetDisplayForWindow(window);
			const SDL_DisplayMode* disp = SDL_GetCurrentDisplayMode(id);
			SDL_SetWindowSize(window, disp->w, disp->h);
			SDL_SetWindowPosition(window, 0, 0);
			SDL_SetWindowAlwaysOnTop(window, SDL_TRUE);
			SDL_SetWindowOpacity(window, 0.20f);
			sdl_surface = SDL_GetWindowSurface(window);

			bool quit = 0;
			while (!quit)
			{
				SDL_Event e;
				while (SDL_PollEvent(&e))
				{
					if (e.type == SDL_EVENT_QUIT)
					{
						quit = 1;
						break;
					}
				}
				SDL_UpdateWindowSurface(window);
				SDL_FillSurfaceRect(sdl_surface, NULL, 0x000000);
			}
			SDL_DestroyWindow(window);
			SDL_Quit();
			return 0;
		}
}