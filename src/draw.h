#ifndef __DRAW_H__
#define __DRAW_H__

#include "SDL_perso.h"
#include "entity.h"

void entity_set_rainbow_color(struct Render_Entity_t* renderEntity, int i);

void draw_grid(SDL_Renderer* renderer);

void draw_background(SDL_Renderer* renderer, double resRatio, double marginx, double marginy);

#endif