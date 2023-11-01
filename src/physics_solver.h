#ifndef __PHYSICS_SOLVER_H__
#define __PHYSICS_SOLVER_H__

#include "SDL_perso.h"
#include "entity.h"
#include "grid.h"
#include "thread_pool.h"

struct thread_physics_entities_t;

struct render_args_t {
    struct SDL_Renderer* renderer;
    struct Render_Entity_t* renderEntities;
    SDL_Texture* texture;
};

struct thread_grid_t {
    struct Cell_t* start;
    struct Cell_t* stop;
};

void update(threadpool thpool, struct Cell_t* grid, struct Entity_t* entities, int entityCount, struct thread_grid_t* threadedGrid, struct thread_physics_entities_t* threadEntities);

struct thread_physics_entities_t* init_entities_arguments(struct Entity_t* entities, int* entityCount);

void free_entities_args(struct thread_physics_entities_t* threadEntities);

#endif