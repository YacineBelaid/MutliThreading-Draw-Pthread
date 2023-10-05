#include <draw_parallel.h>
#include <draw_serial.h>

#include <chrono>
#include <cmath>  // Ajout de cette inclusion pour utiliser std::pow
#include <iostream>
#include <thread>
#include <vector>

int main() {
  /*
   * Créer un banc d'essai de performance pour mesurer l'accélération en
   * fonction du nombre de processeur et de la taille du problème.
   *
   * Faire varier la taille du dessin de la puissance 10 à 29 et le nombre de
   * thread de 1 à hardware_concurrency() par puissance de 2.
   *
   * Utilisez ces données dans votre rapport.
   */

  int ret = 0;
  int maxThreads = std::thread::hardware_concurrency();
  int occurrence = 10;
  int width = 2;
  int height = 2;
  std::vector<int> powers = {10,20,25,29};//25 29

  struct rgb* couleur_test = (rgb*) malloc(sizeof(rgb));
  couleur_test->r = 255;
  couleur_test->b = 4;
  couleur_test->g = 1;

  for (int i :powers) {
    
    // Calcul de la taille du dessin en fonction de la puissance
    uint64_t power = static_cast<double>(std::pow(width*height, static_cast<double>(i)));
    std::cout << "***** Puissance : " << power << " *****" << std::endl;

    for ( double numThreads = 1; numThreads < maxThreads; numThreads = std::pow(numThreads, 2)) {
      std::cout << "  -- Thread : " << numThreads << " --" << std::endl;
      double serialTotalTime = 0.0;
      double parallelTotalTime = 0.0;

       for (int j = 0; j < occurrence; j++) {
        char** canvas_test_1 = (char**)malloc(sizeof(char*)*power);
        char** canvas_test_2 =  (char**)malloc(sizeof(char*)*power);
     
        auto serialStartTime = std::chrono::steady_clock::now();
        dragon_draw_serial(canvas_test_1, couleur_test, height, width, power, 1);
        auto serialEndTime = std::chrono::steady_clock::now();
        double serialDuration = std::chrono::duration_cast<std::chrono::milliseconds>(serialEndTime - serialStartTime).count();
        serialTotalTime += serialDuration;

        auto parallelStartTime = std::chrono::steady_clock::now();
        dragon_draw_parallel(canvas_test_2, couleur_test, width,height, power, numThreads);
        auto parallelEndTime = std::chrono::steady_clock::now();
        double parallelDuration = std::chrono::duration_cast<std::chrono::milliseconds>(parallelEndTime - parallelStartTime).count();
        parallelTotalTime += parallelDuration;

        // Libérer la mémoire
        if (canvas_test_1 != NULL)
          FREE(canvas_test_1);
          
        if (canvas_test_2 != NULL)
          FREE(canvas_test_2);
          
      }
      double avgSerialTime = serialTotalTime / occurrence;
      double avgParallelTime = parallelTotalTime / occurrence;
      std::cout << "    Série (ms) : " << avgSerialTime << std::endl;
      std::cout << "    Parallèle (ms) : " << avgParallelTime << std::endl;
    }
  }

err:
  return -1;
done:
  return ret;
}

