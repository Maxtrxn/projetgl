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


// Parametres du réseau
static int layerSizes[] = {2,15,15,2}; 
static int nbLayers = 4;  


int main(int argc, char* argv[]);

#endif
