// sudo dnf install SDL2 SDL2-devel SDL2_image SDL2_image-devel SDL2_ttf SDL2_ttf-devel SDL2_mixer SDL2_mixer-devel SDL2_net SDL2_net-devel

#include "game.h"
#include "utils.h"
#include "lemon.h"

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

int init_windowRender(struct context2ds* c2d);
int game_init(struct context2ds* c2d, struct gstates* gs);
void game_loop(void);

struct context2ds* c2d;
struct gstates* gs;

int main(int argc, char *argv[])
{
    int quit = 0;

    c2d = (struct context2ds*)malloc(sizeof(struct context2ds));
    gs = (struct gstates*)malloc(sizeof(struct gstates));

    if(argc > 1)
    {
        if(strstr(argv[1], "debug")){
            printf("Debug mode\n");
        }
    }

    // initialize the graphic rendering
    if(init_windowRender(c2d) != 0)
    {
        return -1;
    }

    game_init(c2d, gs);

    #ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(game_loop, 0, 1);
        emscripten_set_main_loop_timing(EM_TIMING_SETTIMEOUT, 1000 / 60);
    #else
        while(!quit)
        {
            SDL_Delay(1000/60);
            game_loop();
            quit = gs->state == 4;
        }
    #endif

    return 0;
}

int init_windowRender(struct context2ds* c2d)
{
    if(c2d == NULL)
    {
        return -1;
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -2;
    }

    c2d->w = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, 0);
    if (c2d->w == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -3;
    }

    c2d->r = SDL_CreateRenderer(c2d->w, -1, SDL_RENDERER_ACCELERATED);
    if (c2d->r == NULL)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return -4;
    }

    //  alpha blend
    SDL_SetRenderDrawBlendMode(c2d->r, SDL_BLENDMODE_BLEND);

    c2d->i = (struct inputs*)malloc(sizeof(struct inputs));
    if(c2d->i == NULL)
    {
        return -5;
    }
    memset(c2d->i, 0, sizeof(struct inputs));
    
    return 0;
}

int game_init(struct context2ds* c2d, struct gstates* gs)
{
    if(c2d == NULL || gs == NULL)
    {
        return -1;
    }

    gs->state = 1;
    gs->data = NULL;
    gs->event = NULL;
    gs->update = NULL;
    gs->draw = NULL;

    ui_ttfInit(c2d, "res/ui/font.otf", 24);

    return lemon_init(c2d, gs);
}

int game_event(struct context2ds* c2d, struct gstates* gs)
{
    // copy the mouse now to the mouse previous
    memcpy(&c2d->i->mp, &c2d->i->mn, sizeof(struct mouses));

    //read mouse event
    SDL_GetMouseState(&c2d->i->mn.x, &c2d->i->mn.y);
    c2d->i->mn.l = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT);
    c2d->i->mn.r = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT);
    c2d->i->mn.m = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE);
    c2d->i->mn.w = 0;
    
    SDL_Event event;

    // poll for events
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            gs->state = 4;
        }
    }

    return 0;
}

void game_loop(void)
{
    int quit = 0;

    // event
    game_event(c2d, gs);
    if(gs->event != NULL)
    {
        gs->event(c2d, gs);
    }

    // clear the screen
    SDL_SetRenderDrawColor(c2d->r, 0, 0, 0, 255);
    SDL_RenderClear(c2d->r);

    // update
    if(gs->update != NULL)
    {
        gs->update(c2d, gs);
    }

    // clear the screen
    SDL_SetRenderDrawColor(c2d->r, 255, 255, 255, 255);
    SDL_RenderClear(c2d->r);

    // draw
    if(gs->draw != NULL)
    {
        gs->draw(c2d, gs);
    }

    // render 
    SDL_RenderPresent(c2d->r);

    if(gs->state == 4)
    {
        quit = 1;
    }
}

struct versions{
    uint8_t major;
    uint8_t minor;
    uint8_t patch;
    uint8_t type; // 0 = dev, 1 = alpha, 2 = beta, 3 = release
};

struct save
{
    uint32_t time;
    uint32_t size;
    struct versions v;
    void* data;
};


// navbar

