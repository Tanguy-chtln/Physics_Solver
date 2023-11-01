#include "events.h"
#include "dimensions.h"
#include "shapes.h"
#include <float.h>

double manage_marginX(int actionType, double value)
{
    static double marginX = DBL_MAX;
    if (actionType == 1)
        marginX = value;
    return marginX;
}

void set_marginX(double marginX)
{
    manage_marginX(1, marginX);
}

double get_marginX()
{
    return manage_marginX(0, 0);
}

double manage_marginY(int actionType, double value)
{
    static double marginY = DBL_MAX;
    if (actionType == 1)
        marginY = value;
    return marginY;
}

void set_marginY(double marginY)
{
    manage_marginY(1, marginY);
}

double get_marginY()
{
    return manage_marginY(0, 0);
}

double manage_resRatio(int actionType, double value)
{
    static double resRatio = DBL_MAX;
    if (actionType == 1)
        resRatio = value;
    return resRatio;
}

void set_resRatio(double resRatio)
{
    manage_resRatio(1, resRatio);
}

double get_resRatio()
{
    return manage_resRatio(0, 0);
}

double manage_accelPower(int actionType, double value)
{
    static double accelPower = DBL_MAX;
    if (actionType == 1)
        accelPower = value;
    return accelPower;
}

void set_accelPower(double accelPower)
{
    manage_accelPower(1, accelPower);
}

double get_accelPower()
{
    return manage_accelPower(0, 0);
}

int manage_accMeth(int actionType, double value)
{
    static int accMeth = INT16_MAX;
    if (actionType == 1)
        accMeth = value;
    return accMeth;
}

void set_accMeth(double accMeth)
{
    manage_accMeth(1, accMeth);
}

int get_accMeth()
{
    return manage_accMeth(0, 0);
}

int manage_symState(int actionType, int value)
{
    static int symState = INT16_MAX;
    if (actionType == 1)
        symState = value;
    return symState;
}

void set_symState(int symState)
{
    manage_symState(1, symState);
}

int get_symState()
{
    return manage_symState(0, 0);
}

void init_eventVariables()
{
    set_resRatio(1. * MAP_SIZE_X / GRIDX);
    set_marginX(-(MAP_SIZE_X - GRIDX * get_resRatio()) / 2.);
    set_marginY(0);
    set_accelPower(ACCELERATION);
    set_accMeth(1);
    set_symState(0);
}

void process_mousemotion(SDL_Event* event)
{
    if (SDL_GetMouseState(NULL, NULL)) {
        set_marginX(get_marginX() - event->motion.xrel);
        set_marginY(get_marginY() - event->motion.yrel);
    }
}

void process_keydown(SDL_Event* event)
{
    if (event->key.keysym.sym == 61) // +
    {
        set_accelPower(get_accelPower() + 0.1);
        printf("\tAccel Power : %f\n", get_accelPower());
    } else if (event->key.keysym.sym == 54) // -
    {
        set_accelPower(get_accelPower() - 0.1);
        printf("\tAccel Power : %f\n", get_accelPower());
    } else if (event->key.keysym.sym == 32) // SPACE
    {
        set_accMeth(get_accMeth() ? 0 : 1);
    } else if (event->key.keysym.sym == 110) // n
    {
        set_symState(get_symState() + 1);
    }
}

void process_mousewheel(SDL_Event* event)
{
    double oldResRatio = get_resRatio();
    if (event->wheel.y > 0) // scroll up
    {
        double curResRatio = get_resRatio() + get_resRatio() * event->wheel.y / 7.;
        set_resRatio(curResRatio > 400 ? 400 : curResRatio);
    } else if (event->wheel.y < 0) // scroll down
    {
        double curResRatio = get_resRatio() + get_resRatio() * event->wheel.y / 7.;
        set_resRatio(curResRatio < 1. ? 1. : curResRatio);
    }
    int curMouseStateX, curMouseStateY;
    SDL_GetMouseState(&curMouseStateX, &curMouseStateY);
    set_marginX(get_resRatio() * (get_marginX() + curMouseStateX) / oldResRatio - curMouseStateX);
    set_marginY(get_resRatio() * (get_marginY() + curMouseStateY) / oldResRatio - curMouseStateY);
}