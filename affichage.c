/**
 * @file affichage.c
 * @brief  Fichier des fonctions nécessaires pour l'affichage en SDL du projet
 * @version 0.1
 * @date 2025-03-23
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "affichage.h"

// -------------------------------------
// Affichage SDL de la classification
// -------------------------------------


/**
 * @brief Calcul de la couleur en fonction de la sortie [pBleu, pRouge]
 * 
 * @param[in] pBleu 
 * @param[in] pRouge 
 * @param[out] r 
 * @param[out] g 
 * @param[out] b 
 * @return void
 */
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
/**
 * @brief Initialisation du rendu de la fenetre
 * 
 * @param[out] renderer Pointeur sur le rendu de la fenetre
 * @return void
 */
void initClassificationTexture(SDL_Renderer *renderer) {
    classificationTexture = SDL_CreateTexture(renderer,
                                              SDL_PIXELFORMAT_RGB888, 
                                              SDL_TEXTUREACCESS_STREAMING, 
                                              WINDOW_WIDTH, WINDOW_HEIGHT);
}
/**
 * @brief Dessine la carte de classification du réseau de neurones
 * 
 * @param[out] renderer Pointeur sur le rendu de la fenetre
 * @param[out] net Réseau de neurones
 * @return void
 */
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
            //Passe avant pour générer les poids et initialiser la classification
            forwardPass(net, input);
            double pBleu  = (net->outputs[net->nbLayers-1][0] + 1.0) / 2.0;
            double pRouge = (net->outputs[net->nbLayers-1][1] + 1.0) / 2.0;
            //Gestion des couleurs
            Uint8 r, g, b;
            getColorFromOutput(pBleu, pRouge, &r, &g, &b);

            // 🟢 Écriture directe dans la mémoire texture
            pixels[py * pitch + px] = (r << 16) | (g << 8) | b;
        }
    }

    SDL_UnlockTexture(classificationTexture); // Débloque l'accès à la texture
    SDL_RenderCopy(renderer, classificationTexture, NULL, NULL); // Affiche d'un coup
}


/**
 * @brief Dessine les points des spirales dans la fenetre
 * 
 * @param[out] renderer Pointeur sur le rendu de la fenetre
 * @param[out] data Pointeur sur les points
 * @param[in] n Nombre de points
 * @param[in] color Couleur des points
 * @return void
 */
void drawSamples(SDL_Renderer *renderer, SamplePoint *data, int n, SDL_Color color) {
    // Calcul le rendu de la couleur du point
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    //Boucle pour chaque point
    for(int i=0; i<n; i++){
        int px = (int)( data[i].x * 40.0 + WINDOW_WIDTH/2.0 );
        int py = (int)( data[i].y * 40.0 + WINDOW_HEIGHT/2.0 );
        //Dessin du point, rectangle de 3x3
        SDL_Rect rect = { px-1, py-1, 3, 3 };
        //Remplissage du point par sa couleur obtenue au rendu
        SDL_RenderFillRect(renderer, &rect);
    }
}
