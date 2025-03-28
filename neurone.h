/**
 * @file neurone.h
 * @author your name (you@domain.com)
 * @brief Fichier header de neurone.c
 * @version 0.1
 * @date 2025-03-23
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef NEURONE_H
#define NEURONE_H

#include "types.h"
//#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

/**
 * @define M_PI Définition de la constante PI
 * 
 */
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

// Fonctions pour générer des spirales (jeu de données)
void generateSpirals(SamplePoint *spiralBlue, SamplePoint *spiralRed, int nbPoints);
void generateTriangle(SamplePoint *spiralBlue, SamplePoint *spiralRed, int nbPoints);
void generateRoses(SamplePoint *spiralBlue, SamplePoint *spiralRed, int nbPoints);

// Fonctions de gestion du réseau
NeuralNetwork* createNetwork(int nbLayers, int *layerSizes);
void freeNetwork(NeuralNetwork *net);

// Fonctions de sauvegarde et chargement du réseau
int saveNetwork(NeuralNetwork *net, const char *filename);
NeuralNetwork* loadNetwork(const char *filename);

// Fonctions pour le calcul des sorties (forward pass) et de la rétropropagation
void forwardPass(NeuralNetwork *net, double *input);
void backpropagation(NeuralNetwork *net, double *target);


// Boucle d'apprentissage
void trainNetwork(NeuralNetwork *net, SamplePoint *dataset, int nbSamples);

#endif