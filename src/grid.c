#include "grid.h"
#include "entity.h"

struct Cell_t* init_grid()
{
    struct Cell_t* grid = malloc(sizeof(struct Cell_t) * GRIDX * GRIDY);
    for (int i = 0; i < GRIDX; i++) {
        for (int j = 0; j < GRIDY; j++) {
            struct Cell_t* cell = grid + i * GRIDY + j;
            cell->id = i * GRIDY + j;
            cell->len = 0;
            for (int k = 0; k < CELL_TAB_SIZE; k++) {
                cell->entities[k] = NULL;
            }
        }
    }
    return grid;
}

void clear_grid(struct Cell_t* grid)
{
    for (struct Cell_t* cell = grid; cell < grid + GRIDX * GRIDY; cell++)
        cell->len = 0;
}

void update_grid(struct Cell_t* grid, struct Entity_t* entities, int entityCount)
{
    clear_grid(grid);
    for (struct Entity_t* entity = entities; entity < entities + entityCount; entity++) {
        struct Cell_t* cell = grid + (int)(entity->position.x) * GRIDY + (int)(entity->position.y);
        cell->entities[cell->len++] = entity;
    }
}
