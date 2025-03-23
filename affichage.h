/**
 * @file affichage.h
 * @author your name (you@domain.com)
 * @brief Fichier header de affichage.c
 * @version 0.1
 * @date 2025-03-23
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "types.h"
#include "neurone.h"

extern SDL_Texture *classificationTexture;


void getColorFromOutput(double pBleu, double pRouge, Uint8 *r, Uint8 *g, Uint8 *b);
void drawClassificationMap(SDL_Renderer *renderer, NeuralNetwork *net);
void drawSamples(SDL_Renderer *renderer, SamplePoint *data, int n, SDL_Color color);

#endif 