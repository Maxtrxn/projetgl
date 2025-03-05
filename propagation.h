#ifndef PROPAGATION_H
#define PROPAGATION_H

#include "network.h"
#include "types.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Passes avant et arrière
void forwardPass(NeuralNetwork *net, double *input);
void backpropagation(NeuralNetwork *net, double *target);

#endif // PROPAGATION_H