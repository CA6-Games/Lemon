#include "lemon.h"


struct menusItems lemon_topBarItems[] = {
    {"Main",            "res/ui/btnHelp.png",           NULL,           lemon_setMenu,      0,      NULL, 1, 1},
    {"Lemon",           "res/ui/btnLemon.png",          NULL,           lemon_setMenu,      1,      NULL, 0, 1},
    {"Skin",            "res/ui/btnPeel.png",           NULL,           lemon_setMenu,      2,      NULL, 0, 1},
    {"Zest",            "res/ui/btnZest.png",           NULL,           lemon_setMenu,      3,      NULL, 0, 1},
    {"Background",      "res/ui/btnBG.png",             NULL,           lemon_setMenu,      4,      NULL, 0, 1},
    {"Gacha",           "res/ui/btnGacha.png",          NULL,           lemon_setMenu,      5,      NULL, 0, 0},
    {"Quit",            "res/ui/btnQuit.png",           NULL,           lemon_setMenu,      -1,     NULL, 0, 1}
};
struct menu_navBars lemon_navBar = {lemon_topBarItems, sizeof(lemon_topBarItems) / sizeof(struct menusItems), NULL, NULL, NULL, 0, 0, 1280, 720 / 15};

struct menusItems lemon_peelItems[] = {
    {"Lemon",           "res/peel/lemon.png",           NULL,           lemon_setPeel,      0,      NULL, 1, 1},

    {"Polka",           "res/peel/polka.png",           NULL,           lemon_setPeel,      100,    NULL, 0, 0},
    {"Camo",            "res/peel/camo.png",            NULL,           lemon_setPeel,      400,    NULL, 0, 0},
    {"Honey",           "res/peel/honey.png",           NULL,           lemon_setPeel,      1000,    NULL, 0, 0},
};
struct menu_grids lemon_peelMenu = {lemon_peelItems, sizeof(lemon_peelItems) / sizeof(struct menusItems), NULL, NULL, NULL, NULL, NULL, NULL, 120, 100, 1040, 520, 8, 4, 0};

struct menusItems lemon_zestItems[] = {
    {"none",        "",                             NULL,           lemon_setZest,  0,      NULL, 1, 1},

    {"halos",       "res/zest/halos.png",           NULL,           lemon_setZest,  500,    NULL, 0, 0},
    
    {"lumi",        "res/zest/lumi1.png",           NULL,           lemon_setZest,  1000,   NULL, 0, 0},
    {"lumi",        "res/zest/lumi2.png",           NULL,           lemon_setZest,  1500,   NULL, 0, 0},
    {"gloom",       "res/zest/gloomi1.png",         NULL,           lemon_setZest,  2000,   NULL, 0, 0},
    {"gloom",       "res/zest/gloomi2.png",         NULL,           lemon_setZest,  2500,   NULL, 0, 0},
    {"lucy",        "res/zest/lucy1.png",           NULL,           lemon_setZest,  3000,   NULL, 0, 0},
    {"lucy",        "res/zest/lucy2.png",           NULL,           lemon_setZest,  3500,   NULL, 0, 0},
};
struct menu_grids lemon_zestMenu = {lemon_zestItems, sizeof(lemon_zestItems) / sizeof(struct menusItems), NULL, NULL, NULL, NULL, NULL, NULL, 120, 100, 1040, 520, 8, 4, 0};

struct menusItems lemon_bgItems[] = {
    {"Lemon",           "res/bg/lemon.png",           NULL,           lemon_setBG,  0,      NULL, 1, 1},

    {"Space",           "res/bg/space.png",           NULL,           lemon_setBG,  100,    NULL, 0, 0},
    {"SpaceLemon",      "res/bg/spaceLemon.png",      NULL,           lemon_setBG,  200,    NULL, 0, 0},
    {"Wormhole",        "res/bg/wormhole.png",        NULL,           lemon_setBG,  300,    NULL, 0, 0},
};
struct menu_grids lemon_bgMenu = {lemon_bgItems, sizeof(lemon_bgItems) / sizeof(struct menusItems), NULL, NULL, NULL, NULL, NULL, NULL, 120, 100, 1040, 520, 8, 4, 0};

void* instruction;
void* juice;

// #define TEST_ENABLEALL

