#include "Game.h"
#include <SDL.h>
#include <SDL_ttf.h>

int main() {
	Game game(1000);
	game.menu();

    /*bool quit = false;
    SDL_Event event;
    int textW = 0;
    int textH = 0;

    //SDL_Init(SDL_INIT_VIDEO);
    //TTF_Init();

    //SDL_Window* window = SDL_CreateWindow("SDL_ttf in SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    //SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    /*TTF_Font* font = TTF_OpenFont("Fonts/NewTegomin-Regular.ttf", 25);

    SDL_Color color = { 255, 255, 255 };
    SDL_Surface* surface = TTF_RenderText_Solid(font, "Henlo!", color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    
    SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
    SDL_Rect textRect = { 0, 0, textW, textH };

    SDL_RenderCopy(
        renderer,
        texture,
        NULL,
        &textRect
    );
    SDL_RenderPresent(renderer);

    /*while (!quit) {
        SDL_WaitEvent(&event);

        switch (event.type) {
        case SDL_QUIT:
            quit = true;
            break;
        }
    }*/

    /*SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    //SDL_DestroyRenderer(renderer);
    //SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    //TTF_Quit();
    //SDL_Quit();*/


	return 0;
}