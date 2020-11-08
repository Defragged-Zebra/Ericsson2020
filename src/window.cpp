//
// Created by flow on 2020. 11. 06..
//

#include "window.h"

void Window::update(){
    SDL_FillRect(screen, nullptr, SDL_MapRGB (screen->format, 0, 0, 0));
    SDL_Event event;
    createGrid(Point(grid->getHeight(),grid->getWidth()),0);
    SDL_UpdateWindowSurface(window);
    bool running=true;
    while(running){
        while (SDL_PollEvent(&event)){
            if(event.type==SDL_KEYDOWN){
                if(event.key.keysym.scancode==SDL_SCANCODE_SPACE){
                    running = false;
                    break;
                }
            }
        }
        SDL_UpdateWindowSurface(window);
    }
}

void Window::setColors() {
    size_t colorDifference=floor(16777215/(this->districtCount));
    for (size_t i=0; i<this->districtCount; i++){
        uint32_t tempcolorval=16777215-i*colorDifference;
        uint8_t r=(tempcolorval&0xFF0000u)>>16u;
        uint8_t g=(tempcolorval&0x00FF00u)>>8u;
        uint8_t b=(tempcolorval&0x0000FFu);
        SDL_Color tempcolor={r, g, b};
        this->colors.push_back(tempcolor);
    }
}


void Window::createText(const Point& p, size_t w, size_t h,size_t sep, const std::string& text){
    SDL_Color color=this->colors[this->grid->getFieldByPoint(p).getAssignedDistrictID()];
    SDL_Color col={static_cast<Uint8>((Uint8)255-color.r),static_cast<Uint8>((Uint8)255-color.g), static_cast<Uint8>((Uint8)255-color.b) };
    SDL_Surface *felirat;
    SDL_Texture *felirat_t;
    SDL_Rect hova = { (int)(p.getX()*(w+sep)+sep), (int)(p.getY()*(h+sep)+sep), static_cast<int>(w-3), static_cast<int>(h-3) };
    felirat = TTF_RenderUTF8_Blended(font, text.c_str(), col);
    felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
    SDL_RenderCopy(renderer, felirat_t, nullptr, &hova);
    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);
    //SDL_RenderPresent(renderer);
}
void Window::createText(const Point &p, size_t w, size_t h,size_t sep, int val) {
    std::string text;
    std::stringstream ss;
    ss<<val;
    ss>>text;
    SDL_Color color=this->colors[this->grid->getFieldByPoint(p).getAssignedDistrictID()];
    SDL_Color col={static_cast<Uint8>((Uint8)255-color.r),static_cast<Uint8>((Uint8)255-color.g), static_cast<Uint8>((Uint8)255-color.b) };
    SDL_Surface *felirat;
    SDL_Texture *felirat_t;
    SDL_Rect hova = { (int)(p.getX()*(w+sep)+sep), (int)(p.getY()*(h+sep)+sep), static_cast<int>(w-3), static_cast<int>(h-3) };
    felirat = TTF_RenderUTF8_Blended(font, text.c_str(), col);
    felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
    SDL_RenderCopy(renderer, felirat_t, nullptr, &hova);
    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);
    //SDL_RenderPresent(renderer);
}
void Window::createText(const Point &p, size_t w, size_t h,size_t sep, double val) {
    std::string text;
    std::stringstream ss;
    ss<<val;
    ss>>text;
    SDL_Color color=this->colors[this->grid->getFieldByPoint(p).getAssignedDistrictID()];
    SDL_Color col={static_cast<Uint8>((Uint8)255-color.r),static_cast<Uint8>((Uint8)255-color.g), static_cast<Uint8>((Uint8)255-color.b) };
    SDL_Surface *felirat;
    SDL_Texture *felirat_t;
    SDL_Rect hova = { (int)(p.getX()*(w+sep)+sep), (int)(p.getY()*(h+sep)+sep), static_cast<int>(w-3), static_cast<int>(h-3) };
    felirat = TTF_RenderUTF8_Blended(font, text.c_str(), col);
    felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
    SDL_RenderCopy(renderer, felirat_t, nullptr, &hova);
    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);
    //SDL_RenderPresent(renderer);
}


//hány sor, hány oszlop, köztük hely, négyzet mérete
void Window::createGrid(const Point& p, size_t sep, size_t sidelen){
    for (size_t y = 0; y < p.getY(); ++y) {
        for (size_t x = 0; x < p.getX(); ++x) {
            createCell(Point(y,x), sidelen, sidelen,sep);
            //createRect(Point(y * (sep + sidelen), x*(sep + sidelen)), sidelen, sidelen, red);
        }
    }
}

void Window::createRect(const Point& p,size_t w, size_t h, size_t sep){
    SDL_Color color=this->colors[this->grid->getFieldByPoint(p).getAssignedDistrictID()];
    SDL_Rect block;
    block.x=p.getX()*(w+sep)+sep;
    block.y=p.getY()*(h+sep)+sep;
    block.w=w;
    block.h=h;
    SDL_FillRect(screen, &block, SDL_MapRGB (screen->format, color.r, color.g, color.b));
}

void Window::createCell(const Point &p, size_t w, size_t h, size_t sep) {
    Field& f = grid->getFieldByPoint(p);
    Window::createRect(p, w, h,sep);
    Window::createText(Point(p.getY(),p.getX()), w, h,sep, f.getCurrentInfectionRate());
}

