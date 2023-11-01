#ifndef __DIMENSIONS_H__
#define __DIMENSIONS_H__

#define FPS_LIMIT 60

#define ENTITIES_COUNT_MAX 103000
#define RADIUS 0.5

#define GRIDX ((int)(300 * 1.))
#define GRIDY ((int)(300 * 1.))

#define MAP_SIZE_X 1000
#define MAP_SIZE_Y 1000

#define CELL_TAB_SIZE 4

#define ACCELERATION 5
#define SUB_STEPS 8

#define N_THREADS 10

#define FPS_TIME (1000 / FPS_LIMIT)
#define DT (1.f / (FPS_LIMIT * SUB_STEPS))

#endif
