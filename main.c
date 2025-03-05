#define _USE_MATH_DEFINES   // Pour que M_PI soit défini
#include <math.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "ubench.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

// -------------------------------------
// Constantes et paramètres
// -------------------------------------

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 1000

// Paramètres de l'apprentissage
#define ETA             0.0001 // Taux d'apprentissage (epsilon)
#define MAX_EPOCHS     5000    // Nombre max d'itérations
#define MIN_ERROR      0.00002  // Seuil d'arrêt sur l'erreur (delta)

// Nombre de points par spirale
#define NB_POINTS_SPIRALE  500

// Ici, on fixe un petit réseau pour l'exemple : 2 entrées, 1 couche cachée de 6 neurones, 2 neurones en sortie
static int layerSizes[] = {2, 8,12,8, 2}; 
static int nbLayers = 5;  // 3 couches = entrée + 1 cachée + sortie

// -------------------------------------
// Structures de données
// -------------------------------------

// Représentation d'un réseau entièrement connecté
typedef struct {
    int nbLayers;      // Nombre total de couches
    int *layerSizes;   // Tableau contenant la taille de chaque couche
                       // ex: layerSizes[0] = 2, layerSizes[1] = 6, layerSizes[2] = 2, ...

    // weights[i][j][k] = poids reliant le neurone k de la couche (i-1)
    //                    au neurone j de la couche i
    double ***weights; 
    
    // biases[i][j] = biais du neurone j de la couche i
    double **biases;

    // outputs[i][j] = sortie du neurone j de la couche i
    double **outputs;

    // delta[i][j] = delta (erreur) du neurone j de la couche i
    double **delta;

} NeuralNetwork;

// Contenant x, y et la classe cible (rouge ou bleu) sous forme d’un one-hot [1, 0] ou [0, 1]
typedef struct {
    double x;
    double y;
    double target[2]; 
} SamplePoint;

// -------------------------------------
// Fonctions utilitaires
// -------------------------------------

// Fonction tanh et sa dérivée
static inline double my_tanh(double x) {
    return tanh(x);
}
static inline double tanh_deriv(double x) {
    // Si x est déjà la sortie du neurone = tanh(S), alors la dérivée = 1 - x^2
    return 1.0 - x*x;
}

// Générateur aléatoire dans [-1, 1]
static double randSymetric() {
    return 2.0 * ((double)rand()/(double)RAND_MAX) - 1.0;
}

// -------------------------------------
// Création / libération du réseau
// -------------------------------------

NeuralNetwork* createNetwork(int nbLayers, int *layerSizes) {
    NeuralNetwork *net = (NeuralNetwork*)malloc(sizeof(NeuralNetwork));
    net->nbLayers   = nbLayers;
    net->layerSizes = (int*)malloc(nbLayers * sizeof(int));
    for(int i=0; i<nbLayers; i++){
        net->layerSizes[i] = layerSizes[i];
    }

    // Allocation des sorties
    net->outputs = (double**)malloc(nbLayers * sizeof(double*));
    for(int i=0; i<nbLayers; i++){
        net->outputs[i] = (double*)calloc(layerSizes[i], sizeof(double));
    }

    // Allocation des deltas
    net->delta = (double**)malloc(nbLayers * sizeof(double*));
    for(int i=0; i<nbLayers; i++){
        net->delta[i] = (double*)calloc(layerSizes[i], sizeof(double));
    }

    // Allocation des poids et biais
    net->weights = (double***)malloc(nbLayers * sizeof(double**));
    net->biases  = (double**)malloc(nbLayers * sizeof(double*));
    // La couche d'entrée n'a pas de poids (pas de neurones entrants)
    for(int i=1; i<nbLayers; i++){
        net->weights[i] = (double**)malloc(layerSizes[i] * sizeof(double*));
        for(int j=0; j<layerSizes[i]; j++){
            net->weights[i][j] = (double*)malloc(layerSizes[i-1] * sizeof(double));
            // Initialisation aléatoire
            for(int k=0; k<layerSizes[i-1]; k++){
                net->weights[i][j][k] = randSymetric() * 0.5; 
            }
        }
        net->biases[i] = (double*)malloc(layerSizes[i] * sizeof(double));
        for(int j=0; j<layerSizes[i]; j++){
            net->biases[i][j] = randSymetric() * 0.5;
        }
    }
    // La couche 0 n'a pas de poids/biais
    net->weights[0] = NULL;
    net->biases[0]  = NULL;

    return net;
}

