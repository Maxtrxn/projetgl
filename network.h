

NeuralNetwork* createNetwork(int nbLayers, int *layerSizes);

void freeNetwork(NeuralNetwork *net);

int saveNetwork(NeuralNetwork *net, const char *filename);

NeuralNetwork* loadNetwork(const char *filename);