int lemon_init(struct context2ds* c2d, struct gstates* gs)
{
    SDL_SetWindowTitle(c2d->w, "Lemon");

    // set the functions
    gs->event = lemon_event;
    gs->update = lemon_update;
    gs->draw = lemon_draw;

    // allocate memory for the data
    gs->data = malloc(sizeof(struct lemon_data));
    if(gs->data == NULL)
    {
        return -1;
    }
    memset(gs->data, 0, sizeof(struct lemon_data));

    struct lemon_data* data = (struct lemon_data*)gs->data;
    data->l.h = 512;
    data->l.w = 512;

    // center the lemon
    data->l.x = 1280 / 2 - data->l.w / 2;
    data->l.y = 720 / 2 - data->l.h / 2;

    // load icons for nav bar
    for(int i = 0; i < lemon_navBar.nbItems; i++)
    {
        if(lemon_navBar.mi[i].path != NULL)
        {
            utils2d_loadTexture(c2d, lemon_navBar.mi[i].path, &lemon_navBar.mi[i].icon);
        }
    }

    // load icons for peel menu
    for(int i = 0; i < lemon_peelMenu.nbItems; i++)
    {
        if(lemon_peelMenu.mi[i].path != NULL)
        {
            utils2d_loadTexture(c2d, lemon_peelMenu.mi[i].path, &lemon_peelMenu.mi[i].icon);
#ifdef TEST_ENABLEALL
            lemon_peelMenu.mi[i].enabled = 1;
#endif
        }
    }

    // load icons for zest menu
    for(int i = 0; i < lemon_zestMenu.nbItems; i++)
    {
        if(lemon_zestMenu.mi[i].path != NULL)
        {
            utils2d_loadTexture(c2d, lemon_zestMenu.mi[i].path, &lemon_zestMenu.mi[i].icon);
#ifdef TEST_ENABLEALL
            lemon_zestMenu.mi[i].enabled = 1;
#endif
        }
    }

    // load icons for bg menu
    for(int i = 0; i < lemon_bgMenu.nbItems; i++)
    {
        if(lemon_bgMenu.mi[i].path != NULL)
        {
            utils2d_loadTexture(c2d, lemon_bgMenu.mi[i].path, &lemon_bgMenu.mi[i].icon);
#ifdef TEST_ENABLEALL
            lemon_bgMenu.mi[i].enabled = 1;
#endif
        }
    }

    utils2d_loadTexture(c2d, "res/ui/btn.png", &lemon_navBar.bgItem);
    utils2d_loadTexture(c2d, "res/ui/btnS.png", &lemon_navBar.bgItemS);
    utils2d_loadTexture(c2d, "res/ui/btnD.png", &lemon_navBar.bgItemD);

    utils2d_loadTexture(c2d, "res/ui/btn2.png", &lemon_peelMenu.bg);
    utils2d_loadTexture(c2d, "res/ui/btn3.png", &lemon_peelMenu.bgItem);
    utils2d_loadTexture(c2d, "res/ui/btn3S.png", &lemon_peelMenu.bgItemS);
    utils2d_loadTexture(c2d, "res/ui/btn3D.png", &lemon_peelMenu.bgItemD);
    utils2d_loadTexture(c2d, "res/ui/btnPrev.png", &lemon_peelMenu.btnPrev);
    utils2d_loadTexture(c2d, "res/ui/btnNext.png", &lemon_peelMenu.btnNext);

    utils2d_loadTexture(c2d, "res/ui/btn2.png", &lemon_zestMenu.bg);
    utils2d_loadTexture(c2d, "res/ui/btn3.png", &lemon_zestMenu.bgItem);
    utils2d_loadTexture(c2d, "res/ui/btn3S.png", &lemon_zestMenu.bgItemS);
    utils2d_loadTexture(c2d, "res/ui/btn3D.png", &lemon_zestMenu.bgItemD);
    utils2d_loadTexture(c2d, "res/ui/btnPrev.png", &lemon_zestMenu.btnPrev);
    utils2d_loadTexture(c2d, "res/ui/btnNext.png", &lemon_zestMenu.btnNext);

    utils2d_loadTexture(c2d, "res/ui/btn2.png", &lemon_bgMenu.bg);
    utils2d_loadTexture(c2d, "res/ui/btn3.png", &lemon_bgMenu.bgItem);
    utils2d_loadTexture(c2d, "res/ui/btn3S.png", &lemon_bgMenu.bgItemS);
    utils2d_loadTexture(c2d, "res/ui/btn3D.png", &lemon_bgMenu.bgItemD);
    utils2d_loadTexture(c2d, "res/ui/btnPrev.png", &lemon_bgMenu.btnPrev);
    utils2d_loadTexture(c2d, "res/ui/btnNext.png", &lemon_bgMenu.btnNext);

    utils2d_loadTexture(c2d, "res/ui/instruction.png", &instruction);
    utils2d_loadTexture(c2d, "res/ui/juice.png", &juice);

    data->nbClick = 0;
    data->menu = 0;
    gs->state = 1; // initialized and ready to run
    
    return 0;
}

