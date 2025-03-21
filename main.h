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
static int layerSizes[] = {2,20,10,20,2}; 
static int nbLayers = 5;  


int main(int argc, char* argv[]);

#endif