int menu_navBar_update(struct context2ds* c2d, struct gstates* gs, struct menu_navBars* nb)
{
    SDL_Rect rect = {nb->x, nb->y, nb->w, nb->h};
    int clk = -1;

    if(!utils2d_clickInRect(c2d, rect.w, rect.h, rect.x, rect.y))
    {
        return 0;
    }

    int itemWidth = rect.w / nb->nbItems;
    int lastItemWidth = rect.w - (nb->nbItems - 1) * itemWidth;
    for(int i = 0; i < nb->nbItems; i++)
    {
        rect.x = i * itemWidth;
        if(i == nb->nbItems - 1)
        {
            rect.w = lastItemWidth;
        }
        else
        {
            rect.w = itemWidth;
        }
        if(utils2d_clickInRect(c2d, rect.w, rect.h, rect.x, rect.y) && nb->mi[i].enabled)
        {
            clk = i;
        }
    }

    if(clk != -1)
    {
        for(int i = 0; i < nb->nbItems; i++)
        {
            nb->mi[i].selected = 0;
        }
        nb->mi[clk].selected = 1;
        nb->mi[clk].callback(c2d, gs, nb->mi[clk].value, nb->mi[clk].data);
    }

    return 0;
}

int menu_navBar_draw(struct context2ds* c2d, struct gstates* gs, struct menu_navBars* nb)
{
    SDL_Rect rect = {nb->x, nb->y, nb->w, nb->h};

    SDL_SetRenderDrawColor(c2d->r, 0, 255, 0, 255);
    SDL_RenderFillRect(c2d->r, &rect);

    int itemWidth = rect.w / nb->nbItems;
    int lastItemWidth = rect.w - (nb->nbItems - 1) * itemWidth;
    for(int i = 0; i < nb->nbItems; i++)
    {
        rect.x = i * itemWidth;
        
        // if alst item, take the remaining space
        if(i == nb->nbItems - 1)
        {
            rect.w = lastItemWidth;
        }
        else
        {
            rect.w = itemWidth;
        }

        if(nb->bgItem != NULL)
        {
            if(nb->mi[i].selected)
            {
                SDL_RenderCopy(c2d->r, nb->bgItemS, NULL, &rect);
            } 
            else if(!nb->mi[i].enabled)
            {
                SDL_RenderCopy(c2d->r, nb->bgItemD, NULL, &rect);
            }
            else
            {
                SDL_RenderCopy(c2d->r, nb->bgItem, NULL, &rect);
            }
        }
        else
        {
            SDL_SetRenderDrawColor(c2d->r, 0, 0, 0, 255);
            SDL_RenderDrawRect(c2d->r, &rect);
        }

        if(nb->mi[i].icon != NULL)
        {
            SDL_RenderCopy(c2d->r, nb->mi[i].icon, NULL, &rect);
        }
    }

    return 0;
}

// grid menu

int menu_grid_update(struct context2ds* c2d, struct gstates* gs, struct menu_grids* g)
{
    SDL_Rect rect = {g->x, g->y, g->w, g->h};

    if(!utils2d_clickInRect(c2d, rect.w, rect.h, rect.x, rect.y))
    {
        return 0;
    }

    int itemWidth = rect.w / g->nbCols;
    int itemHeight = (rect.h-50) / g->nbRows;

    int maxItems = g->nbCols * g->nbRows;
    int maxPages = g->nbItems / maxItems;

    int start = g->page * maxItems;
    int end = start + maxItems;
    if(end > g->nbItems)
    {
        end = g->nbItems;
    }

    for(int i = 0; i < end-start; i++)
    {
        rect.x = (i % g->nbCols) * itemWidth + g->x;
        rect.y = (i / g->nbCols) * itemHeight + g->y;
        rect.w = itemWidth;
        rect.h = itemHeight;

        if(utils2d_clickInRect(c2d, rect.w, rect.h, rect.x, rect.y) && g->mi[i+start].enabled)
        {
            g->mi[i].callback(c2d, gs, g->mi[i+start].value, g->mi[i+start].data);
            printf("Grid item clicked, value: %d\n", g->mi[i+start].value);
            g->mi[i].selected = 1;
        } else {
            g->mi[i].selected = 0;
        }
    }

    // check the prev next buttons
    rect.x = g->x;
    rect.y = g->y + g->h - 50;
    rect.w = g->w / 2;
    rect.h = 50;
    if(utils2d_clickInRect(c2d, rect.w, rect.h, rect.x, rect.y) && g->page > 0)
    {
        printf("Prev button clicked %d\n", g->page);
        g->page--;
    }

    rect.x = g->x + g->w / 2;
    rect.y = g->y + g->h - 50;
    rect.w = g->w / 2;
    rect.h = 50;
    if(utils2d_clickInRect(c2d, rect.w, rect.h, rect.x, rect.y) && g->page < maxPages)
    {
        printf("Next button clicked %d\n", g->page);
        g->page++;
    }


    return 0;
}

