#ifndef __GRID_H2__
#define __GRID_H2__

#include "dimensions.h"

struct Cell_t {
    int id;
    int len;
    struct Entity_t* entities[CELL_TAB_SIZE];
};

struct Cell_t* init_grid();

void update_grid(struct Cell_t* grid, struct Entity_t* entities, int entityCount);

#endif