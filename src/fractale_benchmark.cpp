#include <draw_parallel.h>
#include <draw_serial.h>

#include <iostream>
#include <fstream>
#include <chrono>
#include <cmath> 
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

  struct BenchmarkData {
    uint64_t power;
    int numThreads;
    double serialDuration;
    double parallelDuration;
    double acceleration;
  };

  std::vector<BenchmarkData> benchmarkResults;

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
    
    uint64_t power = static_cast<double>(std::pow(2, static_cast<double>(i)));
    std::cout << "***** Puissance : " << power << " *****" << std::endl;

    for ( double numThreads = 2; numThreads <= maxThreads; numThreads = std::pow(numThreads, 2)) {
      std::cout << "  -- Thread : " << numThreads << " --" << std::endl;
      double serialTotalTime = 0;
      double parallelTotalTime = 0;

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

        if (canvas_test_1 != NULL)
          FREE(canvas_test_1);
          
        if (canvas_test_2 != NULL)
          FREE(canvas_test_2);
          
      }
      double avgSerialTime = serialTotalTime / occurrence;
      double avgParallelTime = parallelTotalTime / occurrence;
      double acceleration = avgSerialTime - avgParallelTime;
      if (acceleration < 0) acceleration = 0;
      std::cout << "    Série (ms) : " << avgSerialTime << std::endl;
      std::cout << "    Parallèle (ms) : " << avgParallelTime << std::endl;

      BenchmarkData data;
      data.power = power;
      data.numThreads = numThreads;
      data.serialDuration = avgSerialTime;
      data.parallelDuration = avgParallelTime;
      data.acceleration = acceleration;
      benchmarkResults.push_back(data);
    }
  }
  
  FREE(couleur_test);

  std::ofstream outputFile("benchmark2.csv");
  if (outputFile.is_open()) {
    outputFile << "Puissance, Nombre de Thread, Duree Serie, Duree Parallele, " << std::endl;

    for (const BenchmarkData& result : benchmarkResults) {
        outputFile << result.power << ", " << result.numThreads << ", "
                    << result.serialDuration << ", " << result.parallelDuration << ", "
                    << result.acceleration << std::endl;
    }
    outputFile.close();
  } else {
    std::cerr << "Impossible d'ouvrir le fichier benchmark.csv pour écriture." << std::endl;
    goto err;
  }

  done:
    return ret;
  err:
    ret = -1;
    goto done;
}

