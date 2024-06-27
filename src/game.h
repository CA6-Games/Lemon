#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct mouses{
    int x; // x position
    int y; // y position

    int l; // left
    int r; // right
    int m; // middle
    int w; // wheel
};

struct inputs{
    struct mouses mn; // mouse now
    struct mouses mp; // mouse previous
};

struct context2ds
{
    SDL_Window* w;
    SDL_Renderer* r;
    struct inputs* i;
};

struct gstates
{
    int state; // 0 = not initialized, 1 = initialized, 2 = running, 3 = paused, 4 = quit
    void* data; // pointer to data of the game running

    int (*event)(struct context2ds* c2d, struct gstates* gs);       // event handler
    int (*update)(struct context2ds* c2d, struct gstates* gs);      // update handler
    int (*draw)(struct context2ds* c2d, struct gstates* gs);        // draw handler
};

// ------------- Menus -------------

struct menusItems
{
    char* name; // displayed text
    char* path; // icon path
    void* icon; // icon texture
    int(*callback)(struct context2ds* c2d, struct gstates* gs, int value, void* data);
    int value; // value to pass to the callback
    void* data; // data to pass to the callback

    int selected;
    int enabled;
};

// ----- nav Bar -----

struct menu_navBars
{
    struct menusItems* mi;
    int nbItems;

    void* bgItem;
    void* bgItemS;
    void* bgItemD;

    int x;
    int y;
    int w; // width should be the width of the window
    int h;
};
int menu_navBar_update(struct context2ds* c2d, struct gstates* gs, struct menu_navBars* nb);
int menu_navBar_draw(struct context2ds* c2d, struct gstates* gs, struct menu_navBars* nb);


// ----- grid menu -----

struct menu_grids{
    struct menusItems* mi;
    int nbItems;

    void* bg;
    void* bgItem;
    void* bgItemS;
    void* bgItemD;
    void* btnPrev;
    void* btnNext;

    int x;
    int y;
    int w;
    int h;

    int nbCols;
    int nbRows;

    int page;
};
int menu_grid_update(struct context2ds* c2d, struct gstates* gs, struct menu_grids* g);
int menu_grid_draw(struct context2ds* c2d, struct gstates* gs, struct menu_grids* g);

// ----- text

int ui_ttfInit(struct context2ds* c2d, char* fontPath, int fontSize);
int ui_ttfWrite(struct context2ds* c2d, char* text, int x, int y, int r, int g, int b, void* img);