void freeNetwork(NeuralNetwork *net) {
    if(!net) return;

    // Libère les poids
    for(int i=1; i<net->nbLayers; i++){
        for(int j=0; j<net->layerSizes[i]; j++){
            free(net->weights[i][j]);
        }
        free(net->weights[i]);
        free(net->biases[i]);
    }
    free(net->weights);
    free(net->biases);

    // Libère outputs et delta
    for(int i=0; i<net->nbLayers; i++){
        free(net->outputs[i]);
        free(net->delta[i]);
    }
    free(net->outputs);
    free(net->delta);

    free(net->layerSizes);
    free(net);
}

// -------------------------------------
// Sauvegarde / chargement du réseau
// -------------------------------------

int saveNetwork(NeuralNetwork *net, const char *filename) {
    FILE *f = fopen(filename, "w");
    if(!f) {
        printf("Impossible d'ouvrir %s en écriture.\n", filename);
        return 0;
    }
    // On écrit : nbLayers
    fprintf(f, "%d\n", net->nbLayers);
    // On écrit : layerSizes
    for(int i=0; i<net->nbLayers; i++){
        fprintf(f, "%d ", net->layerSizes[i]);
    }
    fprintf(f, "\n");
    // On écrit tous les poids / biais
    for(int i=1; i<net->nbLayers; i++){
        for(int j=0; j<net->layerSizes[i]; j++){
            for(int k=0; k<net->layerSizes[i-1]; k++){
                fprintf(f, "%.15f ", net->weights[i][j][k]);
            }
            fprintf(f, "\n");
        }
        for(int j=0; j<net->layerSizes[i]; j++){
            fprintf(f, "%.15f ", net->biases[i][j]);
        }
        fprintf(f, "\n");
    }

    fclose(f);
    printf("Réseau sauvegardé dans %s\n", filename);
    return 1;
}

NeuralNetwork* loadNetwork(const char *filename) {
    FILE *f = fopen(filename, "r");
    if(!f){
        printf("Impossible d'ouvrir %s en lecture.\n", filename);
        return NULL;
    }
    int nbL;
    // On ignore le résultat de fscanf pour éviter les warnings
    // (dans un vrai projet, il vaut mieux tester si fscanf(...) == 1 !)
    (void)fscanf(f, "%d", &nbL);

    int *ls = (int*)malloc(nbL * sizeof(int));
    for(int i=0; i<nbL; i++){
        (void)fscanf(f, "%d", &ls[i]);
    }
    // Crée le réseau
    NeuralNetwork *net = createNetwork(nbL, ls);
    free(ls);

    // Relit tous les poids/biais
    for(int i=1; i<net->nbLayers; i++){
        for(int j=0; j<net->layerSizes[i]; j++){
            for(int k=0; k<net->layerSizes[i-1]; k++){
                (void)fscanf(f, "%lf", &net->weights[i][j][k]);
            }
        }
        for(int j=0; j<net->layerSizes[i]; j++){
            (void)fscanf(f, "%lf", &net->biases[i][j]);
        }
    }
    fclose(f);
    printf("Réseau chargé depuis %s\n", filename);
    return net;
}

// -------------------------------------
// Forward pass
// -------------------------------------
void forwardPass(NeuralNetwork *net, double *input) {
    // La couche d'entrée = input directement
    for(int i=0; i<net->layerSizes[0]; i++){
        net->outputs[0][i] = input[i];
    }

    // Pour chaque couche cachée et la couche de sortie
    for(int i=1; i<net->nbLayers; i++){
        for(int j=0; j<net->layerSizes[i]; j++){
            double sum = 0.0;
            // Somme pondérée des sorties de la couche précédente
            for(int k=0; k<net->layerSizes[i-1]; k++){
                sum += net->outputs[i-1][k] * net->weights[i][j][k];
            }
            // Ajout du biais
            sum += net->biases[i][j];
            // Activation
            net->outputs[i][j] = my_tanh(sum);
        }
    }
}

