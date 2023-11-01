#include "color_fill.h"
#include "dimensions.h"
#include "stdio.h"

SDL_Texture* LoadImgTexture(SDL_Renderer* renderer, char* fileName)
{
    SDL_Surface* surface = IMG_Load(fileName);
    if (surface == NULL) {
        SDL_Log("ERREUR > %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

SDL_Color getCircleColor(Point_t* P, float radius, SDL_Surface* imageSurface)
{
    float centerX = P->x;
    float centerY = P->y;

    float minX = centerX - radius;
    float maxX = centerX + radius;
    float minY = centerY - radius;
    float maxY = centerY + radius;
    int imageWidth = imageSurface->w;
    int imageHeight = imageSurface->h;

    minX = (minX < 0) ? 0 : minX;
    maxX = (maxX >= imageWidth) ? imageWidth - 1 : maxX;
    minY = (minY < 0) ? 0 : minY;
    maxY = (maxY >= imageHeight) ? imageHeight - 1 : maxY;

    int totalRed = 0;
    int totalGreen = 0;
    int totalBlue = 0;
    int pixelCount = 0;

    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {

            int distanceX = x - centerX;
            int distanceY = y - centerY;
            float distance = distanceX * distanceX + distanceY * distanceY;

            if (distance <= radius * radius) {
                Uint32 pixel = *(Uint32*)(imageSurface->pixels + y * imageSurface->pitch + x * imageSurface->format->BytesPerPixel);
                Uint8 red, green, blue;
                SDL_GetRGB(pixel, imageSurface->format, &red, &green, &blue);

                totalRed += red;
                totalGreen += green;
                totalBlue += blue;

                pixelCount++;
            }
        }
    }

    SDL_Color meanPixelColor = { 0, 0, 0, 255 };
    if (pixelCount > 0) {
        meanPixelColor.r = totalRed / pixelCount;
        meanPixelColor.g = totalGreen / pixelCount;
        meanPixelColor.b = totalBlue / pixelCount;
    }
    return meanPixelColor;
}

void setColors(struct Render_Entity_t* entities, int entityCount, char* filename)
{
    SDL_Surface* imageSurface = IMG_Load(filename);
    if (imageSurface == NULL) {
        printf("Erreur lors du chargement de l'image : %s\n", IMG_GetError());
        exit(EXIT_FAILURE);
    }

    const float coefX = 1.f * imageSurface->w / GRIDX;
    const float coefY = 1.f * imageSurface->h / GRIDY;
    const float coef = coefX > coefY ? coefX : coefY;

    for (struct Render_Entity_t* entity = entities; entity < entities + entityCount; entity++) {
        Point_t P = { entity->position->x * coef, entity->position->y * coef };
        entity->color = getCircleColor(&P, entity->radius * coef, imageSurface);
    }
    SDL_FreeSurface(imageSurface);
}