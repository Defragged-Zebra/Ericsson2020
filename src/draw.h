//
// Created by flow on 2020. 11. 06..
//

#ifndef VIRUS_DRAW_H
#define VIRUS_DRAW_H
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL_ttf.h>

class Draw {
    SDL_Window* window;
    SDL_Surface* screen;
    SDL_Renderer* renderer;
    TTF_Font* font;

public:
    explicit Draw(size_t w=640, size_t h=480){
        //itt van-e a cucli
        if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
            throw std::runtime_error( "Failed to initialize the SDL2 library");

        //ablak, 680x480 méretű, középen van a képernyőn?
        this->window = SDL_CreateWindow("SDL2 Window",
                                        100,
                                        100,
                                        w, h,
                                        0);

        //létrehozta-e
        if(!window)
            throw std::runtime_error("Failed to create window");

        this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
        if (!renderer) {
            throw std::runtime_error("Szar az egész ahogy van");
        }


        //ablak neve
        SDL_SetWindowTitle(window, "FlyingDildo");

        //felület létrehozása, ezt tudjuk animálni ha jól értem, az ablak basically csak a keret
        this->screen = SDL_GetWindowSurface(window);

        SDL_FillRect(screen, nullptr, SDL_MapRGB (screen->format, 255, 255, 255));

        TTF_Init();

        this->font = TTF_OpenFont("../src/fonts/LiberationSerif-Regular.ttf", 32);
        if (!font) {
            throw std::runtime_error("Failed to open font");
        }



    }
    void run();

    void createNumbers(SDL_Color color, int x, int y, int w, int h, const char* text);

    //hány sor, hány oszlop, köztük hely, négyzet mérete
    void createGrid(size_t sy,size_t sx,size_t sep=10,size_t sidelen=20);

    void createRect(size_t x, size_t y, size_t w, size_t h, SDL_Color color);

};


#endif //VIRUS_DRAW_H
