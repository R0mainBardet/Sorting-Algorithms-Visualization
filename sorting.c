#include <stdbool.h>
#include <SDL2/SDL_ttf.h>

#include "globals.h"
#include "visual.h"
#include "utils.h"

// Selection sort
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

// Insertion sort
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

// Bubble sort
Metrics bubbleSort(SDL_Texture* textureFond, TTF_Font* font, int* arr, int n, SDL_Renderer* renderer, Metrics metrics) {
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
            bool stillPaused = true;
            while (stillPaused) {
                SDL_Event e;
                while (SDL_PollEvent(&e)) {
                    if (e.type == SDL_QUIT) {
                        running = false;
                        stillPaused = false;
                    }
                    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
                        paused = false;
                        stillPaused = false;
                    }
                }
                SDL_Delay(50);
            }
            continue;
        }

        for (int j = 0; j < n - i - 1; j++) {
            metrics.memAccess += 2;
            metrics.comparisons++;
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }

        SDL_Rect rightArea = { WINDOW_WIDTH / 4, 0, 3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT };
        if (easteregg && textureFond) {
            SDL_RenderCopy(renderer, textureFond, NULL, &rightArea);
        } else {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(renderer, &rightArea);
        }

        drawRectangles(renderer, arr, n, arr[n - i - 1]);

        SDL_RenderPresent(renderer);
    }

    return metrics;
}

// Quick sort
Metrics quicksort(SDL_Texture* textureFond, TTF_Font* font, int* arr, int n, SDL_Renderer* renderer, Metrics metrics) {
    bool running = true;
    bool paused = false;

    SDL_Rect rightArea = { WINDOW_WIDTH / 4, 0, 3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT };

    void quicksortRec(int* arr, int low, int high) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                paused = !paused;
        }

        if (!running) return;

        if (paused) {
            SDL_Delay(10);
            quicksortRec(arr, low, high);
            return;
        }

        if (low < high && running) {
            int pivot = arr[high];
            metrics.memAccess++;
            int i = (low - 1);

            for (int j = low; j < high; j++) {
                metrics.memAccess += 2;
                metrics.comparisons++;
                if (arr[j] < pivot) {
                    i++;
                    int temp = arr[i];
                    arr[i] = arr[j];
                    arr[j] = temp;
                }

                if (easteregg && textureFond) {
                    SDL_RenderCopy(renderer, textureFond, NULL, &rightArea);
                } else {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderFillRect(renderer, &rightArea);
                }
                drawRectangles(renderer, arr, n, arr[j]);
                SDL_RenderPresent(renderer);
            }

            int temp = arr[i + 1];
            arr[i + 1] = arr[high];
            arr[high] = temp;
            int pi = i + 1;

            if (easteregg && textureFond) {
                SDL_RenderCopy(renderer, textureFond, NULL, &rightArea);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &rightArea);
            }
            drawRectangles(renderer, arr, n, arr[pi]);
            SDL_RenderPresent(renderer);

            quicksortRec(arr, low, pi - 1);
            quicksortRec(arr, pi + 1, high);
        }
    }

    quicksortRec(arr, 0, n - 1);
    return metrics;
}