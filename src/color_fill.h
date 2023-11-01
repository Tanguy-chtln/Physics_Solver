#ifndef __COLOR_FILL__
#define __COLOR_FILL__

#include "entity.h"
#include "shapes.h"

#include "SDL_perso.h"

SDL_Texture* LoadImgTexture(SDL_Renderer* renderer, char* fileName);

void setColors(struct Render_Entity_t* entities, int entityCount, char* filename);

#endif