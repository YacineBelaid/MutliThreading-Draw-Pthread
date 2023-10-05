/*
 * fractale.cpp
 *
 * Programme dessinant la fractale du dragon
 *
 * Author: francis
 */

#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <getopt.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

#include "modules.h"

__attribute__((noreturn)) static void usage(void) {
  fprintf(stderr, "fractale\n");
  fprintf(stderr, "Usage: fractale [OPTIONS] [COMMAND]\n");
  fprintf(stderr, "\nOptions:\n");
  fprintf(stderr, "  --help	this help\n");
  fprintf(stderr, "  --cmd		command [ draw | limits ]\n");
  fprintf(stderr, "  --thread	set number of threads\n");
  fprintf(stderr,
          "  --lib		set the threading library to use "
          "[ serial | parallel ]\n");
  fprintf(stderr, "  --output	set image path output\n");
  fprintf(stderr, "  --height	set dragon height\n");
  fprintf(stderr, "  --width	set dragon width\n");
  fprintf(stderr, "  --size	set dragon size\n");
  fprintf(stderr, "  --power	set dragon size by power\n");
  fprintf(stderr, "  --max		compute all dragon to max power\n");
  fprintf(stderr, "\n");
  exit(EXIT_FAILURE);
}

static int cmd_draw(struct command_opts* opts) {
  char* dragon = NULL;
  struct rgb* img;
  int ret = 0;

  img = make_canvas(opts->width, opts->height);
  if (img == NULL) goto err;

  switch (opts->lib->lib) {
    case THREAD_LIB_SERIAL:
    case THREAD_LIB_PTHREAD:
      if (opts->power > 0 && opts->power_max > 0) {
        int i;
        for (i = opts->power; i <= opts->power_max; i++) {
          uint64_t size = 1LL << i;
          if (opts->verbose) printf("draw size=%ld\n", size);
          ret = opts->lib->draw(&dragon, img, opts->width, opts->height, size, opts->nb_thread);
          if (i != opts->power_max) FREE(dragon);
          if (ret < 0) break;
        }
      } else {
        if (opts->verbose) printf("draw size=%ld\n", opts->size);
        ret = opts->lib->draw(&dragon, img, opts->width, opts->height, opts->size, opts->nb_thread);
      }
      break;
    case THREAD_LIB_NONE:
    default:
      ret = -1;
      break;
  }
  if (ret < 0) goto err;

  write_img(img, opts->pgm_path, opts->width, opts->height);
done:
  FREE(dragon);
  FREE(img);
  return ret;
err:
  ret = -1;
  goto done;
}

static const struct command_def cmd_draw_def = {.name = "draw", .handler = cmd_draw};

static int cmd_limits(struct command_opts* opts) {
  int ret = 0;
  limits_t limits;
  memset(&limits, 0, sizeof(limits_t));

  switch (opts->lib->lib) {
    case THREAD_LIB_SERIAL:
    case THREAD_LIB_PTHREAD:
      if (opts->power > 0 && opts->power_max > 0) {
        int i;
        for (i = opts->power; i <= opts->power_max; i++) {
          uint64_t size = 1LL << i;
          if (opts->verbose) std::printf("limits size=%ld\n", size);
          ret = opts->lib->limits(&limits, size, opts->nb_thread);
          if (ret < 0) break;
        }
      } else {
        if (opts->verbose) std::printf("limits size=%ld\n", opts->size);
        ret = opts->lib->limits(&limits, opts->size, opts->nb_thread);
      }
      break;
    case THREAD_LIB_NONE:
    default:
      ret = -1;
  }

  if (ret < 0) goto err;

  dump_limits(&limits);
done:
  return ret;
err:
  ret = -1;
  goto done;
}

static const struct command_def cmd_limit_def = {.name = "limits", .handler = cmd_limits};

static const struct command_def cmd_def_last = {.name = NULL, .handler = NULL};

static const struct command_def* commands[] = {&cmd_draw_def, &cmd_limit_def, &cmd_def_last};

static const struct command_def* lookup_cmd(const char* name) {
  for (int i = 0; commands[i]->name != NULL; i++) {
    if (strcmp(commands[i]->name, name) == 0) return commands[i];
  }
  return NULL;
}

static const struct lib_def* lookup_lib(const char* name) {
  int i;
  for (i = 0; libs[i].name != NULL; i++) {
    if (strcmp(libs[i].name, name) == 0) return &libs[i];
  }
  return NULL;
}

