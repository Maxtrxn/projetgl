#include "neuralNetwork.h"
#include "types.h"
#include "utils.h"
#include "propagation.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdlib.h>


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