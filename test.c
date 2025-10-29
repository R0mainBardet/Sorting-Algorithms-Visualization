#include <SDL2/SDL.h>
#include <stdio.h>

int main() {

    // Initialiser SDL et verifie si il y a des erreurs d'initialisation
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    int n = 100;
    int array[n];
    int win_width = 800;
    int win_height = 600;

    for(int i = 0; i < n; i++) {
        array[i] = n - i;
    }
    printf("%d", array[0]);

    // Creer une fenetre SDL 
    SDL_Window* window = SDL_CreateWindow("Sorting algorithm visualization",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          win_width, win_height,
                                          SDL_WINDOW_SHOWN);
    // Verifie si la fenetre est creee correctement sinon retourne une erreur
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white bars
    SDL_RenderFillRect(renderer, &bar);
    
    // Delai de 3000 milliseconds (3 seconds)
    SDL_Delay(3000);

    // detruire la fenetre et quitter SDL
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
