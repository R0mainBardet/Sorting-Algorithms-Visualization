#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define window_name "Sorting algorithm visualization"
#define INITIAL_WIDTH 800
#define INITIAL_HEIGHT 600

int main() {
    int n = 500;
    int win_width = INITIAL_WIDTH;
    int win_height = INITIAL_HEIGHT;

    int *tableau = malloc(n * sizeof(int));
    if (!tableau) {
        printf("Erreur d'allocation mémoire\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        tableau[i] = i + 1;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        free(tableau);
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        window_name,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        win_width, win_height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE  // ✅ fenetre redimensionnable
    );

    if (!window) {
        printf("Erreur SDL_CreateWindow : %s\n", SDL_GetError());
        SDL_Quit();
        free(tableau);
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erreur SDL_CreateRenderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        free(tableau);
        return 1;
    }

    int running = 1;
    SDL_Event event;

    while (running) {
        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            else if (event.type == SDL_WINDOWEVENT &&
                     event.window.event == SDL_WINDOWEVENT_RESIZED) {
                // ✅ Met à jour la taille quand la fenêtre change
                win_width = event.window.data1;
                win_height = event.window.data2;
            }
        }

        float width_rectangle = (float)win_width / n;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        float posx = 0;
        for (int i = 0; i < n; i++) {
            SDL_Rect bar;
            bar.w = width_rectangle;
            bar.h = (tableau[i] * win_height) / n;
            bar.x = posx;
            bar.y = win_height - bar.h;

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &bar);

            posx += width_rectangle;
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    free(tableau);
    return 0;
}
