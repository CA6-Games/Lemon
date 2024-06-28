#pragma once

#include "game.h"

int utils2d_pointInRect(int x, int y, int w, int h, int x2, int y2);
int utils2d_clickInRect(struct context2ds* c2d, int w, int h, int x, int y);
int utils2d_holdInRect(struct context2ds* c2d, int w, int h, int x, int y);

int utils2d_loadTexture(struct context2ds* c2d, char* path, void** texture);