/**
 * @file types.h
 * @author 
 * @brief Fichier header de types.C
 * @version 0.1
 * @date 2025-03-23
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// -------------------------------------
// Constantes et paramètres
// -------------------------------------
/**
 * @define WINDOW_WIDTH Définittion de la largeur de la fenêtre
 * 
 */
#define WINDOW_WIDTH  1000
/**
 * @define WINDOW_HEIGHT Définittion de la hauteur de la fenêtre
 * 
 */
#define WINDOW_HEIGHT 1000


/**
 * @brief Paramètres de l'apprentissage
 * 
 */
 /**
 * @define ETA Taux d'apprentissage (epsilon)
  */
#define ETA             0.00001 // Taux d'apprentissage (epsilon)
/**
 * @define MAX_EPOCHS Nombre max d'entrainements
*/
#define MAX_EPOCHS     5000    // Nombre max d'itérations
/**
 * @define MIN_ERROR Seuil d'arrêt sur l'erreur (delta)
 * 
 */
#define MIN_ERROR      0.0001  // Seuil d'arrêt sur l'erreur (delta)

/**
 * @define NB_POINTS_SPIRALE Nombre de points par spirale
 * 
 */
#define NB_POINTS_SPIRALE  500


// -------------------------------------
// Structures de données
// -------------------------------------

/**
 * @struct NeuralNetwork types.h
 * @brief Représentation du réseau de neurones
 */
typedef struct {
    int nbLayers;      // Nombre total de couches
    int *layerSizes;   // Tableau contenant la taille de chaque couche
                       // ex: layerSizes[0] = 2, layerSizes[1] = 6, layerSizes[2] = 2, ...

    // weights[i][j][k] = poids reliant le neurone k de la couche (i-1)
    //                    au neurone j de la couche i
    double ***weights; 

    // outputs[i][j] = sortie du neurone j de la couche i
    double **outputs;

    // delta[i][j] = delta (erreur) du neurone j de la couche i
    double **delta;

} NeuralNetwork;

/**
 * @struct SamplePoint types.h
 * @brief Représenation d'un point d'une spirale
 */
typedef struct {
    double x; //Coordonnée en x
    double y; //Coordonnée en y
    double target[2]; //La cible = [1, 0] => bleu ou [0, 1] => rouge
} SamplePoint;


#endif