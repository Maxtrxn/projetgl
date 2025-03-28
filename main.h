/**
 * @file main.h
 * @brief Fichier header de main.c
 * @version 1.0
 * @date 2025-03-28
 * 
 */

#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "types.h"
//#include "utils.h"
#include "neurone.h"
#include "affichage.h"


// Parametres du réseau1
/**
 * @define LAYER_SIZES Taille des couches du réseau de neurones
 * 
 */
static int layerSizes[] = {2, 48, 8, 24, 8, 32, 2};
/**
 * @define NB_LAYERS Nombre de couches du réseau de neurones
 * 
 */
static int nbLayers = sizeof(layerSizes) / sizeof(int);  


int main(int argc, char* argv[]);

#endif
