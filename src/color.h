#pragma once

struct rgb {
  unsigned char r;
  unsigned char g;
  unsigned char b;
};

struct palette {
  struct rgb* colors;
  int len;
};

extern const struct rgb white;
extern const struct rgb black;

void random_color(struct rgb* color);
struct palette* init_palette(int num);
void free_palette(struct palette* palette);
void dump_palette(struct palette* palette);
