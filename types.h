#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// -------------------------------------
// Constantes et paramètres
// -------------------------------------

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 1000

// Paramètres de l'apprentissage
#define ETA             0.00001 // Taux d'apprentissage (epsilon)
#define MAX_EPOCHS     5000    // Nombre max d'itérations
#define MIN_ERROR      0.0001  // Seuil d'arrêt sur l'erreur (delta)

// Nombre de points par spirale
#define NB_POINTS_SPIRALE  500


// -------------------------------------
// Structures de données
// -------------------------------------

// Représentation d'un réseau entièrement connecté
typedef struct {
    int nbLayers;      // Nombre total de couches
    int *layerSizes;   // Tableau contenant la taille de chaque coucheq

    // weights[i][j][k] = poids reliant le neurone k de la couche (i-1)
    //                    au neurone j de la couche i
    double ***weights; 

    // outputs[i][j] = sortie du neurone j de la couche i
    double **outputs;

    // delta[i][j] = delta (erreur) du neurone j de la couche i
    double **delta;

} NeuralNetwork;

// Contenant x, y et la classe cible (rouge ou bleu) sous forme d’un one-hot [1, 0] ou [0, 1]
typedef struct {
    double x;
    double y;
    double target[2]; 
} SamplePoint;


#endif