int menu_grid_draw(struct context2ds* c2d, struct gstates* gs, struct menu_grids* g)
{
    SDL_Rect rect = {g->x, g->y, g->w, g->h};

    int itemWidth = rect.w / g->nbCols;
    int itemHeight = (rect.h-50) / g->nbRows;

    int maxItems = g->nbCols * g->nbRows;

    int start = g->page * maxItems;
    int end = start + maxItems;
    if(end > g->nbItems)
    {
        end = g->nbItems;
    }

    for(int i = 0; i < end-start; i++)
    {
        rect.x = (i % g->nbCols) * itemWidth + g->x;
        rect.y = (i / g->nbCols) * itemHeight + g->y;
        rect.w = itemWidth;
        rect.h = itemHeight;

        if(g->bgItem != NULL)
        {
            if(g->mi[i+start].selected)
            {
                SDL_RenderCopy(c2d->r, g->bgItem, NULL, &rect);
            }
             else if(!g->mi[i+start].enabled)
            {
                SDL_RenderCopy(c2d->r, g->bgItemD, NULL, &rect);
            } 
            else
            {
                SDL_RenderCopy(c2d->r, g->bgItem, NULL, &rect);
            }
        }
        else
        {
            SDL_SetRenderDrawColor(c2d->r, 0, 0, 0, 255);
            SDL_RenderDrawRect(c2d->r, &rect);
        }

        rect.x += 10;
        rect.y += 10;
        rect.w -= 20;
        rect.h -= 20;

        if(g->mi[i+start].icon != NULL)
        {
            SDL_RenderCopy(c2d->r, g->mi[i+start].icon, NULL, &rect);
        } 
    }

    // draw the prev buttons
    rect.x = g->x;
    rect.y = g->y + g->h - 50;
    rect.w = g->w / 4;
    rect.h = 50;
    if(g->btnPrev != NULL)
    {
        SDL_RenderCopy(c2d->r, g->btnPrev, NULL, &rect);
    }
    else
    {
        SDL_SetRenderDrawColor(c2d->r, 0, 0, 0, 255);
        SDL_RenderDrawRect(c2d->r, &rect);
    }

    // draw the next button
    rect.x = g->x + (3*(g->w / 4));
    rect.y = g->y + g->h - 50;
    rect.w = g->w / 4;
    rect.h = 50;
    if(g->btnNext != NULL)
    {
        SDL_RenderCopy(c2d->r, g->btnNext, NULL, &rect);
    }
    else
    {
        SDL_SetRenderDrawColor(c2d->r, 0, 0, 0, 255);
        SDL_RenderDrawRect(c2d->r, &rect);
    }

    return 0;
}

TTF_Font* font = NULL;
int ui_ttfInit(struct context2ds* c2d, char* fontPath, int fontSize)
{
    //  init ttf
    if(TTF_Init() == -1)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        return 1;
    }

    font = TTF_OpenFont(fontPath, fontSize);
    if(font == NULL)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        return 1;
    }

    return 0;

}

int ui_ttfWrite(struct context2ds* c2d, char* text, int x, int y, int r, int g, int b, void* img)
{
    SDL_Color color = {r, g, b};
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    if(surface == NULL)
    {
        printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(c2d->r, surface);
    if(texture == NULL)
    {
        printf("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
        return 1;
    }

    //  aling center text
    SDL_Rect rect = {x, y, 0, 0};
    SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
    rect.x = x - rect.w / 2;
    rect.y = y - rect.h / 2;

    if(img != NULL)
    {
        SDL_Rect rect2 = {rect.x-15, rect.y-15, rect.w+30, rect.h+30};
        SDL_RenderCopy(c2d->r, img, NULL, &rect2);
    }

    // draw a rectangle behind the text
    // SDL_SetRenderDrawColor(c2d->r, 0, 0, 0, 64);
    // SDL_RenderFillRect(c2d->r, &rect);

    SDL_RenderCopy(c2d->r, texture, NULL, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    return 0;
}