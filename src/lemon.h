#pragma once

#include "game.h"
#include "utils.h"

struct lemons{
    int x; // x position
    int y; // y position
    int h; // height
    int w; // width

    int peel;
    int zest;
    int bg;
};

struct lemon_data
{
    int menu; // 0 = main, 1 = lemon, 2 = skin, 3 = zest

    int nbClick; // number of click on the lemon

    struct lemons l; // the lemon
};

int lemon_init(struct context2ds* c2d, struct gstates* gs);
int lemon_event(struct context2ds* c2d, struct gstates* gs);
int lemon_update(struct context2ds* c2d, struct gstates* gs);
int lemon_draw(struct context2ds* c2d, struct gstates* gs);

int lemon_mainMenu_draw(struct context2ds* c2d, struct gstates* gs);
int lemon_mainMenu_update(struct context2ds* c2d, struct gstates* gs);
int lemon_skinMenu_update(struct context2ds* c2d, struct gstates* gs);
int lemon_skinMenu_draw(struct context2ds* c2d, struct gstates* gs);
int lemon_zestMenu_update(struct context2ds* c2d, struct gstates* gs);
int lemon_zestMenu_draw(struct context2ds* c2d, struct gstates* gs);
int lemon_bgMenu_update(struct context2ds* c2d, struct gstates* gs);
int lemon_bgMenu_draw(struct context2ds* c2d, struct gstates* gs);
int lemon_lemon_update(struct context2ds* c2d, struct gstates* gs);
int lemon_lemon_draw(struct context2ds* c2d, struct gstates* gs);
int lemon_bg_update(struct context2ds* c2d, struct gstates* gs);
int lemon_bg_draw(struct context2ds* c2d, struct gstates* gs);
int lemon_gacha_update(struct context2ds* c2d, struct gstates* gs);
int lemon_gacha_draw(struct context2ds* c2d, struct gstates* gs);

int lemon_setMenu(struct context2ds* c2d, struct gstates* gs, int value, void* data);
int lemon_setPeel(struct context2ds* c2d, struct gstates* gs, int value, void* data);
int lemon_setZest(struct context2ds* c2d, struct gstates* gs, int value, void* data);
int lemon_setBG(struct context2ds* c2d, struct gstates* gs, int value, void* data);