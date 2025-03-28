/**
 * @file neurone.c
 * @brief Fichier des fonctions nécessaires pour la création, la libération, la sauvegarde, le chargement, la propagation et la rétropropagation du réseau de neurones
 * @version 1.1
 * @date 2025-03-23
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "neurone.h"


// Fonction d'activation tanh et sa dérivée
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

// -------------------------------------
// Génération des spirales (jeu de données)
// -------------------------------------
/**
 * @brief Fonction qui génère les spirales bleues et rouges
 * 
 * @param[out] spiralBlue Les points de la spirale bleue
 * @param[out] spiralRed Les points de la spirale rouge
 * @param[in] nbPoints Le nombre de points total
 * @return void
 */
void generateSpirals(SamplePoint *spiralBlue, SamplePoint *spiralRed, int nbPoints) {
    double step = 4.0 * M_PI / (double)(nbPoints); //2(rotation) + (2.0 = WINDOW_WIDTH/NB_POINTS = ORDRE de GRANDEUR) = DISTANCE ENTRE 2 SPIRES 
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


/**
 * @brief Fonction qui génère les roses bleues et rouges
 * 
 * @param[out] spiralBlue Les points de la spirale bleue
 * @param[out] spiralRed Les points de la spirale rouge
 * @param[in] nbPoints nbPoints Le nombre de points total
 * @return void
 */
void generateRoses(SamplePoint *spiralBlue, SamplePoint *spiralRed, int nbPoints) {
    double step = 2.0 * M_PI / nbPoints;

    double scaleBlue = 10.0;  // Radius pour les spirale bleue 
    double scaleRed  = 10.0;  // Radius pour les spirale rouge
    double angleOffset = M_PI / 6;  // Rotation des spirales rouges pour les décaler

    for (int i = 0; i < nbPoints; i++) {
        double t = i * step;

        // Spire des rose bleue (r = cos(3t))
        double r_blue = cos(3 * t);
        spiralBlue[i].x = scaleBlue * r_blue * cos(t);
        spiralBlue[i].y = scaleBlue * r_blue * sin(t);
        spiralBlue[i].target[0] = 1.0;
        spiralBlue[i].target[1] = 0.0;

        // Spire des rose rouge (r = sin(4t)) 
        double r_red = sin(3 * t);
        //decalage 
        double t_shifted = t + angleOffset;
        spiralRed[i].x = scaleRed * r_red * cos(t_shifted);
        spiralRed[i].y = scaleRed * r_red * sin(t_shifted);
        spiralRed[i].target[0] = 0.0;
        spiralRed[i].target[1] = 1.0;
    }
}

/**
 * @brief Fonction qui génère les triangles bleus et rouges
 * 
 * @param[out] spiralBlue Les points du triangle bleue
 * @param[out] spiralRed Les points du triangle rouge
 * @param [in] nbPoints nbPoints Le nombre de points total
 * @return void
 */
void generateTriangle(SamplePoint *spiralBlue, SamplePoint *spiralRed, int nbPoints) {
    //Divise le nombre de points par 2 pour avoir 2 triangles
    int halfPoints = nbPoints / 2;
    // On divise le nombre de points par 3 pour avoir les 3 côtés du triangle  
    int pointsPerEdge = halfPoints / 3;
    //Taille des côtes du triangle
    double outerRadius = 10.0;
    double innerRadius = 5.0;

    // Angles des sommets du triangle
    double angles[3] = { -M_PI / 2, -M_PI / 2 + 2.0 * M_PI / 3.0, -M_PI / 2 + 4.0 * M_PI / 3.0 };

    // Generation du triangle bleu (extérieur)
    for (int i = 0; i < 3; i++) {
        //Calcul les coordonnées des sommets et des côtés du triangle bleu
        double ax = outerRadius * cos(angles[i]);
        double ay = outerRadius * sin(angles[i]);
        double bx = outerRadius * cos(angles[(i + 1) % 3]);
        double by = outerRadius * sin(angles[(i + 1) % 3]);
        //Calcul les coordonnées des points sur du triangle bleu
        for (int j = 0; j < pointsPerEdge; j++) {
            int index = i * pointsPerEdge + j;
            double t = (double)j / (pointsPerEdge - 1);
            spiralBlue[index].x = (1 - t) * ax + t * bx;
            spiralBlue[index].y = (1 - t) * ay + t * by;
            spiralBlue[index].target[0] = 1.0;
            spiralBlue[index].target[1] = 0.0;
        }
    }

    // Generattion du triangle rouge (intérieur)
    for (int i = 0; i < 3; i++) {
        //Calcul les coordonées des sommets et des côtés du triangle rouge
        double ax = innerRadius * cos(angles[i]);
        double ay = innerRadius * sin(angles[i]);
        double bx = innerRadius * cos(angles[(i + 1) % 3]);
        double by = innerRadius * sin(angles[(i + 1) % 3]);
        //Calcul les coordonnées des points du triangle rouge
        for (int j = 0; j < pointsPerEdge; j++) {
            int index = i * pointsPerEdge + j;
            double t = (double)j / (pointsPerEdge - 1);
            spiralRed[index].x = (1 - t) * ax + t * bx;
            spiralRed[index].y = (1 - t) * ay + t * by;
            spiralRed[index].target[0] = 0.0;
            spiralRed[index].target[1] = 1.0;
        }
    }
}


// -------------------------------------
// Création / libération du réseau
// -------------------------------------
/**
 * @brief Crée le réseau de neurones et initialise les poids aléatoirement
 * @param[in] nbLayers 
 * @param[out] layerSizes 
 * @return NeuralNetwork*
 */
NeuralNetwork* createNetwork(int nbLayers, int *layerSizes) {
    //Allocation de la structure de données
    NeuralNetwork *net = (NeuralNetwork*)malloc(sizeof(NeuralNetwork));
    //Initialisation du nombre de couches
    net->nbLayers   = nbLayers;
    //Allocation de la taille des couches
    net->layerSizes = (int*)malloc(nbLayers * sizeof(int));
    for(int i=0; i<nbLayers; i++){
        net->layerSizes[i] = layerSizes[i];
    }

    // Allocation des sorties
    net->outputs = (double**)malloc(nbLayers * sizeof(double*));
    for(int i=0; i<nbLayers; i++){
        net->outputs[i] = (double*)calloc(layerSizes[i], sizeof(double));
    }

    // Allocation des deltas
    net->delta = (double**)malloc(nbLayers * sizeof(double*));
    for(int i=0; i<nbLayers; i++){
        net->delta[i] = (double*)calloc(layerSizes[i], sizeof(double));
    }

    // Allocation des poids 
    net->weights = (double***)malloc(nbLayers * sizeof(double**));
    // La couche d'entrée n'a pas de poids (pas de neurones entrants)
    for(int i=1; i<nbLayers; i++){
        net->weights[i] = (double**)malloc(layerSizes[i] * sizeof(double*));
        for(int j=0; j<layerSizes[i]; j++){
            net->weights[i][j] = (double*)malloc(layerSizes[i-1] * sizeof(double));
            //Initialisation aléatoire
            for(int k=0; k<layerSizes[i-1]; k++){
                net->weights[i][j][k] = randSymetric() * 0.5; 
            }
            
        }
        
    }
    // La couche 0 n'a pas de poids
    net->weights[0] = NULL;
    return net;
}
/**
 * @brief Libère 
 * 
 * @param[out] net réseau de neurones
 * @return void
 */
void freeNetwork(NeuralNetwork *net) {
    if(!net) return;

    // Libère les poids pour chaque couche
    for(int i=1; i<net->nbLayers; i++){
        for(int j=0; j<net->layerSizes[i]; j++){
            free(net->weights[i][j]);
        }
        free(net->weights[i]);
        
    }
    free(net->weights);
    

    // Libère outputs et delta pour chaque couche
    for(int i=0; i<net->nbLayers; i++){
        free(net->outputs[i]);
        free(net->delta[i]);
    }
    free(net->outputs);
    free(net->delta);
    //Liberation du pointeur de taille des couches
    free(net->layerSizes);
    //Liberation de la structure de données du réseau
    free(net);
}

// -------------------------------------
// Sauvegarde / chargement du réseau
// -------------------------------------
/**
 * @brief Sauvegarde du réseau de neurones dans le fichier reseau_sauvegarde.txt
 * 
 * @param[out] net réseau de neurones
 * @param[out] filename nom du fichier
 * @return int retourne 1 si le fichier est sauvegardé sinon 0
 */
int saveNetwork(NeuralNetwork *net, const char *filename) {
    FILE *f = fopen(filename, "w");
    if(!f) {
        printf("Impossible d'ouvrir %s en écriture.\n", filename);
        return 0;
    }
    // On écrit : nbLayers
    fprintf(f, "%d\n", net->nbLayers);
    // On écrit : layerSizes
    for(int i=0; i<net->nbLayers; i++){
        fprintf(f, "%d ", net->layerSizes[i]);
    }
    fprintf(f, "\n");
    // On écrit tous les poids 
    for(int i=1; i<net->nbLayers; i++){
        for(int j=0; j<net->layerSizes[i]; j++){
            for(int k=0; k<net->layerSizes[i-1]; k++){
                fprintf(f, "%.15f ", net->weights[i][j][k]);
            }
            fprintf(f, "\n");
        }
        
        fprintf(f, "\n");
    }

    fclose(f);
    printf("Réseau sauvegardé dans %s\n", filename);
    return 1;
}
/**
 * @brief Charge le réseau de neurones depuis le fichier reseau_sauvegarde.txt
 * 
 * @param[out] filename nom du fichier
 * @return NeuralNetwork* retourne le réseau de neurones
 */
NeuralNetwork* loadNetwork(const char *filename) {
    FILE *f = fopen(filename, "r");
    if(!f){
        printf("Impossible d'ouvrir %s en lecture.\n", filename);
        return NULL;
    }
    int nbL;
    // On ignore le résultat de fscanf pour éviter les warnings
    // (dans un vrai projet, il vaut mieux tester si fscanf(...) == 1 !)
    (void)fscanf(f, "%d", &nbL);

    int *ls = (int*)malloc(nbL * sizeof(int));
    for(int i=0; i<nbL; i++){
        (void)fscanf(f, "%d", &ls[i]);
    }
    // Crée le réseau
    NeuralNetwork *net = createNetwork(nbL, ls);
    free(ls);

    // Relit tous les poids
    for(int i=1; i<net->nbLayers; i++){
        for(int j=0; j<net->layerSizes[i]; j++){
            for(int k=0; k<net->layerSizes[i-1]; k++){
                (void)fscanf(f, "%lf", &net->weights[i][j][k]);
            }
        }
    }
    fclose(f);
    printf("Réseau chargé depuis %s\n", filename);
    return net;
}


// -------------------------------------
// Forward pass
// -------------------------------------
/**
 * @brief Passe avant du réseau de neurones
 * 
 * @param[out] net réseau de neurones
 * @param[out] input tableau des coordonnées x et y
 * @return void
 */
void forwardPass(NeuralNetwork *net, double *input) {
    // La couche d'entrée = input directement
    for(int i=0; i<net->layerSizes[0]; i++){
        net->outputs[0][i] = input[i];
    }

for (int i=1; i<net->nbLayers; i++) {
    double *prev_outputs = net->outputs[i-1];  // Variable locale pour éviter d’accéder au pointeur
    double *cur_outputs = net->outputs[i];
    for (int j=0; j<net->layerSizes[i]; j++) {
        double sum = 0.0;
        double *weights_j = net->weights[i][j];  // Pointer directement sur les poids
        for (int k=0; k<net->layerSizes[i-1]; k++) {
            sum += prev_outputs[k] * weights_j[k];
        }
        //non-linéarité
        cur_outputs[j] = my_tanh(sum);
    }
}

}


// -------------------------------------
// Backpropagation
// -------------------------------------
/**
 * @brief Rétropropagation du réseau de neurones
 * 
 * @param[out] net réseau de neurones
 * @param[out] target tableau des points x et y ciblés
 * @return void
 */
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

    // 3. Mise à jour des poids 
    for(int i=1; i<net->nbLayers; i++){
        for(int j=0; j<net->layerSizes[i]; j++){
            for(int k=0; k<net->layerSizes[i-1]; k++){
                double grad = net->delta[i][j] * net->outputs[i-1][k];
                net->weights[i][j][k] += ETA * grad;
            }
        }
    }
}

// -------------------------------------
// Boucle d'apprentissage principal
// -------------------------------------
/**
 * @brief Entraînement/Apprentissahe du réseau de neurones
 * 
 * @param[out] net réseau de neurones
 * @param[out] dataset jeu de données des spirales
 * @param[int] nbSamples nombre de points total
 * @return void
 */
void trainNetwork(NeuralNetwork *net, SamplePoint *dataset, int nbSamples) {
    //On entraine sur le nombre de fois définie dans types.h par MAX_EPOCHS
    for(int epoch=0; epoch<MAX_EPOCHS; epoch++){
        double maxDelta = 0.0;

        // Parcours de tous les points
        for(int i=0; i<nbSamples; i++){
            double input[2] = { dataset[i].x, dataset[i].y };
            // Passe avant
            forwardPass(net, input);
            // Propagation arrière
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

        if(epoch % 500 == 0){
            printf("Epoch=%d, maxDelta=%.6f\n", epoch, maxDelta);
        }
    }
}
