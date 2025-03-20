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