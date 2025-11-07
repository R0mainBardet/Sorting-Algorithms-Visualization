#include <stdbool.h>
#include <SDL2/SDL_ttf.h>

#include "globals.h"
#include "visual.h"
#include "utils.h"

// Selection sort
Metrics selectionSort(int n) {
    bool running = true;
    bool paused = false;
    Uint64 start = 0;
    Uint64 end = 0;
    start = SDL_GetPerformanceCounter();

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
            metrics.memAccessSort += 2;
            metrics.comparisons++;
            metrics.comparisonsSort++;
            if (numbers[j] < numbers[minIndex]) {
                minIndex = j;
            }
        }

        metrics.memAccess += 4;
        metrics.memAccessSort += 4;
        float temp = numbers[i];
        numbers[i] = numbers[minIndex];
        numbers[minIndex] = temp;
        
        SDL_Rect rightArea = { WINDOW_WIDTH / 4, 0, 3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT };
        enableEasteregg(rightArea);

        drawRectangles(numbers, n, numbers[i]);

        SDL_RenderPresent(renderer);
        
        end = SDL_GetPerformanceCounter();
        metrics.timeElapsed = (double)(end - start) / SDL_GetPerformanceFrequency();
        drawMenu(numberRectList[numberRectListIndex], sortList[sortListIndex]);
    }

    return metrics;
}

// Insertion sort
Metrics insertionSort(int n) {
    bool running = true;
    bool paused = false;
    Uint64 start = 0;
    Uint64 end = 0;
    start = SDL_GetPerformanceCounter();

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

        float key = numbers[i];
        int j = i - 1;

        while (j >= 0 && numbers[j] > key) {
            metrics.memAccess += 4;
            metrics.memAccessSort += 4;
            metrics.comparisons++;
            metrics.comparisonsSort++;
            numbers[j + 1] = numbers[j];
            j--;

            if (easteregg){
                SDL_RenderCopy(renderer, textureFond, NULL, &rightArea);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &rightArea);
            }

            drawRectangles(numbers, n, key);
        }

        numbers[j + 1] = key;

        enableEasteregg(rightArea);

        drawRectangles(numbers, n, key);

        end = SDL_GetPerformanceCounter();
        metrics.timeElapsed = (double)(end - start) / SDL_GetPerformanceFrequency();
        drawMenu(numberRectList[numberRectListIndex], sortList[sortListIndex]);
    }
    return metrics;
}

// Bubble sort
Metrics bubbleSort(int n) {
    bool running = true;
    bool paused = false;
    Uint64 start = 0;
    Uint64 end = 0;
    start = SDL_GetPerformanceCounter();

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
            metrics.memAccessSort += 2;
            metrics.comparisons++;
            metrics.comparisonsSort++;
            if (numbers[j] > numbers[j + 1]) {
                metrics.memAccess += 4;
                metrics.memAccessSort += 4;
                float temp = numbers[j];
                numbers[j] = numbers[j + 1];
                numbers[j + 1] = temp;
            }
        }

        SDL_Rect rightArea = { WINDOW_WIDTH / 4, 0, 3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT };
        enableEasteregg(rightArea);

        drawRectangles(numbers, n, numbers[n - i - 1]);

        SDL_RenderPresent(renderer);

        end = SDL_GetPerformanceCounter();
        metrics.timeElapsed = (double)(end - start) / SDL_GetPerformanceFrequency();
        drawMenu(numberRectList[numberRectListIndex], sortList[sortListIndex]);
    }

    return metrics;
}

// Quick sort
Metrics quicksort(int n) {
    bool running = true;
    bool paused = false;
    Uint64 start = SDL_GetPerformanceCounter();

    SDL_Rect rightArea = { WINDOW_WIDTH / 4, 0, 3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT };

    void quicksortRec(float* numbers, int low, int high) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                paused = !paused;
        }

        if (!running) return;

        while (paused && running) {
            SDL_Delay(10);
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT)
                    running = false;
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                    paused = !paused;
            }
        }

        if (low < high && running) {
            float pivot = numbers[high];
            metrics.memAccess++;
            metrics.memAccessSort++;

            int i = (low - 1);
            for (int j = low; j < high; j++) {
                metrics.memAccess += 2;
                metrics.memAccessSort += 2;
                metrics.comparisons++;
                metrics.comparisonsSort++;

                if (numbers[j] < pivot) {
                    i++;
                    metrics.memAccess += 4;
                    metrics.memAccessSort += 4;
                    float temp = numbers[i];
                    numbers[i] = numbers[j];
                    numbers[j] = temp;
                }

                Uint64 now = SDL_GetPerformanceCounter();
                metrics.timeElapsed = (double)(now - start) / SDL_GetPerformanceFrequency();

                enableEasteregg(rightArea);
                drawRectangles(numbers, n, numbers[j]);
                SDL_RenderPresent(renderer);
                drawMenu(numberRectList[numberRectListIndex], sortList[sortListIndex]);
            }

            float temp = numbers[i + 1];
            numbers[i + 1] = numbers[high];
            numbers[high] = temp;
            int pi = i + 1;

            Uint64 now = SDL_GetPerformanceCounter();
            metrics.timeElapsed = (double)(now - start) / SDL_GetPerformanceFrequency();

            enableEasteregg(rightArea);
            drawRectangles(numbers, n, numbers[pi]);
            SDL_RenderPresent(renderer);
            drawMenu(numberRectList[numberRectListIndex], sortList[sortListIndex]);

            quicksortRec(numbers, low, pi - 1);
            quicksortRec(numbers, pi + 1, high);
        }
    }

    quicksortRec(numbers, 0, n - 1);

    Uint64 end = SDL_GetPerformanceCounter();
    metrics.timeElapsedSort = (double)(end - start) / SDL_GetPerformanceFrequency();

    return metrics;
}