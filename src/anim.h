#ifndef ANIM_H
#define ANIM_H

#include "system.h"
#include "base.h"

void make_gif(struct TwoDimSystem *sys, uint pixel_DIM, double end_time, uint num_stills, uint ms, char *color_scheme);

#endif
