// -------------------------------------
// Constantes et paramètres
// -------------------------------------

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 1000

// Paramètres de l'apprentissage
#define ETA             0.0001 // Taux d'apprentissage (epsilon)
#define MAX_EPOCHS     5000    // Nombre max d'itérations
#define MIN_ERROR      0.00002  // Seuil d'arrêt sur l'erreur (delta)

// Nombre de points par spirale
#define NB_POINTS_SPIRALE  500

// Ici, on fixe un petit réseau pour l'exemple : 2 entrées, 1 couche cachée de 6 neurones, 2 neurones en sortie
static int layerSizes[] = {2, 8,12,8, 2}; 
static int nbLayers = 5;  // 3 couches = entrée + 1 cachée + sortie

// -------------------------------------
// Structures de données
// -------------------------------------


// Représentation d'un réseau entièrement connecté
typedef struct {
    int nbLayers;      // Nombre total de couches
    int *layerSizes;   // Tableau contenant la taille de chaque couche
                       // ex: layerSizes[0] = 2, layerSizes[1] = 6, layerSizes[2] = 2, ...

    // weights[i][j][k] = poids reliant le neurone k de la couche (i-1)
    //                    au neurone j de la couche i
    double ***weights; 
    
    // biases[i][j] = biais du neurone j de la couche i
    double **biases;

    // outputs[i][j] = sortie du neurone j de la couche i
    double **outputs;

    // delta[i][j] = delta (erreur) du neurone j de la couche i
    double **delta;

} NeuralNetwork;

// Contenant x, y et la classe cible (rouge ou bleu) sous forme d’un one-hot [1, 0] ou [0, 1]
typedef struct {
    double x;
    double y;
    double target[2]; 
} SamplePoint;

// Getters and setters for NeuralNetwork structure

int getNbLayers(NeuralNetwork *nn);

void setNbLayers(NeuralNetwork *nn, int nbLayers);

int* getLayerSizes(NeuralNetwork *nn);

void setLayerSizes(NeuralNetwork *nn, int *layerSizes);

double*** getWeights(NeuralNetwork *nn);

void setWeights(NeuralNetwork *nn, double ***weights);

double** getBiases(NeuralNetwork *nn);

void setBiases(NeuralNetwork *nn, double **biases);

double** getOutputs(NeuralNetwork *nn);

void setOutputs(NeuralNetwork *nn, double **outputs);

double** getDelta(NeuralNetwork *nn);

void setDelta(NeuralNetwork *nn, double **delta);

// Getters and setters for SamplePoint structure

double getX(SamplePoint *sp);
void setX(SamplePoint *sp, double x);

double getY(SamplePoint *sp);

void setY(SamplePoint *sp, double y);

double* getTarget(SamplePoint *sp);

void setTarget(SamplePoint *sp, double target[2]);