int lemon_event(struct context2ds* c2d, struct gstates* gs)
{
    // struct lemon_data* data = (struct lemon_data*)gs->data;
    
    return 0;
}

int lemon_update(struct context2ds* c2d, struct gstates* gs)
{
    struct lemon_data* data = (struct lemon_data*)gs->data;

    menu_navBar_update(c2d, gs, &lemon_navBar);

    switch(data->menu)
    {
        case -1:
            gs->state = 4;
        case 0:
            lemon_mainMenu_update(c2d, gs);
            break;
        case 1:
            lemon_lemon_update(c2d, gs);
            break;
        case 2:
            lemon_skinMenu_update(c2d, gs);
            break;
        case 3:
            lemon_zestMenu_update(c2d, gs);
            break;
        case 4:
            lemon_bg_update(c2d, gs);
            break;
        case 5:
            lemon_gacha_update(c2d, gs);
            break;
    }
    
    return 0;
}

int lemon_draw(struct context2ds* c2d, struct gstates* gs)
{
    struct lemon_data* data = (struct lemon_data*)gs->data;

    SDL_Rect rect;

    // draw the background
    rect.x = 0;
    rect.y = 0;
    rect.w = 1280;
    rect.h = 720;
    for(int i = 0; i < lemon_bgMenu.nbItems; i++)
    {
        if(lemon_bgMenu.mi[i].value == data->l.bg)
        {
            if(lemon_bgMenu.mi[i].icon != NULL)
            {
                SDL_RenderCopy(c2d->r, lemon_bgMenu.mi[i].icon, NULL, &rect);
            }
            break;
        }
    }

    menu_navBar_draw(c2d, gs, &lemon_navBar);

    switch(data->menu)
    {
        case 0:
            lemon_mainMenu_draw(c2d, gs);
            break;
        case 1:
            lemon_lemon_draw(c2d, gs);
            break;
        case 2:
            lemon_skinMenu_draw(c2d, gs);
            break;
        case 3:
            lemon_zestMenu_draw(c2d, gs);
            break;
        case 4:
            lemon_bg_draw(c2d, gs);
            break;
        case 5:
            lemon_gacha_draw(c2d, gs);
            break;
    }
    return 0;
}
int lemon_setMenu(struct context2ds* c2d, struct gstates* gs, int value, void* data)
{
    struct lemon_data* ldata = (struct lemon_data*)gs->data;
    ldata->menu = value;
    return 0;
}

int lemon_setPeel(struct context2ds* c2d, struct gstates* gs, int value, void* data)
{
    struct lemon_data* ldata = (struct lemon_data*)gs->data;
    ldata->l.peel = value;
    return 0;
}

int lemon_setZest(struct context2ds* c2d, struct gstates* gs, int value, void* data)
{
    struct lemon_data* ldata = (struct lemon_data*)gs->data;
    ldata->l.zest = value;
    return 0;
}

int lemon_setBG(struct context2ds* c2d, struct gstates* gs, int value, void* data)
{
    struct lemon_data* ldata = (struct lemon_data*)gs->data;
    ldata->l.bg = value;
    return 0;
}

// ------------- main menu

int lemon_mainMenu_update(struct context2ds* c2d, struct gstates* gs)
{
    // struct lemon_data* data = (struct lemon_data*)gs->data;

    return 0;
}

int lemon_mainMenu_draw(struct context2ds* c2d, struct gstates* gs)
{
    struct lemon_data* data = (struct lemon_data*)gs->data;
    SDL_Rect rect;

    rect.x = data->l.x;
    rect.y = data->l.y;
    rect.w = data->l.w;
    rect.h = data->l.h;

    SDL_RenderCopy(c2d->r, instruction, NULL, &rect);

    return 0;
}

// ------------- skin

int lemon_skinMenu_update(struct context2ds* c2d, struct gstates* gs)
{
    // struct lemon_data* data = (struct lemon_data*)gs->data;

    menu_grid_update(c2d, gs, &lemon_peelMenu);
    
    return 0;
}

int lemon_skinMenu_draw(struct context2ds* c2d, struct gstates* gs)
{
    // struct lemon_data* data = (struct lemon_data*)gs->data;
    
    menu_grid_draw(c2d, gs, &lemon_peelMenu);

    return 0;
}

// ------------- zest

int lemon_zestMenu_update(struct context2ds* c2d, struct gstates* gs)
{
    // struct lemon_data* data = (struct lemon_data*)gs->data;

    menu_grid_update(c2d, gs, &lemon_zestMenu);

    return 0;
}


