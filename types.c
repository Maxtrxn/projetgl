#include <stdio.h>
#include <stdlib.h>
#include "types.h"

// Getters and setters for NeuralNetwork structure

int getNbLayers(NeuralNetwork *nn) {
    return nn->nbLayers;
}

void setNbLayers(NeuralNetwork *nn, int nbLayers) {
    nn->nbLayers = nbLayers;
}

int* getLayerSizes(NeuralNetwork *nn) {
    return nn->layerSizes;
}

void setLayerSizes(NeuralNetwork *nn, int *layerSizes) {
    nn->layerSizes = layerSizes;
}

double*** getWeights(NeuralNetwork *nn) {
    return nn->weights;
}

void setWeights(NeuralNetwork *nn, double ***weights) {
    nn->weights = weights;
}

double** getBiases(NeuralNetwork *nn) {
    return nn->biases;
}

void setBiases(NeuralNetwork *nn, double **biases) {
    nn->biases = biases;
}

double** getOutputs(NeuralNetwork *nn) {
    return nn->outputs;
}

void setOutputs(NeuralNetwork *nn, double **outputs) {
    nn->outputs = outputs;
}

double** getDelta(NeuralNetwork *nn) {
    return nn->delta;
}

void setDelta(NeuralNetwork *nn, double **delta) {
    nn->delta = delta;
}

// Getters and setters for SamplePoint structure

double getX(SamplePoint *sp) {
    return sp->x;
}

void setX(SamplePoint *sp, double x) {
    sp->x = x;
}

double getY(SamplePoint *sp) {
    return sp->y;
}

void setY(SamplePoint *sp, double y) {
    sp->y = y;
}

double* getTarget(SamplePoint *sp) {
    return sp->target;
}

void setTarget(SamplePoint *sp, double target[2]) {
    sp->target[0] = target[0];
    sp->target[1] = target[1];
}