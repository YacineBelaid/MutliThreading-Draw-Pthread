#include <cstring>
#include <iostream>
#include <vector>
#include <sstream>

#include "modules.h"
#include "product.hpp"

class test_item {
public:
  test_item(size_t s, int n, int w, int h) : size(s), nb_thread(n), width(w), height(h) {}
  size_t size;
  int nb_thread;
  int width;
  int height;
};

static int check_limits(test_item* opts) {
  int ret = 0;
  int i;
  limits_t lim_expected, lim_actual;
  memset(&lim_expected, 0, sizeof(limits_t));

  if (dragon_limits_serial(&lim_expected, opts->size, opts->nb_thread) < 0) {
    std::printf("Error: limits serial failed\n");
    return -1;
  }

  for (i = 1; libs[i].lib != THREAD_LIB_NONE; i++) {
    memset(&lim_actual, 0, sizeof(limits_t));
    const char* name = libs[i].name;
    ret = libs[i].limits(&lim_actual, opts->size, opts->nb_thread);
    if (ret < 0) {
      std::printf("Error executing limits with %s\n", name);
      return -1;
    }
    if (cmp_limits(&lim_expected, &lim_actual) == 0) {
      std::printf("PASS %10s %10s\n", "limits", name);
    } else {
      ret = -1;
      std::printf("FAIL %10s %10s\n", "limits", name);
      std::printf("expected: ");
      dump_limits(&lim_expected);
      std::printf("actual  : ");
      dump_limits(&lim_actual);
    }
  }
  return ret;
}

static int check_draw(test_item* opts) {
  int ret = 0;
  int i;
  limits_t limits;
  int area;
  int dragon_width;
  int dragon_height;
  int threshold;
  char *drg_exp = NULL, *drg_act = NULL;
  struct rgb *img_exp = NULL, *img_act = NULL;
  char *f1 = NULL, *f2 = NULL;
  static const char* fmt = "%s %10s %10s threshold=%d gap=%d (%.3f%%)\n";

  if (dragon_limits_serial(&limits, opts->size, opts->nb_thread) < 0) {
    std::printf("Error: limits serial failed\n");
    goto err;
  }

  dragon_width = limits.maximums.x - limits.minimums.x;
  dragon_height = limits.maximums.y - limits.minimums.y;
  area = dragon_width * dragon_height;
  threshold = opts->nb_thread * 2;

  img_exp = make_canvas(opts->width, opts->height);
  img_act = make_canvas(opts->width, opts->height);
  if (img_exp == NULL || img_act == NULL) goto err;

  if (dragon_draw_serial(&drg_exp, img_exp, opts->width, opts->height, opts->size, opts->nb_thread)
      < 0) {
    std::printf("Error: draw serial failed\n");
    goto err;
  }

  for (i = 1; libs[i].lib != THREAD_LIB_NONE; i++) {
    const char* name = libs[i].name;
    ret = libs[i].draw(&drg_act, img_act, opts->width, opts->height, opts->size, opts->nb_thread);
    if (ret < 0) {
      std::printf("Error executing draw with %s\n", name);
      goto err;
    }
    int gap = cmp_canvas(drg_exp, drg_act, dragon_width, dragon_height, true);
    float gap_f = gap * 100 / ((float)area);
    if (gap < threshold && gap >= 0) {
      std::printf(fmt, "PASS", "draw", name, threshold, gap, gap_f);
    } else {
      ret = -1;
      std::printf(fmt, "FAIL", "draw", name, threshold, gap, gap_f);
      std::string f1 = "dragon_check_failed_serial.ppm";
      std::string f2 = std::string("dragon_check_failed_") + name + std::string(".ppm");
      if (write_img(img_exp, f1.c_str(), opts->width, opts->height) < 0) goto err;
      if (write_img(img_act, f2.c_str(), opts->width, opts->height) < 0) goto err;
      std::printf("expected: %s\n", f1.c_str());
      std::printf("actual  : %s\n", f2.c_str());
    }
    FREE(drg_act);
  }

done:
  FREE(img_exp);
  FREE(img_act);
  FREE(drg_exp);
  FREE(drg_act);
  FREE(f1);
  FREE(f2);
  return ret;
err:
  ret = -1;
  goto done;
}

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;
  int res = 0;

  std::vector<size_t> s{1 << 9, 1 << 10, 1 << 20};
  std::vector<int> t{1, 2, 5, 8};
  std::vector<int> w{512, 99};
  std::vector<int> h{512, 99};

  for (const auto& p : iter::product(s, t, w, h)) {
    std::cout << std::get<0>(p) << ", " << std::get<1>(p) << ", " << std::get<2>(p) << ", "
              << std::get<3>(p) << ", " << std::endl;

    test_item item(std::get<0>(p), std::get<1>(p), std::get<2>(p), std::get<3>(p));
    if (check_limits(&item) < 0  || check_draw(&item) < 0) {
      printf("test");
      res = -1;
      break;
    }
  }

  return res;
}
