#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "propagation.h"
#include "data.h"
#include "types.h"
#include "network.h"
#include "utils.h"
#include "draw.h"

// -------------------------------------
// Affichage SDL de la classification
// -------------------------------------

// Calcul de la couleur en fonction de la sortie [pBleu, pRouge]
static void getColorFromOutput(double pBleu, double pRouge, Uint8 *r, Uint8 *g, Uint8 *b) {
    double sum = pBleu + pRouge;
    if(sum < 1e-9) sum = 1e-9;
    double pb = pBleu / sum;
    double pr = pRouge / sum;

    // Couleur = pb * bleu + pr * rouge
    double db = pb * 255.0; 
    double dr = pr * 255.0; 
    *r = (Uint8)dr;
    *g = 0;
    *b = (Uint8)db;
}

// Dessine la "carte" de classification
void drawClassificationMap(SDL_Renderer *renderer, NeuralNetwork *net) {
    for(int px=0; px<WINDOW_WIDTH; px++){
        for(int py=0; py<WINDOW_HEIGHT; py++){
            double x = ( (double)px - WINDOW_WIDTH/2.0 ) / 40.0;
            double y = ( (double)py - WINDOW_HEIGHT/2.0 ) / 40.0;
            double input[2] = {x, y};
            forwardPass(net, input);

            double pBleu  = net->outputs[net->nbLayers-1][0];
            double pRouge = net->outputs[net->nbLayers-1][1];

            // Tanh renvoie dans [-1,1], on ramène dans [0,1]
            pBleu  = (pBleu  + 1.0)/2.0;
            pRouge = (pRouge + 1.0)/2.0;

            Uint8 r, g, b;
            getColorFromOutput(pBleu, pRouge, &r, &g, &b);
            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            SDL_RenderDrawPoint(renderer, px, py);
        }
    }
}

// Dessine les points
void drawSamples(SDL_Renderer *renderer, SamplePoint *data, int n, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    for(int i=0; i<n; i++){
        int px = (int)( data[i].x * 40.0 + WINDOW_WIDTH/2.0 );
        int py = (int)( data[i].y * 40.0 + WINDOW_HEIGHT/2.0 );
        SDL_Rect rect = { px-1, py-1, 3, 3 };
        SDL_RenderFillRect(renderer, &rect);
    }
}