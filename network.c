#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "network.h"
#include "types.h"

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