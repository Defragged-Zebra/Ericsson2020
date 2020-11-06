//
// Created by flow on 2020. 11. 06..
//

#include "draw.h"

void Draw::run(){
    //ablak addig nyitva, amíg ki nem ixeled
    SDL_Event event;
    bool running = true;

    while (running){
        while (SDL_PollEvent(&event)){
            if(event.type==SDL_QUIT){
                running = false;
                break;
            }
            SDL_UpdateWindowSurface(window);
        }
    }
    TTF_CloseFont(font);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Draw::createNumbers(SDL_Color color, int x, int y, int w, int h, const char* text){
    SDL_Surface *felirat;
    SDL_Texture *felirat_t;
    SDL_Rect hova = { x, y, w, h };
    felirat = TTF_RenderUTF8_Blended(font, text, color);
    felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
    hova.x = x;
    hova.y = y;
    hova.w = w;
    hova.h = h;
    SDL_RenderCopy(renderer, felirat_t, nullptr, &hova);
    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);
    SDL_RenderPresent(renderer);
}

//hány sor, hány oszlop, köztük hely, négyzet mérete
void Draw::createGrid(size_t sy,size_t sx,size_t sep=10,size_t sidelen=20){
    Uint32 red = SDL_MapRGB (screen->format, 255, 0, 0);
    for (size_t y = 1; y < sy+1; ++y) {
        for (size_t x = 1; x < sx+1; ++x) {
            createRect(x*(sep + sidelen), y * (sep + sidelen), sidelen, sidelen, red);
        }
    }
}

void Draw::createRect(size_t x, size_t y, size_t w, size_t h, SDL_Color color){
    SDL_Rect block;
    block.x=x;
    block.y=y;
    block.w=w;
    block.h=h;
    SDL_FillRect(screen, &block, color);
}