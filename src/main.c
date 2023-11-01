
#include "SDL_perso.h"
#include <assert.h>
#include <stdbool.h>

#include "dimensions.h"
#include "shapes.h"

#include "color_fill.h"
#include "draw.h"
#include "entity.h"
#include "entity_rendering.h"
#include "events.h"
#include "grid.h"
#include "physics_solver.h"

// Multi-threading
#include "thread_pool.h"

void quit_renderer(SDL_Window* window, SDL_Renderer* renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main()
{
    assert(RADIUS <= 0.5f);
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Surface* image = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("ERREUR > %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    window = SDL_CreateWindow("Physics solver",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        MAP_SIZE_X, MAP_SIZE_Y,
        SDL_WINDOW_SHOWN);
    if (window == NULL) {
        SDL_Log("ERREUR > %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL) {
        SDL_Log("ERREUR > %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    IMG_Init(IMG_INIT_PNG);
    image = IMG_Load("res/circle.png");
    if (image == NULL) {
        SDL_Log("ERREUR > %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    SDL_SetWindowResizable(window, true);
    SDL_Texture* texture = LoadImgTexture(renderer, "./res/circle.png");

    int entityCount = 0;
    char title[100];
    unsigned int stackFPS = 1;
    unsigned int meanFPS = 0;
    Uint32 ellapsedTime = 0;
    bool isRunning = true;
    unsigned int k = 0;
    unsigned int kFinal = 0;

    threadpool thpool = thpool_init(N_THREADS);
    struct Cell_t* grid = init_grid();
    struct Entity_t* entities = malloc(sizeof(struct Entity_t) * ENTITIES_COUNT_MAX);
    struct Render_Entity_t* renderEntities = malloc(sizeof(struct Render_Entity_t) * ENTITIES_COUNT_MAX);
    struct render_args_t renderArgs = { renderer, renderEntities, texture };
    struct thread_physics_entities_t* threadEntities = init_entities_arguments(entities, &entityCount);
    struct thread_draw_arguments_t* threadDrawArguments = init_threaded_renderer(&renderArgs, &entityCount);

    struct thread_grid_t* threadedGrid = malloc(sizeof(struct thread_grid_t) * 20);
    threadedGrid[0].start = grid + GRIDY;
    threadedGrid[0].stop = grid + GRIDY + ((GRIDX - 2) * GRIDY + 1) / (10 * 2);
    for (int i = 1; i < 20; i++) {
        threadedGrid[i].start = threadedGrid[i - 1].stop;
        threadedGrid[i].stop = threadedGrid[i].start + ((GRIDX - 2) * GRIDY + 1) / (10 * 2);
    }
    threadedGrid[19].stop = grid + (GRIDX - 1) * GRIDY + 1;

    init_eventVariables();
    Uint32 previousTime = SDL_GetTicks();
    while (isRunning) {
        for (int adding = 0; adding < 40; adding++) {
            if (entityCount < ENTITIES_COUNT_MAX) {
                if (get_symState() != 1)
                    add_entity(renderEntities, entities, entityCount++);
                if (get_symState() == 0)
                    entity_set_rainbow_color(renderEntities, entityCount - 1);
            } else if (kFinal == 0) { // auto-switch
                kFinal = k;
            } else if (k > kFinal + 15 * 12) { // auto-switch
                set_symState(1);
            }
        }

        if (get_symState() == 1) {
            set_symState(2); // auto-switch
            setColors(renderEntities, entityCount, "./res/image.png");
            entityCount = 0;
            k = 0;
            kFinal = UINT16_MAX;
            render(thpool, &renderArgs, threadDrawArguments, entityCount);
        }

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;

            case (SDL_MOUSEMOTION):
                process_mousemotion(&event);
                break;

            case (SDL_KEYDOWN):
                if (event.key.keysym.sym == 27) // ECHAP
                    isRunning = false;
                else
                    process_keydown(&event);
                break;

            case (SDL_MOUSEWHEEL):
                process_mousewheel(&event);
                break;

            default:
                break;
            }
        }

        update(thpool, grid, entities, entityCount, threadedGrid, threadEntities);
        render(thpool, &renderArgs, threadDrawArguments, entityCount);

        ellapsedTime = SDL_GetTicks() - previousTime;
        if (ellapsedTime < FPS_TIME)
            SDL_Delay(FPS_TIME - ellapsedTime);

        stackFPS += (SDL_GetTicks() - previousTime);
        if (!(k % 100)) {
            meanFPS = 100 * 1000. / stackFPS;
            stackFPS = 0;
        }
        sprintf(title, "Physics Solver | entities : %d | FPS : %d", entityCount, meanFPS);
        SDL_SetWindowTitle(window, title);

        previousTime = SDL_GetTicks();
        k++;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(texture);
    SDL_Quit();

    thpool_destroy(thpool);
    free(entities);
    free(renderEntities);
    free(grid);
    close_threaded_renderer(threadDrawArguments);
    free_entities_args(threadEntities);
    free(threadedGrid);

    return EXIT_SUCCESS;
}
