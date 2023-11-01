#include "physics_solver.h"
#include "dimensions.h"
#include "events.h"
#include "shapes.h"

struct thread_physics_entities_t {
    struct Entity_t* start;
    struct Entity_t* stop;
    struct Entity_t* entities;
    int i;
    int* entityCount;
};

Vector_t get_acc(struct Entity_t* entity)
{
    const Vector_t acc = { (GRIDX / 2. - entity->position.x) * get_accelPower(), (GRIDY / 2. - entity->position.y) * get_accelPower() };
    return acc;
}

Vector_t get_mouse_acc(struct Entity_t* entity)
{
    int mouseX;
    int mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    Vector_t acc = { ((mouseX + get_marginX()) / get_resRatio() - entity->position.x) * get_accelPower(), ((mouseY + get_marginY()) / get_resRatio() - entity->position.y) * get_accelPower() };
    return acc;
}

void move_entity(struct Entity_t* entity, Vector_t v)
{
    entity->position.x += v.x;
    entity->position.y += v.y;
}

void wall_collision(struct Entity_t* entity)
{
    if (entity->position.x < 2)
        entity->position.x = 2;
    else if (entity->position.x >= GRIDX - 2)
        entity->position.x = GRIDX - 2;
    if (entity->position.y < 2)
        entity->position.y = 2;
    else if (entity->position.y >= GRIDY - 2)
        entity->position.y = GRIDY - 2;
}

void solve_overlap(struct Entity_t* entity1, struct Entity_t* entity2)
{
    if (entity1 == entity2)
        return;
    const double dx = entity2->position.x - entity1->position.x;
    const double dy = entity2->position.y - entity1->position.y;

    double distance = dx * dx + dy * dy;

    if (distance < 1. && distance > 1e-4f) {

        distance = sqrt(distance);

        const double overlap = (1. - distance) * 0.5;
        const double offsetX = overlap * dx / distance;
        const double offsetY = overlap * dy / distance;

        Vector_t v1 = { -offsetX, -offsetY };
        Vector_t v2 = { offsetX, offsetY };
        move_entity(entity1, v1);
        move_entity(entity2, v2);
    }
}

void process_cells(struct Entity_t* entity1, struct Cell_t* cell2)
{
    for (int cell2Entity = 0; cell2Entity < cell2->len; cell2Entity++)
        solve_overlap(entity1, cell2->entities[cell2Entity]);
}

void process_collisions_threaded(void* voidThreadedGrid)
{
    struct thread_grid_t* threadedGrid = voidThreadedGrid;
    for (struct Cell_t* cell1 = threadedGrid->start; cell1 < threadedGrid->stop; cell1++) {
        for (int cell1Entity = 0; cell1Entity < cell1->len; cell1Entity++) {
            process_cells(cell1->entities[cell1Entity], cell1);
            process_cells(cell1->entities[cell1Entity], cell1 - 1);
            process_cells(cell1->entities[cell1Entity], cell1 + 1);
            process_cells(cell1->entities[cell1Entity], cell1 - 1 - GRIDY);
            process_cells(cell1->entities[cell1Entity], cell1 - GRIDY);
            process_cells(cell1->entities[cell1Entity], cell1 + 1 - GRIDY);
            process_cells(cell1->entities[cell1Entity], cell1 - 1 + GRIDY);
            process_cells(cell1->entities[cell1Entity], cell1 + GRIDY);
            process_cells(cell1->entities[cell1Entity], cell1 + 1 + GRIDY);
        }
    }
}

void collisions_threaded(threadpool thpool, struct thread_grid_t* threadedGrid)
{
    for (int i = 0; i < 20 / 2; i++)
        thpool_add_work(thpool, process_collisions_threaded, (void*)(threadedGrid + 2 * i));

    thpool_wait(thpool);
    for (int i = 0; i < 20 / 2; i++)
        thpool_add_work(thpool, process_collisions_threaded, (void*)(threadedGrid + 2 * i + 1));

    thpool_wait(thpool);
}

struct thread_physics_entities_t* init_entities_arguments(struct Entity_t* entities, int* entityCount)
{
    struct thread_physics_entities_t* threadEntities = malloc(sizeof(struct thread_physics_entities_t) * N_THREADS);
    for (int i = 0; i < N_THREADS; i++) {
        threadEntities[i].i = i;
        threadEntities[i].entities = entities;
        threadEntities[i].entityCount = entityCount;
    }
    return threadEntities;
}

void update_entities_arguments(struct thread_physics_entities_t* threadEntities)
{
    threadEntities->start = threadEntities->entities + (*threadEntities->entityCount * threadEntities->i / N_THREADS);
    if (threadEntities->i != N_THREADS - 1)
        threadEntities->stop = threadEntities->start + (*threadEntities->entityCount / N_THREADS);
    else
        threadEntities->stop = threadEntities->entities + *threadEntities->entityCount;
}

void resolve_physics_threaded(void* voidThreadEntities)
{
    double speedX;
    double speedY;
    struct thread_physics_entities_t* threadEntities = voidThreadEntities;
    update_entities_arguments(threadEntities);
    Vector_t acceleration = { 0, get_accelPower() * 6 };
    for (struct Entity_t* entity = threadEntities->start; entity < threadEntities->stop; entity++) {
        speedX = entity->position.x - entity->lastPosition.x;
        speedY = entity->position.y - entity->lastPosition.y;
        if (!(get_accMeth()))
            acceleration = get_mouse_acc(entity);
        Vector_t v = { speedX + (acceleration.x - speedX * 40.) * (DT * DT),
            speedY + (acceleration.y - speedY * 40.) * (DT * DT) };

        if (get_accMeth() == 4) {
            v.x /= 1.001;
            v.y /= 1.001;
        }
        entity->lastPosition = entity->position;
        move_entity(entity, v);
        wall_collision(entity);
    }
}

void physics_threaded(threadpool thpool, struct thread_physics_entities_t* threadEntities)
{
    for (int i = 0; i < N_THREADS; i++)
        thpool_add_work(thpool, resolve_physics_threaded, (void*)(threadEntities + i));
    thpool_wait(thpool);
}

void update_positions(threadpool thpool, struct Cell_t* grid, struct Entity_t* entities, int entityCount, struct thread_grid_t* threadedGrid, struct thread_physics_entities_t* threadEntities)
{
    update_grid(grid, entities, entityCount);
    collisions_threaded(thpool, threadedGrid);
    physics_threaded(thpool, threadEntities);
}

void update(threadpool thpool, struct Cell_t* grid, struct Entity_t* entities, int entityCount, struct thread_grid_t* threadedGrid, struct thread_physics_entities_t* threadEntities)
{
    for (int step = SUB_STEPS; step; step--)
        update_positions(thpool, grid, entities, entityCount, threadedGrid, threadEntities);
}

void free_entities_args(struct thread_physics_entities_t* threadEntities)
{
    free(threadEntities);
}