#include "main.h"


// Fonction tanh et sa dérivée
static inline double my_tanh(double x) {
    return tanh(x);
}
static inline double tanh_deriv(double x) {
    // Si x est déjà la sortie du neurone = tanh(S), alors la dérivée = 1 - x^2
    return 1.0 - x*x;
}


// -------------------------------------
// MAIN
// -------------------------------------
int main(int argc, char **argv) {
    (void)argc; // on ignore les paramètres pour éviter le warning
    (void)argv;

    srand((unsigned int)time(NULL));

    // 1. Génération des spirales
    SamplePoint spiralBlue[NB_POINTS_SPIRALE];
    SamplePoint spiralRed[NB_POINTS_SPIRALE];
    generateSpirals(spiralBlue, spiralRed, NB_POINTS_SPIRALE);

    // 2. Création du réseau
    NeuralNetwork *net = createNetwork(nbLayers, layerSizes);

    // 3. Initialisation de la SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Erreur SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window *window = SDL_CreateWindow("Classification Spirales - Reseau de Neurones",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if(!window){
        fprintf(stderr, "Erreur CreateWindow: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        fprintf(stderr, "Erreur CreateRenderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int running = 1;
    int needRedraw = 1;
    while(running) {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT){
                running = 0;
            }
            else if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_ESCAPE || e.key.keysym.sym == SDLK_q){
                    running = 0;
                }
                else if(e.key.keysym.sym == SDLK_t){
                    printf("Lancement de l'apprentissage...\n");
                    // On combine les deux spirales dans un seul dataset
                    
                        SamplePoint dataset[2*NB_POINTS_SPIRALE];
                        for(int i=0; i<NB_POINTS_SPIRALE; i++){
                            dataset[i] = spiralBlue[i];
                        }
                        for(int i=0; i<NB_POINTS_SPIRALE; i++){
                            dataset[NB_POINTS_SPIRALE + i] = spiralRed[i];
                        }
                        for(int i =0; i< 3; i++){
                        printf("entrainement n° %d\n",i+1);
                        trainNetwork(net, dataset, 2*NB_POINTS_SPIRALE);
                        
                        needRedraw = 1;
                        
                         if(needRedraw){
                            // 1) On dessine la carte de classification
                            drawClassificationMap(renderer, net);
                            // 2) On dessine les points (spirales)
                            SDL_Color blue = {0, 0, 0xFF, 0xFF};
                            SDL_Color red  = {0xFF, 0, 0, 0xFF};
                            drawSamples(renderer, spiralBlue, NB_POINTS_SPIRALE, blue);
                            drawSamples(renderer, spiralRed,  NB_POINTS_SPIRALE, red);
                            SDL_RenderPresent(renderer);
                            needRedraw = 0;
                        }
                    }
                }
                else if(e.key.keysym.sym == SDLK_s){
                    saveNetwork(net, "reseau_sauvegarde.txt");
                }
                else if(e.key.keysym.sym == SDLK_l){
                    NeuralNetwork *tmp = loadNetwork("reseau_sauvegarde.txt");
                    if(tmp){
                        freeNetwork(net);
                        net = tmp;
                    }
                    needRedraw = 1;
                }
            }
        }

        if(needRedraw){
            // 1) On dessine la carte de classification
            drawClassificationMap(renderer, net);
            // 2) On dessine les points (spirales)
            SDL_Color blue = {0, 0, 0xFF, 0xFF};
            SDL_Color red  = {0xFF, 0, 0, 0xFF};
            drawSamples(renderer, spiralBlue, NB_POINTS_SPIRALE, blue);
            drawSamples(renderer, spiralRed,  NB_POINTS_SPIRALE, red);
            SDL_RenderPresent(renderer);
            needRedraw = 0;
        }

        SDL_Delay(10);
    }

    // Nettoyage
    freeNetwork(net);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

