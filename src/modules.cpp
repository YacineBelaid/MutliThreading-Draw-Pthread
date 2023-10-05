#include "modules.h"

const struct lib_def libs[] = {
    {.name = "serial",
     .lib = THREAD_LIB_SERIAL,
     .draw = dragon_draw_serial,
     .limits = dragon_limits_serial},
    {.name = "parallel",
     .lib = THREAD_LIB_PTHREAD,
     .draw = dragon_draw_parallel,
     .limits = dragon_limits_pthread},
    {.name = NULL, .lib = THREAD_LIB_NONE, .draw = NULL, .limits = NULL},
};
