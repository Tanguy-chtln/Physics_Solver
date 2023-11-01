#include "entity.h"
#include "SDL_perso.h"
#include "dimensions.h"
#include "events.h"

void add_entity(struct Render_Entity_t* renderEntities, struct Entity_t* entities, int i)
{
    struct Entity_t* entity = entities + i;

    if (i % 2 == 0) {
        entity->position.x = 2.1;
        entity->position.y = 10 + i % 40;
        entity->lastPosition = entity->position;
        entity->lastPosition.x -= 0.3f;
        entity->lastPosition.y -= (i % 40) / 600.;
    } else if (i % 2 == 1) {
        entity->position.x = GRIDX - 2.1;
        entity->position.y = 10 + i % 40;
        entity->lastPosition = entity->position;
        entity->lastPosition.x += 0.3f;
        entity->lastPosition.y -= (i % 40) / 600. - 1 / 600.;
    }
    entity->radius = RADIUS;

    struct Render_Entity_t* renderEntity = renderEntities + i;
    renderEntity->position = &(entity->position);
    SDL_Rect imageRect = {
        (entity->position.x - entity->radius) * get_resRatio(),
        (entity->position.y - entity->radius) * get_resRatio(),
        2 * entity->radius * get_resRatio(),
        2 * entity->radius * get_resRatio()
    };
    renderEntity->imageRect = imageRect;
    renderEntity->radius = entity->radius;
}