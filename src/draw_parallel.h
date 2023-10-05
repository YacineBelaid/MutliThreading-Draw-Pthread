#pragma once

#include "draw_serial.h"

int dragon_draw_parallel(char** canvas, struct rgb* image, int width, int height, uint64_t size,
                        int nb_thread);
int dragon_limits_pthread(limits_t* lim, uint64_t size, int nb_thread);
