#ifndef DRAW_H
#define DRAW_H

#include <SDL2/SDL.h>
#include "propagation.h"
#include "data.h"
#include "types.h"
#include "network.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Calcul de la couleur en fonction de la sortie [pBleu, pRouge]
static void getColorFromOutput(double pBleu, double pRouge, Uint8 *r, Uint8 *g, Uint8 *b);

// Dessine la "carte" de classification
void drawClassificationMap(SDL_Renderer *renderer, NeuralNetwork *net);

// Dessine les points
void drawSamples(SDL_Renderer *renderer, SamplePoint *data, int n, SDL_Color color);

#endif // DRAW_H