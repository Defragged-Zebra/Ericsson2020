//
// Created by flow on 2020. 11. 06..
//

#ifndef VIRUS_WINDOW_H
#define VIRUS_WINDOW_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL_ttf.h>
#include <SDL.h>
#include <SDL2/SDL_mouse.h>
#include <vector>

#include "grid.h"
#include "utils.h"

class Window {
    SDL_Window *window;
    SDL_Surface *screen;
    SDL_Renderer *renderer;
    TTF_Font *font;
    size_t districtCount{};
    Grid *grid = nullptr;
    SDL_Color red = {255, 0, 0};
    SDL_Color black = {0, 0, 0};
    SDL_Color green = {0, 255, 0};
    SDL_Color blue = {0, 0, 255};
    std::vector<SDL_Color> colors;
    size_t mode = 0;

public:
    Window &operator=(const Window &) = delete;

    Window(const Window &) = delete;

    explicit Window(size_t w = 1800, size_t h = 1000) {
        //itt van-e a cucli
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
            throw std::runtime_error("Failed to initialize the SDL2 library");

        //ablak, 680x480 méretű, középen van a képernyőn?
        this->window = SDL_CreateWindow("SDL2 Window", 100, 100, w, h, 0);

        //létrehozta-e
        if (!window)
            throw std::runtime_error("Failed to create window");

        this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
        if (!renderer) {
            throw std::runtime_error("Szar az egész ahogy van");
        }


        //ablak neve
        SDL_SetWindowTitle(window, "Ericsson bajnoksag visualization - made by: DefraggedZebra");

        //felület létrehozása, ezt tudjuk animálni ha jól értem, az ablak basically csak a keret
        this->screen = SDL_GetWindowSurface(window);

        SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format, 0, 0, 0));

        TTF_Init();

        this->font = TTF_OpenFont("../src/fonts/LiberationSerif-Regular.ttf", 32);
        if (!font) {
            throw std::runtime_error("Failed to open font");
        }


    }

    void update();

    void setGrid(Grid *g) {
        this->grid = g;
        this->districtCount = g->getNumberOfDistricts();
        setColors();
    }

    void setColors();

    void createText(const Point &p, size_t w, size_t h, size_t sep, const std::string &text);

    void createText(const Point &p, size_t w, size_t h, size_t sep, int val);

    void createText(const Point &p, size_t w, size_t h, size_t sep, double val);

    //hány sor, hány oszlop, köztük hely, négyzet mérete
    void createGrid(const Point &windowLoc, const Point &gridSize, size_t sep = 10, size_t sidelen = 30);

    void createRect(const Point &p, size_t w, size_t h, size_t sep);

    void createDistrictCell(const Point &windowLoc, const Point &gridElement, size_t w, size_t h, size_t sep);

    void createInfectionHeatMap(const Point &windowLoc, const Point &gridElement, size_t w, size_t h, size_t sep);

    void createVaccinationMap(const Point &windowLoc, const Point &gridElement, size_t w, size_t h, size_t sep);

    ~Window() {
        TTF_CloseFont(font);
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
    }

    void createMinimalVaccinationMap(const Point &windowLoc, const Point &gridElement, size_t w, size_t h, size_t sep);

    void showStatus(const Point &windowLoc, const Point &gridElement, size_t w = 100, size_t h = 900, size_t sep = 5);
};


#endif //VIRUS_WINDOW_H
