#ifndef __ENTITY_RENDERERING_H__
#define __ENTITY_RENDERERING_H__

#include "physics_solver.h"
#include "thread_pool.h"

struct thread_draw_arguments_t;

struct thread_draw_arguments_t* init_threaded_renderer(struct render_args_t* renderArgs, int* entityCount);

void close_threaded_renderer(struct thread_draw_arguments_t* threadDrawArguments);

void render(threadpool thpool, struct render_args_t* renderArgs, struct thread_draw_arguments_t* threadDrawArguments, int entityCount);

#endif