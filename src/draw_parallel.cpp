#include "draw_parallel.h"

#include <pthread.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <iostream>

#include "color.h"
#include "draw_serial.h"

static pthread_mutex_t mutex_stdout;

void printf_safe(char* format, ...) {
  va_list ap;

  va_start(ap, format);
  pthread_mutex_lock(&mutex_stdout);
  std::vprintf(format, ap);
  pthread_mutex_unlock(&mutex_stdout);
  va_end(ap);
}
void* worker(void* arg) {
  struct draw_data* data = (struct draw_data*)arg;
  int area = data->dragon_width * data->dragon_height;
  uint64_t start = data->id * data->size / data->nb_thread;
  uint64_t end = (data->id + 1) * data->size / data->nb_thread;

  /* 1. Initialiser la surface de dessin avec init_canvas() */
  init_canvas(start, area, data->dragon, -1);
  pthread_barrier_wait(data->barrier);

  /* 2. Dessiner la fractale avec draw_dragon_raw() */
   dragon_draw_raw(start, end, data->dragon, data->dragon_width, data->dragon_height, data->limits, data->id);
   pthread_barrier_wait(data->barrier);

  /* 3. Effectuer le rendu final avec scale_dragon() */
  scale_dragon(start, data->image_height, data->image, data->image_width, data->image_height, data->dragon, data->dragon_width, data->dragon_height, data->palette);

  return NULL;
}

int dragon_draw_parallel(char** canvas, struct rgb* image, int width, int height, uint64_t size,
                         int nb_thread) {
  pthread_t* threads = NULL;
  pthread_barrier_t barrier;
  limits_t limits;
  struct draw_data info;
  char* dragon = NULL;
  int scale_x;
  int scale_y;
  struct draw_data* data = NULL;
  struct palette* palette = NULL;
  int ret = 0;

  palette = init_palette(nb_thread);
  if (palette == NULL) goto err;

  if (pthread_barrier_init(&barrier, NULL, nb_thread) != 0) {
    printf("barrier init error\n");
    goto err;
  }

  /* 1. Calculer les limites du dragon */
  if (dragon_limits_pthread(&limits, size, nb_thread) < 0) {
    goto err;
  }

  info.dragon_width = limits.maximums.x - limits.minimums.x;
  info.dragon_height = limits.maximums.y - limits.minimums.y;

  if ((dragon = (char*)malloc(info.dragon_width * info.dragon_height)) == NULL) {
    printf("malloc error dragon\n");
    goto err;
  }

  if ((data = (struct draw_data*)malloc(sizeof(struct draw_data) * nb_thread)) == NULL) {
    printf("malloc error data\n");
    goto err;
  }

  if ((threads = (pthread_t*)malloc(sizeof(pthread_t) * nb_thread)) == NULL) {
    printf("malloc error threads\n");
    goto err;
  }

  info.image_height = height;
  info.image_width = width;
  scale_x = info.dragon_width / width + 1;
  scale_y = info.dragon_height / height + 1;
  info.scale = (scale_x > scale_y ? scale_x : scale_y);
  info.deltaJ = (info.scale * width - info.dragon_width) / 2;
  info.deltaI = (info.scale * height - info.dragon_height) / 2;
  info.nb_thread = nb_thread;
  info.dragon = dragon;
  info.image = image;
  info.size = size;
  info.limits = limits;
  info.barrier = &barrier;
  info.palette = palette;

  /* 2. Lancement du calcul parallèle principal avec draw_dragon_worker */
  for (int i = 0; i < nb_thread; i++) {
    data[i] = info;
    data[i].id = i;
    pthread_create(&threads[i], nullptr, worker, &data[i]);
  }

  /* 3. Attendre la fin du traitement */
  for (int i = 0; i < nb_thread; i++) {
    pthread_join(threads[i], nullptr);
  }

  if (pthread_barrier_destroy(&barrier) != 0) {
    printf("barrier destroy error\n");
    goto err;
  }

done:
  FREE(data);
  FREE(threads);
  free_palette(palette);
  *canvas = dragon;
  //*canvas = NULL;  // FIXME: retourner le dragon calculé
  return ret;

err:
  FREE(dragon);
  ret = -1;
  goto done;
}
void* dragon_limit_worker(void* data) {
  struct limit_data* lim = (struct limit_data*)data;
  piece_init(&lim->piece);
  piece_limit(lim->start, lim->end, &lim->piece);
  return NULL;
}

/*
 * Calcule les limites en terme de largeur et de hauteur de
 * la forme du dragon. Requis pour allouer la matrice de dessin.
 */
int dragon_limits_pthread(limits_t* limits, uint64_t size, int nb_thread) {

  int ret = 0;
  pthread_t* threads = NULL;
  struct limit_data* data = NULL;
  struct limit_data info_data; 
  piece_t master;

  piece_init(&master);

  if ((threads = (pthread_t*)calloc(nb_thread, sizeof(pthread_t))) == NULL) {
    goto err;
  }

  if ((data = (struct limit_data*)calloc(nb_thread, sizeof(struct limit_data))) == NULL) {
    goto err;
  }
info_data.piece = master;
  /* 1. Lancement du calcul en parallèle avec dragon_limit_worker */
for(int i = 0; i< nb_thread; i++){
  data[i] = info_data;
  data[i].id = i;
  data[i].start = (i * size) / nb_thread;
  data[i].end = ((i+1) * size)/ nb_thread; 
  if(pthread_create(&threads[i],NULL,dragon_limit_worker,&data[i])!=0) {
    goto err;
  }
}
  /* 2. Attendre la fin du traitement */
for(int i = 0;i<nb_thread;i++){
  if(pthread_join(threads[i], NULL) != 0){
    goto err;
  }
}
  /* 3. Fusion des pièces avec piece_merge() */
  for(int i = 0;i<nb_thread;i++){
  piece_merge(&master,&data[i].piece);
}

done:
  FREE(threads);
  FREE(data);
  *limits = master.limits;
  return ret;
err:
  ret = -1;
  goto done;
}
