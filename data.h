#ifndef DATA_H
#define DATA_H

#include "types.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "utils.h"
#include "network.h"
#include "types.h"
#include "propagation.h"

// Génération des spirales (jeu de données)
void generateSpirals(SamplePoint *spiralBlue, SamplePoint *spiralRed, int nbPoints);

// Boucle d'apprentissage principal
void trainNetwork(NeuralNetwork *net, SamplePoint *dataset, int nbSamples);

#endif // DATA_H