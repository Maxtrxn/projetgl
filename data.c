#include "types.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "utils.h"
#include "network.h"
#include "types.h"
#include "propagation.h"
#include "data.h"

/*Version sans get et set  A SUPP
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
*/
// -------------------------------------
// Génération des spirales (jeu de données)
// -------------------------------------
void generateSpirals(SamplePoint *spiralBlue, SamplePoint *spiralRed, int nbPoints) {
    double step = 4.0 * M_PI / (double)(nbPoints);
    for(int i=0; i<nbPoints; i++){
        double t =  i * step  ;
        // Spirale bleue: (t cos t, t sin t)
        setX(&spiralBlue[i], t * cos(t));
        setY(&spiralBlue[i], t * sin(t));
        double targetBlue[2] = {1.0, 0.0};
        setTarget(&spiralBlue[i], targetBlue);

        // Spirale rouge: (-t cos t, -t sin t)
        setX(&spiralRed[i], -t * cos(t));
        setY(&spiralRed[i], -t * sin(t));
        double targetRed[2] = {0.0, 1.0};
        setTarget(&spiralRed[i], targetRed);
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
            double input[2] = { getX(&dataset[i]), getY(&dataset[i]) };
            // Forward
            forwardPass(net, input);
            // Backprop
            backpropagation(net, getTarget(&dataset[i]));
            // On suit le maxDelta sur la couche de sortie
            for(int j=0; j<getLayerSizes(net)[getNbLayers(net)-1]; j++){
                double d = fabs(getDelta(net)[getNbLayers(net)-1][j]);
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