#include "draw.h"
#include "dimensions.h"
#include <math.h>

SDL_Color rainbow(double i)
{
    SDL_Color res;
    double t = 2 * M_PI * i;
    res.r = (int)(sin(t) * sin(t) * 255);
    res.g = (int)(sin(t + 0.33f * 2. * M_PI) * sin(t + 0.33f * 2. * M_PI) * 255);
    res.b = (int)(sin(t + 0.66f * 2. * M_PI) * sin(t + 0.66f * 2.0 * M_PI) * 255);
    res.a = 255;

    return res;
}

void entity_set_rainbow_color(struct Render_Entity_t* renderEntity, int i)
{
    (renderEntity + i)->color = rainbow(i * 1e-5);
}

void draw_background(SDL_Renderer* renderer, double resRatio, double marginx, double marginy)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_Rect imageRect = {
        -marginx + resRatio / 2 - 1,
        -marginy + resRatio / 2 - 1,
        (GRIDX - 1) * resRatio + 1,
        (GRIDY - 1) * resRatio + 1
    };
    SDL_SetRenderDrawColor(renderer, 120, 120, 120, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &imageRect);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(renderer, &imageRect);
}