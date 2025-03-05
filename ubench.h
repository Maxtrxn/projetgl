#ifndef UBENCH_H
#define UBENCH_H

#include <stdio.h>
#include <time.h>

// Structure pour stocker les informations de benchmark
typedef struct {
    clock_t start_time;  // Temps de début
    clock_t end_time;    // Temps de fin
    double duration;     // Durée en secondes
} Benchmark;

// Fonction pour démarrer le benchmark
static inline void startBenchmark(Benchmark *bench) {
    bench->start_time = clock();  // Récupère l'heure de début
}

// Fonction pour arrêter le benchmark et afficher les résultats
static inline void stopBenchmark(Benchmark *bench, const char *label) {
    bench->end_time = clock();  // Récupère l'heure de fin
    bench->duration = ((double)(bench->end_time - bench->start_time)) / CLOCKS_PER_SEC;  // Calcul de la durée en secondes

    // Affichage des résultats
    printf("[%s] Temps d'exécution : %.6f secondes\n", label, bench->duration);
}

// Macro pour mesurer le temps d'exécution d'une fonction ou d'un bloc de code
#define BENCHMARK_CODE(label, code) {  \
    Benchmark bench;                  \
    startBenchmark(&bench);           \
    code;                             \
    stopBenchmark(&bench, label);     \
}

#endif // UBENCH_H

// Fonction pour mettre a 0 le benchmark
void resetBenchmark(Benchmark *bench) {
    bench->start_time = 0;  // Réinitialiser l'heure de départ
    bench->end_time = 0;    // Réinitialiser l'heure de fin
    bench->duration = 0.0;
}
