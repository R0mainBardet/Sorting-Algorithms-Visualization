#include <stdbool.h>
#include <SDL2/SDL_ttf.h>

#include "globals.h"
#include "visual.h"

Metrics selectionSort(SDL_Texture* textureFond, TTF_Font* font, int* arr, int n, SDL_Renderer* renderer, Metrics metrics) {
    bool running = true;
    bool paused = false;

    for (int i = 0; i < n - 1 && running; i++) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                paused = !paused;
        }

        if (paused) {
            i--;
            continue;
        }

        int minIndex = i;

        for (int j = i + 1; j < n; j++) {
            metrics.memAccess += 2;
            metrics.comparisons++;
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }

        int temp = arr[i];
        arr[i] = arr[minIndex];
        arr[minIndex] = temp;
        
        SDL_Rect rightArea = { WINDOW_WIDTH / 4, 0, 3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT };
        if (easteregg && textureFond) {
            SDL_RenderCopy(renderer, textureFond, NULL, &rightArea);
        } else {
            SDL_Rect rightArea = { WINDOW_WIDTH / 4, 0, 3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT };
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(renderer, &rightArea);
        }

        drawRectangles(renderer, arr, n, arr[i]);

        SDL_RenderPresent(renderer);
    }

    return metrics;
}

Metrics insertionSort(SDL_Texture* textureFond, TTF_Font* font, int* arr, int n, SDL_Renderer* renderer, Metrics metrics) {
    bool running = true;
    bool paused = false;
    SDL_Rect rightArea = { WINDOW_WIDTH / 4, 0, 3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT };

    for (int i = 1; i < n && running; i++) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                paused = !paused;
        }

        if (paused) {
            i--;
            continue;
        }

        int key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key) {
            metrics.memAccess += 2;
            metrics.comparisons++;
            arr[j + 1] = arr[j];
            j--;

            if (easteregg){
                SDL_RenderCopy(renderer, textureFond, NULL, &rightArea);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &rightArea);
            }

            drawRectangles(renderer, arr, n, key);
        }

        arr[j + 1] = key;

        if (easteregg && textureFond) {
            SDL_RenderCopy(renderer, textureFond, NULL, &rightArea);
        } else {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(renderer, &rightArea);
        }

        drawRectangles(renderer, arr, n, key);
    }
    return metrics;
}