int lemon_zestMenu_draw(struct context2ds* c2d, struct gstates* gs)
{
    // struct lemon_data* data = (struct lemon_data*)gs->data;
    
    menu_grid_draw(c2d, gs, &lemon_zestMenu);

    return 0;
}

// ------------- bg

int lemon_bg_update(struct context2ds* c2d, struct gstates* gs)
{
    // struct lemon_data* data = (struct lemon_data*)gs->data;

    menu_grid_update(c2d, gs, &lemon_bgMenu);

    return 0;
}


int lemon_bg_draw(struct context2ds* c2d, struct gstates* gs)
{
    // struct lemon_data* data = (struct lemon_data*)gs->data;
    
    menu_grid_draw(c2d, gs, &lemon_bgMenu);

    return 0;
}

// ------------- lemon -------------

int lemon_lemon_update(struct context2ds* c2d, struct gstates* gs)
{
    struct lemon_data* data = (struct lemon_data*)gs->data;

    // did the mouse click the lemon?
    if(c2d->i->mp.l == 0 && c2d->i->mn.l == 1)
    {
        if(utils2d_clickInRect(c2d, data->l.w, data->l.h, data->l.x, data->l.y))
        {
            data->nbClick++;
            printf("Lemon clicked %d times\n", data->nbClick); fflush(stdout);

            if(data->nbClick > 100 && data->autoPull){
                // gatcha_pull();
            }

        }
    }

    // go throu the peel menu and enable the ones that value < nbClick
    for(int i = 0; i < lemon_peelMenu.nbItems; i++)
    {
        if(lemon_peelMenu.mi[i].value < data->nbClick)
        {
            lemon_peelMenu.mi[i].enabled = 1;
        }
        else
        {
            lemon_peelMenu.mi[i].enabled = 0;
        }
    }

    // go throu the zest menu and enable the ones that value < nbClick
    for(int i = 0; i < lemon_zestMenu.nbItems; i++)
    {
        if(lemon_zestMenu.mi[i].value < data->nbClick)
        {
            lemon_zestMenu.mi[i].enabled = 1;
        }
        else
        {
            lemon_zestMenu.mi[i].enabled = 0;
        }
    }

    // go throu the bg menu and enable the ones that value < nbClick
    for(int i = 0; i < lemon_bgMenu.nbItems; i++)
    {
        if(lemon_bgMenu.mi[i].value < data->nbClick)
        {
            lemon_bgMenu.mi[i].enabled = 1;
        }
        else
        {
            lemon_bgMenu.mi[i].enabled = 0;
        }
    }
    
    return 0;
}

int lemon_lemon_draw(struct context2ds* c2d, struct gstates* gs)
{
    struct lemon_data* data = (struct lemon_data*)gs->data;
    SDL_Rect rect;

    // draw the lemon
    rect.x = data->l.x;
    rect.y = data->l.y;
    rect.w = data->l.w;
    rect.h = data->l.h;
    for(int i = 0; i < lemon_peelMenu.nbItems; i++)
    {
        if(lemon_peelMenu.mi[i].value == data->l.peel)
        {
            if(lemon_peelMenu.mi[i].icon != NULL)
            {
                SDL_RenderCopy(c2d->r, lemon_peelMenu.mi[i].icon, NULL, &rect);
            }
            break;
        }
    }

    for(int i = 0; i < lemon_zestMenu.nbItems; i++)
    {
        if(lemon_zestMenu.mi[i].value == data->l.zest)
        {
            if(lemon_zestMenu.mi[i].icon != NULL)
            {
                SDL_RenderCopy(c2d->r, lemon_zestMenu.mi[i].icon, NULL, &rect);
            }
            break;
        }
    }

    // write the number of clicks
    char str[256];
    sprintf(str, "Lemon Slices: %d", data->nbClick);
    ui_ttfWrite(c2d, str, data->l.x+(data->l.w/2), data->l.y+data->l.h+25, 255, 255, 255, lemon_navBar.bgItem);

    if(utils2d_holdInRect(c2d, data->l.w, data->l.h, data->l.x, data->l.y))
    {
        rect.x = c2d->i->mn.x;
        rect.y = c2d->i->mn.y-64;
        rect.w = 64;
        rect.h = 64;
        SDL_RenderCopy(c2d->r, juice, NULL, &rect);
    }

    return 0;
}

// gacha

int lemon_gacha_update(struct context2ds* c2d, struct gstates* gs)
{
    // struct lemon_data* data = (struct lemon_data*)gs->data;


    return 0;
}


int lemon_gacha_draw(struct context2ds* c2d, struct gstates* gs)
{
    // struct lemon_data* data = (struct lemon_data*)gs->data;
    

    return 0;
}