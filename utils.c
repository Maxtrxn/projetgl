// Fonction tanh et sa dérivée
static inline double my_tanh(double x) {
    return tanh(x);
}
static inline double tanh_deriv(double x) {
    // Si x est déjà la sortie du neurone = tanh(S), alors la dérivée = 1 - x^2
    return 1.0 - x*x;
}

// Générateur aléatoire dans [-1, 1]
static double randSymetric() {
    return 2.0 * ((double)rand()/(double)RAND_MAX) - 1.0;
}