static void dump_opts(struct command_opts* opts) {
  std::printf("%10s %s\n", "option", "value");
  std::printf("%10s %s\n", "cmd", opts->cmd->name);
  std::printf("%10s %s\n", "lib", opts->lib->name);
  std::printf("%10s %s\n", "output", opts->pgm_path);
  std::printf("%10s %d\n", "thread", opts->nb_thread);
  std::printf("%10s %d\n", "height", opts->height);
  std::printf("%10s %d\n", "width", opts->width);
  std::printf("%10s %ld\n", "size", opts->size);
  std::printf("%10s %d\n", "power", opts->power);
  std::printf("%10s %d\n", "max", opts->power_max);
}

void default_int_value(int* val, int def) {
  if (*val == 0) *val = def;
}

static int parse_opts(int argc, char** argv, struct command_opts* opts) {
  int idx;
  int opt;
  int ret = 0;

  struct option options[]
      = {{"help", 0, 0, 'h'},  {"cmd", 1, 0, 'c'},    {"thread", 1, 0, 't'},  {"output", 1, 0, 'o'},
         {"lib", 1, 0, 'l'},   {"height", 5, 0, 'y'}, {"width", 1, 0, 'x'},   {"size", 1, 0, 's'},
         {"power", 1, 0, 'p'}, {"max", 1, 0, 'm'},    {"verbose", 0, 0, 'v'}, {0, 0, 0, 0}};

  memset(opts, 0, sizeof(struct command_opts));

  while ((opt = getopt_long(argc, argv, "hvx:y:s:c:t:l:p:o:m:", options, &idx)) != -1) {
    switch (opt) {
      case 'c':
        opts->cmd = lookup_cmd(optarg);
        break;
      case 't':
        opts->nb_thread = atoi(optarg);
        break;
      case 'l':
        opts->lib = lookup_lib(optarg);
        if (opts->lib == NULL) {
          std::printf("unknown threading lib %s\n", optarg);
          ret = -1;
        }
        break;
      case 'o':
        opts->pgm_path = strdup(optarg);
        break;
      case 'y':
        opts->height = atoi(optarg);
        break;
      case 'x':
        opts->width = atoi(optarg);
        break;
      case 's':
        opts->size = atoi(optarg);
        break;
      case 'p':
        opts->power = atoi(optarg);
        break;
      case 'm':
        opts->power_max = atoi(optarg);
        break;
      case 'h':
        usage();
        break;
      case 'v':
        opts->verbose = 1;
        break;
      default:
        std::printf("unknown option %c\n", opt);
        ret = -1;
        break;
    }
  }

  /* default values*/
  if (opts->lib == NULL) opts->lib = lookup_lib(DEFAULT_LIB_NAME);

  if (opts->pgm_path == NULL) opts->pgm_path = strdup(DEFAULT_IMG_PATH);

  if (opts->size > (1LL << POWER_MAX)) {
    std::printf("Error: size must be lower or equals to %ld\n", opts->size);
    ret = -1;
  }
  if ((opts->power < 0) || (opts->power >= POWER_MAX)) {
    std::printf("Error: power argument out of range [0,%d]\n", POWER_MAX);
    ret = -1;
  }

  if (opts->power_max < 0 || opts->power >= POWER_MAX) {
    std::printf("Error: max argument out of range [0,%d]\n", POWER_MAX);
    ret = -1;
  }

  if (opts->power > 0 && opts->power_max > 0) {
    if (opts->power > opts->power_max) {
      std::printf("Error: max must be greater than or equals to power\n");
      ret = -1;
    }
  }

  if (opts->power > 0) opts->size = 1LL << opts->power;

  if (opts->size == 0) opts->size = DEFAULT_SIZE;

  default_int_value(&opts->height, DEFAULT_HEIGHT);
  default_int_value(&opts->width, DEFAULT_WIDTH);
  default_int_value(&opts->nb_thread, DEFAULT_NB_THREAD);

  if (opts->width == 0 || opts->height == 0) {
    std::printf("argument error: height and width must be greater than 0\n");
    ret = -1;
  }

  if (opts->verbose) dump_opts(opts);

done:
  return ret;
err:
  ret = -1;
  goto done;
}

int main(int argc, char** argv) {
  int ret = 0;
  struct command_opts opts;
  if (parse_opts(argc, argv, &opts) < 0) {
    std::printf("Error while parsing arguments\n");
    usage();
  }

  if (opts.cmd == NULL) {
    std::printf("Select a command to run\n");
    usage();
  }

  if ((opts.cmd->handler(&opts)) < 0) {
    std::printf("Error while executing command %s\n", opts.cmd->name);
    goto err;
  }

done:
  FREE(opts.pgm_path);
  return ret;

err:
  ret = EXIT_FAILURE;
  goto done;
}
