#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "SDL_perso.h"
#include "shapes.h"

struct Entity_t {
    Point_t position;
    Point_t lastPosition;
    double radius;
};

struct Render_Entity_t {
    SDL_Texture* texture;
    Point_t* position;
    double radius;
    SDL_Rect imageRect;
    SDL_Color color;
};

void add_entity(struct Render_Entity_t* renderEntities, struct Entity_t* entities, int i);

#endif