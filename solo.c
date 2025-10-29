#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define window_name "Sorting algorithm visualization"
#define win_width 800
#define win_height 600

int main(){

    int n = 500;
    float width_rectangle = (float)win_width / n;


    int *tableau = malloc(n * sizeof(int));
    for(int i = 0; i < n; i++) {
        tableau[i] = i+1;
    }

    // Initialisation de SDL et gestion des erreurs
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return 1;
    }

    // Creation de la fenetre
    SDL_Window *window = SDL_CreateWindow(window_name,
                        SDL_WINDOWPOS_CENTERED,
                        SDL_WINDOWPOS_CENTERED,
                        win_width, win_height,
                        SDL_WINDOW_SHOWN);

    //gestion des erreurs de creation de fenetre
    if (!window) {
        printf("Erreur SDL_CreateWindow : %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Creation du renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    //boucle principale
    int running = 1;
    SDL_Event event;
    while (running) {
        //stock tout les evenements 
        while (SDL_PollEvent(&event)) {
            //si l'evenement est de type QUIT(cliquer sur la croix), on arrete la boucle
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }
        float posx = 0;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        for(int i = 0; i < n; i++) {
            SDL_Rect bar;
            bar.h = (tableau[i]*win_height)/n;
            bar.w =width_rectangle;
            bar.x = posx;
            bar.y = win_height - bar.h;
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderFillRect(renderer, &bar);
            posx += width_rectangle;
        }
        SDL_RenderPresent(renderer);
    }

    // Nettoyage et fermeture de SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
