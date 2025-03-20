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
static int layerSizes[] = {2, 48, 8, 24, 8, 32, 2};
static int nbLayers = sizeof(layerSizes) / sizeof(int);  


int main(int argc, char* argv[]);

#endif
