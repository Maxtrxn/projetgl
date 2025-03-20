#include "affichage.h"

// -------------------------------------
// Affichage SDL de la classification
// -------------------------------------

// Calcul de la couleur en fonction de la sortie [pBleu, pRouge]
void getColorFromOutput(double pBleu, double pRouge, Uint8 *r, Uint8 *g, Uint8 *b) {
    double sum = pBleu + pRouge + 1e-9;  // Évite la division par zéro
    double pb = pBleu * (1.0 / sum);  // Évite la division répétée
    double pr = pRouge * (1.0 / sum);
    *r = (Uint8)(pr * 255.0);
    *g = 0;
    *b = (Uint8)(pb * 255.0);
}


// Dessine la "carte" de classification
// Déclarer une texture globale
SDL_Texture *classificationTexture = NULL;

void initClassificationTexture(SDL_Renderer *renderer) {
    classificationTexture = SDL_CreateTexture(renderer,
                                              SDL_PIXELFORMAT_RGB888, 
                                              SDL_TEXTUREACCESS_STREAMING, 
                                              WINDOW_WIDTH, WINDOW_HEIGHT);
}

void drawClassificationMap(SDL_Renderer *renderer, NeuralNetwork *net) {
    if (!classificationTexture) {
        initClassificationTexture(renderer); // S'assure que la texture existe
    }

    Uint32 *pixels;
    int pitch;
    SDL_LockTexture(classificationTexture, NULL, (void**)&pixels, &pitch);
    pitch /= sizeof(Uint32); // Convertir pitch en nombre de pixels

    for(int py=0; py<WINDOW_HEIGHT; py++){
        for(int px=0; px<WINDOW_WIDTH; px++){
            double x = ((double)px - WINDOW_WIDTH/2.0) / 40.0;
            double y = ((double)py - WINDOW_HEIGHT/2.0) / 40.0;
            double input[2] = {x, y};

            forwardPass(net, input);
            double pBleu  = (net->outputs[net->nbLayers-1][0] + 1.0) / 2.0;
            double pRouge = (net->outputs[net->nbLayers-1][1] + 1.0) / 2.0;

            Uint8 r, g, b;
            getColorFromOutput(pBleu, pRouge, &r, &g, &b);

            // 🟢 Écriture directe dans la mémoire texture
            pixels[py * pitch + px] = (r << 16) | (g << 8) | b;
        }
    }

    SDL_UnlockTexture(classificationTexture); // Débloque l'accès à la texture
    SDL_RenderCopy(renderer, classificationTexture, NULL, NULL); // Affiche d'un coup
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
