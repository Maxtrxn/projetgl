/**
 * @file utils.c
 * @brief Fichier des fonctions utilitaires
 * @version 0.1
 * @date 2025-03-23
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "utils.h"

// -------------------------------------
// Fonctions utilitaires
// -------------------------------------

/**
 * @brief Fonction tanh
 * 
 * @param[in] x paramètre de la fonction
 * @return double 
 */
inline double my_tanh(double x) {
    return tanh(x);
}
/**
 * @brief Fonction dérivée de tanh
 * 
 * @param x 
 * @return double 
 */
inline double tanh_deriv(double x) {
    // Si x est déjà la sortie du neurone = tanh(S), alors la dérivée = 1 - x^2
    return 1.0 - x*x;
}
/**
 * @brief Générateur aléatoire dans [-1, 1]
 * 
 * @return double 
 */

double randSymetric() {
    return 2.0 * ((double)rand()/(double)RAND_MAX) - 1.0;
}
