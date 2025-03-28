/**
 * @file main.c
 * @author { Zoé Arthapignet, Mélie BORDAGE, Nicolas BOUILLON, Zack HEBERT}
 * @brief Fichier main du projet
 * @version 1.1
 * @date 2025-03-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */

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
/**
 * @brief Fonction main du projet
 * 
 * @param[in] argc 
 * @param[in] argv 
 * @return int 
 */
int main(int argc, char **argv) {
    (void)argc; // on ignore les paramètres pour éviter le warning
    (void)argv;

    // Initialisation du générateur aléatoire
    srand((unsigned int)time(NULL)); 

    //Génération des spirales
    SamplePoint spiralBlue[NB_POINTS_SPIRALE];
    SamplePoint spiralRed[NB_POINTS_SPIRALE];
    generateSpirals(spiralBlue, spiralRed, NB_POINTS_SPIRALE);
    //generateTriangle(spiralBlue, spiralRed, NB_POINTS_SPIRALE);
    //generateRoses(spiralBlue, spiralRed, NB_POINTS_SPIRALE);
    

    //Création du réseau
    NeuralNetwork *net = createNetwork(nbLayers, layerSizes);

    // Initialisation de la SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Erreur SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    //Création de la fenêtre
    SDL_Window *window = SDL_CreateWindow("Classification Spirales - Reseau de Neurones",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    //Test de la création de la fenêtre                                        
    if(!window){
        fprintf(stderr, "Erreur CreateWindow: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    //Création du rendu
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    //Test de la création du rendu
    if(!renderer){
        fprintf(stderr, "Erreur CreateRenderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    //initClassificationTexture(renderer);
    
    //variable de la boucle principale
    int running = 1; //Variable d'arrêt de la boucle principale
    int needRedraw = 1; //Variable de redessinnage/affichage de la carte de classification en SDL
    //Boucle principale
    while(running) {
        //Création de l'evénement SDL
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            //Si evenement
            if(e.type == SDL_QUIT){
                running = 0;
            }
            //Si touche pressée
            else if(e.type == SDL_KEYDOWN){
                //Le programme s'arrête si on appuie sur la touche 'q' ou 'echap'
                if(e.key.keysym.sym == SDLK_ESCAPE || e.key.keysym.sym == SDLK_q){
                    running = 0;
                }
                //L'entraînement du réseau de neurones est lancé si on appuie sur 't'
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
                        double sum = 0.0;
                        const int iterations = 2;
                        //Boucle d'entrainement
                        for(int i =0; i< iterations; i++){
                        printf("entrainement n° %d\n",i+1);
                        //variable de temps avant apprentissage
                        double before = clock();
                        trainNetwork(net, dataset, NB_POINTS_SPIRALE*2);
                        //variable de temps après apprentissage
                        double after = clock();
                        //On affiche le temps d'apprentissage
                        printf("Apprentissage en %f secondes\n", (after - before) / CLOCKS_PER_SEC);
                        sum += (after - before) / CLOCKS_PER_SEC;
                        needRedraw = 1;
                        // On affiche la carte de classification
                         if(needRedraw){
                            // 1) On dessine la carte de classification
                            drawClassificationMap(renderer, net);
                            // 2) On dessine les points (spirales)
                            SDL_Color blue = {0, 0, 0xFF, 0xFF};
                            SDL_Color red  = {0xFF, 0, 0, 0xFF};
                            drawSamples(renderer, spiralBlue, NB_POINTS_SPIRALE, blue);
                            drawSamples(renderer, spiralRed,  NB_POINTS_SPIRALE, red);
                            // 3) On affiche le rendu
                            SDL_RenderPresent(renderer);
                            needRedraw = 0;
                        }
                    }
                    printf("Apprentissage moyen en %f secondes\n", sum/iterations);
                    printf("Temps d'apprentissage total : %f secondes\n", sum);

                }
                //L'enregistrement du réseau de neurones est lancé si on appuie sur 's'
                else if(e.key.keysym.sym == SDLK_s){
                    saveNetwork(net, "reseau_sauvegarde.txt");
                }
                //Restauration du réseau de neurones sauvegardé est lancé si on appuie sur 'l'
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
        //Si needRedraw =1 on dessine la carte de classification
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

    // Liberation des ressources en mémoire
    freeNetwork(net);
    //Nettoyage de la SDL
    SDL_DestroyTexture(classificationTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

