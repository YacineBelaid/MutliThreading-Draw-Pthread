#pragma once

#include <inttypes.h>

#include "draw_parallel.h"
#include "draw_serial.h"

/* Globals and defaults */
#define DEFAULT_HEIGHT 512
#define DEFAULT_WIDTH 512
#define DEFAULT_SIZE 1048576
#define DEFAULT_NB_THREAD 2
#define DEFAULT_LIB_NAME "serial"
#define DEFAULT_IMG_PATH "fractale.ppm"
#define POWER_MAX 30

typedef int (*cmd_handler)(struct command_opts*);
struct command_def {
  const char* name;
  cmd_handler handler;
};

/*
 * Over POWER_MAX = 30, the types used in array indexes overflows
 * and memory usage is very high, limiting power to 2^29
 * */

enum thread_lib {
  THREAD_LIB_NONE,
  THREAD_LIB_SERIAL,
  THREAD_LIB_PTHREAD,
};

struct command_opts {
  const struct command_def* cmd;
  const struct lib_def* lib;
  char* pgm_path;
  int nb_thread;
  int height;
  int width;
  int power;
  int power_max;
  int verbose;
  uint64_t size;
};

typedef int (*draw_handler)(char**, struct rgb*, int, int, uint64_t, int);
typedef int (*limits_handler)(limits_t*, uint64_t, int);

struct lib_def {
  const char* name;
  enum thread_lib lib;
  draw_handler draw;
  limits_handler limits;
};

extern const struct lib_def libs[];