// -------------------------------------
// Backpropagation
// -------------------------------------
void backpropagation(NeuralNetwork *net, double *target) {
    int L = net->nbLayers - 1; // index de la dernière couche

    // 1. Calcul du delta pour la couche de sortie
    for(int j=0; j<net->layerSizes[L]; j++){
        double out = net->outputs[L][j];
        // delta = (1 - out^2) * (target[j] - out)
        net->delta[L][j] = tanh_deriv(out) * (target[j] - out);
    }

    // 2. Rétropropagation pour les couches cachées
    for(int i=L-1; i>=1; i--){
        for(int j=0; j<net->layerSizes[i]; j++){
            double out = net->outputs[i][j];
            double sum = 0.0;
            // On somme sur tous les neurones de la couche i+1
            for(int k=0; k<net->layerSizes[i+1]; k++){
                sum += net->delta[i+1][k] * net->weights[i+1][k][j];
            }
            net->delta[i][j] = tanh_deriv(out) * sum;
        }
    }

    // 3. Mise à jour des poids / biais
    for(int i=1; i<net->nbLayers; i++){
        for(int j=0; j<net->layerSizes[i]; j++){
            for(int k=0; k<net->layerSizes[i-1]; k++){
                double grad = net->delta[i][j] * net->outputs[i-1][k];
                net->weights[i][j][k] += ETA * grad;
            }
            net->biases[i][j] += ETA * net->delta[i][j];
        }
    }
}

// -------------------------------------
// Génération des spirales (jeu de données)
// -------------------------------------
void generateSpirals(SamplePoint *spiralBlue, SamplePoint *spiralRed, int nbPoints) {
    double step = 4.0 * M_PI / (double)(nbPoints);
    for(int i=0; i<nbPoints; i++){
        double t =  i * step  ;
        // Spirale bleue: (t cos t, t sin t)
        spiralBlue[i].x = t * cos(t);
        spiralBlue[i].y = t * sin(t);
        // Cible = [1, 0] => bleu
        spiralBlue[i].target[0] = 1.0;
        spiralBlue[i].target[1] = 0.0;

        // Spirale rouge: (-t cos t, -t sin t)
        spiralRed[i].x = -t * cos(t);
        spiralRed[i].y = -t * sin(t);
        // Cible = [0, 1] => rouge
        spiralRed[i].target[0] = 0.0;
        spiralRed[i].target[1] = 1.0;
    }
}

// -------------------------------------
// Boucle d'apprentissage principal
// -------------------------------------
void trainNetwork(NeuralNetwork *net, SamplePoint *dataset, int nbSamples) {
    for(int epoch=0; epoch<MAX_EPOCHS; epoch++){
        double maxDelta = 0.0;

        // Parcours de tous les échantillons
        for(int i=0; i<nbSamples; i++){
            double input[2] = { dataset[i].x, dataset[i].y };
            // Forward
            forwardPass(net, input);
            // Backprop
            backpropagation(net, dataset[i].target);
            // On suit le maxDelta sur la couche de sortie
            for(int j=0; j<net->layerSizes[net->nbLayers-1]; j++){
                double d = fabs(net->delta[net->nbLayers-1][j]);
                if(d>maxDelta) maxDelta = d;
            }
        }



        if(maxDelta < MIN_ERROR){
            printf("Convergence atteinte a l'epoque %d, maxDelta=%.6f\n", epoch, maxDelta);
            break;
        }

        if(epoch % 1000 == 0){
            printf("Epoch=%d, maxDelta=%.6f\n", epoch, maxDelta);
        }
    }
}

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

