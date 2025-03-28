#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**
 * Programme d'analyse d'image.
 * Utilisation : ./analyse reference.bmp screenshot.bmp
 * Calcule et affiche le pourcentage de similarité entre les deux images.
 */
int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s reference.bmp screenshot.bmp\n", argv[0]);
        return 1;
    }
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    
    // Charge les images BMP originales
    SDL_Surface *refOrig = SDL_LoadBMP(argv[1]);
    SDL_Surface *testOrig = SDL_LoadBMP(argv[2]);
    if (!refOrig || !testOrig) {
        fprintf(stderr, "Erreur lors du chargement d'une image: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    
    // Vérifie que les images ont les mêmes dimensions
    if (refOrig->w != testOrig->w || refOrig->h != testOrig->h) {
        fprintf(stderr, "Les images n'ont pas les mêmes dimensions !\n");
        SDL_FreeSurface(refOrig);
        SDL_FreeSurface(testOrig);
        SDL_Quit();
        return 1;
    }
    
    // Convertir les surfaces vers un format 32 bits (ARGB8888) pour simplifier l'accès aux pixels
    SDL_Surface *ref = SDL_ConvertSurfaceFormat(refOrig, SDL_PIXELFORMAT_ARGB8888, 0);
    SDL_Surface *test = SDL_ConvertSurfaceFormat(testOrig, SDL_PIXELFORMAT_ARGB8888, 0);
    SDL_FreeSurface(refOrig);
    SDL_FreeSurface(testOrig);
    
    if (!ref || !test) {
        fprintf(stderr, "Erreur lors de la conversion des surfaces: %s\n", SDL_GetError());
        if(ref) SDL_FreeSurface(ref);
        if(test) SDL_FreeSurface(test);
        SDL_Quit();
        return 1;
    }
    
    int total_pixels = ref->w * ref->h;
    int matching_pixels = 0;
    
    // Tolérance pour la comparaison de chaque canal de couleur
    double tolerance = 10.0;
    
    SDL_PixelFormat *fmt = ref->format;
    Uint32 *pixels_ref = (Uint32 *)ref->pixels;
    Uint32 *pixels_test = (Uint32 *)test->pixels;
    
    for (int i = 0; i < total_pixels; i++) {
        Uint8 r1, g1, b1, a1;
        Uint8 r2, g2, b2, a2;
        SDL_GetRGBA(pixels_ref[i], fmt, &r1, &g1, &b1, &a1);
        SDL_GetRGBA(pixels_test[i], fmt, &r2, &g2, &b2, &a2);
        
        // Compare les canaux (on ignore alpha ici)
        if (fabs(r1 - r2) <= tolerance &&
            fabs(g1 - g2) <= tolerance &&
            fabs(b1 - b2) <= tolerance) {
            matching_pixels++;
        }
    }
    
    double similarity = (double)matching_pixels / total_pixels * 100.0;
    printf("Similarité : %.2f%%\n", similarity);
    
    SDL_FreeSurface(ref);
    SDL_FreeSurface(test);
    SDL_Quit();
    
    return 0;
}
