#include "utils.h"


int utils2d_pointInRect(int x, int y, int w, int h, int x2, int y2)
{
    if(x > x2 && x < x2 + w && y > y2 && y < y2 + h)
    {
        return 1;
    }
    return 0;
}

int utils2d_clickInRect(struct context2ds* c2d, int w, int h, int x, int y)
{
    if(c2d->i->mp.l == 0 && c2d->i->mn.l == 1)
    {
        if(utils2d_pointInRect(c2d->i->mp.x, c2d->i->mp.y, w, h, x, y))
        {
            return 1;
        }
    }
    return 0;
}

int utils2d_holdInRect(struct context2ds* c2d, int w, int h, int x, int y)
{
    if(c2d->i->mp.l == 1 && c2d->i->mn.l == 1)
    {
        if(utils2d_pointInRect(c2d->i->mp.x, c2d->i->mp.y, w, h, x, y))
        {
            return 1;
        }
    }
    return 0;
}

int utils2d_releaseInRect(struct context2ds* c2d, int w, int h, int x, int y)
{
    if(c2d->i->mp.l == 1 && c2d->i->mn.l == 0)
    {
        if(utils2d_pointInRect(c2d->i->mp.x, c2d->i->mp.y, w, h, x, y))
        {
            return 1;
        }
    }
    return 0;
}

int utils2d_loadTexture(struct context2ds* c2d, char* path, void** texture)
{
    SDL_Surface* s = IMG_Load(path);
    if(s == NULL)
    {
        printf("Error loading image: %s\n", IMG_GetError());
        return 1;
    }

    *texture = SDL_CreateTextureFromSurface(c2d->r, s);
    if(*texture == NULL)
    {
        printf("Error creating texture: %s\n", SDL_GetError());
        return 1;
    }

    SDL_FreeSurface(s);

    return 0;
}