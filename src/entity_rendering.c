#include "entity_rendering.h"
#include "draw.h"
#include "events.h"
#include "thread_pool.h"

struct thread_draw_arguments_t {
    struct Render_Entity_t* start;
    struct Render_Entity_t* stop;
    int i;
    struct Render_Entity_t* entities;
    int* entityCount;
};

struct thread_draw_arguments_t* init_threaded_renderer(struct render_args_t* renderArgs, int* entityCount)
{
    struct thread_draw_arguments_t* threadDrawArguments = malloc(sizeof(struct thread_draw_arguments_t) * N_THREADS);
    for (int i = 0; i < N_THREADS; i++) {
        threadDrawArguments[i].entityCount = entityCount;
        threadDrawArguments[i].entities = renderArgs->renderEntities;
        threadDrawArguments[i].i = i;
    }

    return threadDrawArguments;
}

void close_threaded_renderer(struct thread_draw_arguments_t* threadDrawArguments)
{
    free(threadDrawArguments);
}

void update_draw_arguments(struct thread_draw_arguments_t* threadDrawArguments)
{
    int i = threadDrawArguments->i;
    threadDrawArguments->start = threadDrawArguments->entities + (*threadDrawArguments->entityCount * i / N_THREADS);
    if (i != N_THREADS - 1)
        threadDrawArguments->stop = threadDrawArguments->start + (*threadDrawArguments->entityCount / N_THREADS);
    else
        threadDrawArguments->stop = threadDrawArguments->entities + *threadDrawArguments->entityCount;
}

void UpdateQuad(SDL_Rect* quad, Point_t* circle_center, float radius, float resRatio, float marginX, float marginY)
{
    quad->x = (circle_center->x - radius) * resRatio - marginX;
    quad->y = (circle_center->y - radius) * resRatio - marginY;
    quad->h = 2 * radius * resRatio;
    quad->w = 2 * radius * resRatio;
}

void update_quad_threaded(void* args)
{
    struct thread_draw_arguments_t* arg = args;
    update_draw_arguments(args);
    for (struct Render_Entity_t* entity = arg->start; entity < arg->stop; entity++) {
        UpdateQuad(&entity->imageRect, entity->position, RADIUS, get_resRatio(), get_marginX(), get_marginY());
    }
}

void DrawEntity(SDL_Renderer* renderer, SDL_Color drawColor, SDL_Texture* texture, SDL_Rect* quad)
{
    SDL_SetTextureColorMod(texture, drawColor.r, drawColor.g, drawColor.b);
    SDL_RenderCopy(renderer, texture, NULL, quad);
}

void render(threadpool thpool, struct render_args_t* renderArgs, struct thread_draw_arguments_t* threadDrawArguments, int entityCount)
{
    draw_background(renderArgs->renderer, get_resRatio(), get_marginX(), get_marginY());

    for (int i = 0; i < N_THREADS; i++)
        thpool_add_work(thpool, update_quad_threaded, (void*)(threadDrawArguments + i));

    thpool_wait(thpool);

    for (int i = 0; i < entityCount; i++)
        DrawEntity(renderArgs->renderer, renderArgs->renderEntities[i].color, renderArgs->texture, &renderArgs->renderEntities[i].imageRect);
    SDL_RenderPresent(renderArgs->renderer);
}