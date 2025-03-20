#ifndef NEURONE_H
#define NEURONE_H

#include "types.h"
//#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>


#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

// Fonctions de gestion du réseau
NeuralNetwork* createNetwork(int nbLayers, int *layerSizes);
void freeNetwork(NeuralNetwork *net);

// Fonctions de sauvegarde et chargement du réseau
int saveNetwork(NeuralNetwork *net, const char *filename);
NeuralNetwork* loadNetwork(const char *filename);

// Fonctions pour le calcul des sorties (forward pass) et de la rétropropagation
void forwardPass(NeuralNetwork *net, double *input);
void backpropagation(NeuralNetwork *net, double *target);

// Fonctions pour générer des spirales (jeu de données)
void generateSpirals(SamplePoint *spiralBlue, SamplePoint *spiralRed, int nbPoints);

// Boucle d'apprentissage
void trainNetwork(NeuralNetwork *net, SamplePoint *dataset, int nbSamples);

#endif