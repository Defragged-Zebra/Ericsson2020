//
// Created by flow on 2020. 11. 06..
//

#include "window.h"

void Window::update(){
    SDL_FillRect(screen, nullptr, SDL_MapRGB (screen->format, 0, 0, 0));
    SDL_Event event;
    createGrid(Point(0,0),Point(grid->getHeight(),grid->getWidth()),0,20);
    SDL_UpdateWindowSurface(window);
    bool running=true;
    int y=0, x=0;
    bool pressed=false;
    while(running){
        while (SDL_PollEvent(&event)){
            if(event.type==SDL_KEYDOWN){
                if(event.key.keysym.scancode==SDL_SCANCODE_SPACE){
                    running = false;
                    break;
                }
            }
            if(event.type==SDL_MOUSEBUTTONDOWN){
                pressed=true;
            }
            if(event.type==SDL_MOUSEBUTTONUP){
                if(pressed){
                    SDL_GetMouseState(&x,&y);
                    SDL_Rect clearA({1550,0,1800-1550,1000-0});
                    SDL_FillRect(screen, &clearA, SDL_MapRGB (screen->format, 0, 0, 0));
                    showStatus(Point(0,1550),Point(std::floor((y/21.0)),std::floor(x/21.0)),100,50,10);
                    y=0;x=0;
                }
                pressed=false;

            }
            if(event.type==SDL_QUIT){
                exit(0);
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
}


//hány sor, hány oszlop, köztük hely, négyzet mérete
void Window::createGrid(const Point& windowLoc,const Point& gridSize, size_t sep, size_t sidelen){
    for (size_t y = 0; y < gridSize.getY(); ++y) {
        for (size_t x = 0; x < gridSize.getX(); ++x) {
            createDistrictCell(Point(y, x), Point(y, x), sidelen, sidelen, sep+1);
            createInfectionHeatMap(Point(y,x+gridSize.getX()*(sep+1+sidelen)+20*sep+10), Point(y,x), sidelen, sidelen, sep);
            //createVaccinationMap(Point(y,(x+gridSize.getX()*(sep+sidelen)+20*sep)*2+100), Point(y,x), sidelen, sidelen, sep);
            createMinimalVaccinationMap(Point(y,(x+gridSize.getX()*(sep+sidelen)+20*sep)*2+100), Point(y,x), sidelen, sidelen, sep);
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

void Window::createDistrictCell(const Point& windowLoc, const Point& gridElement, size_t w, size_t h, size_t sep) {
    Field& f = grid->getFieldByPoint(gridElement);
    Window::createRect(windowLoc, w, h,sep);
    Window::createText(Point(windowLoc.getY(),windowLoc.getX()), w, h,sep, f.getCurrentInfectionRate());
}


void Window::createInfectionHeatMap(const Point& windowLoc, const Point& gridElement, size_t w, size_t h, size_t sep){
    //színes négyzet létrehozása
    uint32_t tempcolorval=this->grid->getFieldByPoint(gridElement).getCurrentInfectionRate();
    uint8_t r=255;
    uint8_t g=255;
    uint8_t b=255;
    if(tempcolorval!=0){
        r=255;
        b=200-floor(tempcolorval*2);
        g=200-floor(tempcolorval*2);
    }
    SDL_Color color={r, g, b};
    SDL_Rect block;
    block.x=windowLoc.getX()+gridElement.getX()*(w+sep)+sep;
    block.y=windowLoc.getY()+gridElement.getY()*(h+sep)+sep;
    block.w=w;
    block.h=h;
    SDL_FillRect(screen, &block, SDL_MapRGB (screen->format, color.r, color.g, color.b));
    //szám létrehozása
    std::string text;
    std::stringstream ss;
    ss<<grid->getFieldByPoint(gridElement).getCurrentInfectionRate();
    ss>>text;
    SDL_Color col={0,static_cast<Uint8>((Uint8)255-color.g), static_cast<Uint8>((Uint8)255-color.b) };
    SDL_Surface *felirat;
    SDL_Texture *felirat_t;
    SDL_Rect hova = { (int)(windowLoc.getX()+gridElement.getX()*(w+sep)+sep), (int)(windowLoc.getY()+gridElement.getY()*(h+sep)+sep), static_cast<int>(w-3), static_cast<int>(h-3) };
    felirat = TTF_RenderUTF8_Blended(font, text.c_str(), col);
    felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
    SDL_RenderCopy(renderer, felirat_t, nullptr, &hova);
    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);
}

void Window::createVaccinationMap(const Point &windowLoc, const Point &gridElement, size_t w, size_t h, size_t sep) {
    //színes négyzet létrehozása
    uint32_t tempcolorval=this->grid->getFieldByPoint(gridElement).getStoredVaccines()[0];
    uint8_t r=255;
    uint8_t g=255;
    uint8_t b=255;
    if(tempcolorval!=0){
        r=200-floor(tempcolorval*2);
        g=200-floor(tempcolorval*2);
        b=255;
    }
    SDL_Color color={r, g, b};
    SDL_Rect block;
    block.x=windowLoc.getX()+gridElement.getX()*(w+sep)+sep;
    block.y=windowLoc.getY()+gridElement.getY()*(h+sep)+sep;
    block.w=w;
    block.h=h;
    SDL_FillRect(screen, &block, SDL_MapRGB (screen->format, color.r, color.g, color.b));
    //szám létrehozása
    std::string text;
    std::stringstream ss;
    ss<<grid->getFieldByPoint(gridElement).getStoredVaccines()[0];
    ss>>text;
    SDL_Color col={0,static_cast<Uint8>((Uint8)255-color.g), static_cast<Uint8>((Uint8)255-color.b) };
    SDL_Surface *felirat;
    SDL_Texture *felirat_t;
    SDL_Rect hova = { (int)(windowLoc.getX()+gridElement.getX()*(w+sep)+sep), (int)(windowLoc.getY()+gridElement.getY()*(h+sep)+sep), static_cast<int>(w-3), static_cast<int>(h-3) };
    felirat = TTF_RenderUTF8_Blended(font, text.c_str(), col);
    felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
    SDL_RenderCopy(renderer, felirat_t, nullptr, &hova);
    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);
}
void Window::createMinimalVaccinationMap(const Point &windowLoc, const Point &gridElement, size_t w, size_t h, size_t sep) {
    //színes négyzet létrehozása
    uint32_t tempcolorval=this->grid->getFieldByPoint(gridElement).vaccinesToPutMinimal(0);
    uint8_t r=255;
    uint8_t g=255;
    uint8_t b=255;
    if(tempcolorval!=0){
        r=200-floor(tempcolorval*2);
        g=200-floor(tempcolorval*2);
        b=255;
    }
    SDL_Color color={r, g, b};
    SDL_Rect block;
    block.x=windowLoc.getX()+gridElement.getX()*(w+sep)+sep;
    block.y=windowLoc.getY()+gridElement.getY()*(h+sep)+sep;
    block.w=w;
    block.h=h;
    SDL_FillRect(screen, &block, SDL_MapRGB (screen->format, color.r, color.g, color.b));
    //szám létrehozása
    std::string text;
    std::stringstream ss;
    ss<<this->grid->getFieldByPoint(gridElement).vaccinesToPutMinimal(0);
    ss>>text;
    SDL_Color col={0,static_cast<Uint8>((Uint8)255-color.g), static_cast<Uint8>((Uint8)255-color.b) };
    SDL_Surface *felirat;
    SDL_Texture *felirat_t;
    SDL_Rect hova = { (int)(windowLoc.getX()+gridElement.getX()*(w+sep)+sep), (int)(windowLoc.getY()+gridElement.getY()*(h+sep)+sep), static_cast<int>(w-3), static_cast<int>(h-3) };
    felirat = TTF_RenderUTF8_Blended(font, text.c_str(), col);
    felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
    SDL_RenderCopy(renderer, felirat_t, nullptr, &hova);
    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);
}
void Window::showStatus(const Point &windowLoc, const Point &gridElement, size_t w, size_t h, size_t sep){
    if(!gridElement.withinBounds())return;
    std::string text;
    std::stringstream ss;
    ss<<"Y: "<<gridElement.getY()<<" X: "<<gridElement.getX()<<std::endl;
    ss<<grid->getFieldByPoint(gridElement)<<std::endl;
    int lineH=0;
    while(std::getline(ss,text)){
        SDL_Surface *felirat;
        SDL_Texture *felirat_t;
        SDL_Rect hova = { (int)(windowLoc.getX()), (int)(windowLoc.getY()+lineH), static_cast<int>(w), static_cast<int>(h) };
        felirat = TTF_RenderUTF8_Blended(font, text.c_str(), SDL_Color({255,255,255}));
        felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
        SDL_RenderCopy(renderer, felirat_t, nullptr, &hova);
        SDL_FreeSurface(felirat);
        SDL_DestroyTexture(felirat_t);
        lineH+=h+sep;
    }

}

