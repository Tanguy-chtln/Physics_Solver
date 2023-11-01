#ifndef __EVENTS_H__
#define __EVENTS_H__

#include "SDL_perso.h"

void set_marginX(double marginX);
double get_marginX();

void set_marginY(double marginY);
double get_marginY();

void set_resRatio(double resRatio);
double get_resRatio();

void set_accelPower(double accelPower);
double get_accelPower();

void set_accMeth(double accMeth);
int get_accMeth();

void set_symState(int symState);
int get_symState();

void init_eventVariables();
void process_mousemotion(SDL_Event* event);
void process_keydown(SDL_Event* event);
void process_mousewheel(SDL_Event* event);

#endif