// -------------------------------------
// MAIN
// -------------------------------------
int main(int argc, char **argv) {
    (void)argc; // on ignore les paramètres pour éviter le warning
    (void)argv;
    
    Benchmark bench;  // Déclare une variable pour stocker les informations de benchmark
    Benchmark bench2;

    srand((unsigned int)time(NULL));

    // 1. Génération des spirales
    SamplePoint spiralBlue[NB_POINTS_SPIRALE];
    SamplePoint spiralRed[NB_POINTS_SPIRALE];
    generateSpirals(spiralBlue, spiralRed, NB_POINTS_SPIRALE);

    // 2. Création du réseau
    NeuralNetwork *net = createNetwork(nbLayers, layerSizes);

    // 3. Initialisation de la SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Erreur SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window *window = SDL_CreateWindow("Classification Spirales - Reseau de Neurones",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if(!window){
        fprintf(stderr, "Erreur CreateWindow: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        fprintf(stderr, "Erreur CreateRenderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int running = 1;
    int needRedraw = 1;
    while(running) {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT){ // si on ferme l'application, on sort de la boucle 
                running = 0;
            }
            else if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_ESCAPE || e.key.keysym.sym == SDLK_q){ // si on appuie sur echap ou q on quitte l'application
                    running = 0;
                }
                else if(e.key.keysym.sym == SDLK_t){ // si on appuie sur t
                    startBenchmark(&bench);  // Démarre le chronomètre (pour test de temps)
                
                    printf("Lancement de l'apprentissage...\n");
                    // On combine les deux spirales dans un seul dataset
                    
                    SamplePoint dataset[2*NB_POINTS_SPIRALE];
                    for(int i=0; i<NB_POINTS_SPIRALE; i++){
                        dataset[i] = spiralBlue[i];
                    }
                    for(int i=0; i<NB_POINTS_SPIRALE; i++){
                        dataset[NB_POINTS_SPIRALE + i] = spiralRed[i];
                    }

                    //on commence les entrainements
                    for(int i =0; i< 3; i++){
                        startBenchmark(&bench2);  // Démarre le chronomètre (pour test de temps)
                        printf("entrainement n° %d\n",i+1);
                        trainNetwork(net, dataset, 2*NB_POINTS_SPIRALE);
                        
                        needRedraw = 1;
                        
                         if(needRedraw){
                            // 1) On dessine la carte de classification
                            drawClassificationMap(renderer, net);
                            // 2) On dessine les points (spirales)
                            SDL_Color blue = {0, 0, 0xFF, 0xFF};
                            SDL_Color red  = {0xFF, 0, 0, 0xFF};
                            drawSamples(renderer, spiralBlue, NB_POINTS_SPIRALE, blue);
                            drawSamples(renderer, spiralRed,  NB_POINTS_SPIRALE, red);
                            SDL_RenderPresent(renderer);
                            needRedraw = 0;
                        }
                        printf("Fin de l'entraînement\n");
                        // Arrêter le benchmark et afficher les résultats
                        stopBenchmark(&bench2, "Test de chaque entrainement");
                        
                    }
                    stopBenchmark(&bench, "Test de l'appentissage");

                }
                else if(e.key.keysym.sym == SDLK_s){
                    saveNetwork(net, "reseau_sauvegarde.txt");
                }
                else if(e.key.keysym.sym == SDLK_l){
                    NeuralNetwork *tmp = loadNetwork("reseau_sauvegarde.txt");
                    if(tmp){
                        freeNetwork(net);
                        net = tmp;
                    }
                    needRedraw = 1;
                }
            }
        }

        if(needRedraw){
            // 1) On dessine la carte de classification
            drawClassificationMap(renderer, net);
            // 2) On dessine les points (spirales)
            SDL_Color blue = {0, 0, 0xFF, 0xFF};
            SDL_Color red  = {0xFF, 0, 0, 0xFF};
            drawSamples(renderer, spiralBlue, NB_POINTS_SPIRALE, blue);
            drawSamples(renderer, spiralRed,  NB_POINTS_SPIRALE, red);
            SDL_RenderPresent(renderer);
            needRedraw = 0;
        }

        SDL_Delay(10);
    }
    
    // Nettoyage
    freeNetwork(net);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

