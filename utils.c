#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>

#include "globals.h"
#include "sorting.h"
#include "visual.h"

Uint64 startSort = 0;
Uint64 endSort = 0;

// Create all the buttons
void createButtons() {

    listButtons[0].rect.x = WINDOW_WIDTH / 40.0;
    listButtons[0].rect.y = (4.2/8.0) * WINDOW_HEIGHT;
    listButtons[0].rect.w = (float)WINDOW_WIDTH/11.42857;
    listButtons[0].rect.h = WINDOW_HEIGHT / 12.0;
    listButtons[0].color = (SDL_Color){0, 0, 0};

    listButtons[1].rect.x = listButtons[0].rect.x + listButtons[0].rect.w + listButtons[0].rect.x;
    listButtons[1].rect.y = (4.2/8.0) * WINDOW_HEIGHT;
    listButtons[1].rect.w = (float)WINDOW_WIDTH/11.42857;
    listButtons[1].rect.h = WINDOW_HEIGHT / 12.0;
    listButtons[1].color = (SDL_Color){0, 0, 0};

    listButtons[2].rect.x = WINDOW_WIDTH / 40.0;
    listButtons[2].rect.y = (6.0/8.0) * WINDOW_HEIGHT;
    listButtons[2].rect.w = (float)WINDOW_WIDTH/11.42857;
    listButtons[2].rect.h = WINDOW_HEIGHT / 12.0;
    listButtons[2].color = (SDL_Color){0, 0, 0};

    listButtons[3].rect.x = listButtons[2].rect.x + listButtons[2].rect.w + listButtons[2].rect.x;
    listButtons[3].rect.y = (6.0/8.0) * WINDOW_HEIGHT;
    listButtons[3].rect.w = (float)WINDOW_WIDTH/11.42857;
    listButtons[3].rect.h = WINDOW_HEIGHT / 12.0;
    listButtons[3].color = (SDL_Color){0, 0, 0};

    listButtons[4].rect.x = WINDOW_WIDTH / 40.0;
    listButtons[4].rect.y = (7.0/8.0)*WINDOW_HEIGHT;
    listButtons[4].rect.w = (float)WINDOW_WIDTH/4.0 - listButtons[0].rect.x*2.0;
    listButtons[4].rect.h = WINDOW_HEIGHT / 12.0;
    listButtons[4].color = (SDL_Color){255, 0, 0};

    listButtons[5].rect.x = WINDOW_WIDTH / 40.0;
    listButtons[5].rect.y = (3.4 / 8.0) * WINDOW_HEIGHT;
    listButtons[5].rect.w = (float)WINDOW_WIDTH/4.0 - listButtons[5].rect.x*2.0;
    listButtons[5].rect.h = WINDOW_HEIGHT / 12.0;
    listButtons[5].color = (SDL_Color){0, 0, 0};
}

float* generateNumbers(int n);
float* generateIntegers(int n);
float* generateFloats(int n);

// Check if a button is clicked
bool isButtonClicked(Button* button) {
    return mouseX >= button->rect.x && mouseX <= button->rect.x + button->rect.w &&
           mouseY >= button->rect.y && mouseY <= button->rect.y + button->rect.h;
}

float* generateNumbers(int n){
    if (isInteger){
        return generateIntegers(n);
    }
    return generateFloats(n);
}

// Generate the list of int numbers
float* generateIntegers(int n) {
    float* arr = malloc(n * sizeof(float));
    if (!arr) return NULL;

    for (int i = 0; i < n; i++) {
        arr[i] = (float)(rand() % n + 1);
    }

    return arr;
}

// Generate the list of float numbers
float* generateFloats(int n) {
    float* arr = malloc(n * sizeof(float));
    if (!arr) return NULL;

    for (int i = 0; i < n; i++) {
        arr[i] = ((float)rand() / RAND_MAX) * n;
    }

    return arr;
}

// Randomize the list of numbers
float* randomizeNumbers(float* arr, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        float temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
    return arr;
}

// Verify if the list is sorted
bool isNumberListSorted(float* arr, int n){
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]){
            return false;
        }
    }
    return true;
}

// Launch the selected sort
void launchSort(SDL_Rect rightArea) {
    if (isButtonClicked(&listButtons[4])) {

        if (isNumberListSorted(numbers, numberRectList[numberRectListIndex])) {
            free(numbers);
            numbers = generateNumbers(numberRectList[numberRectListIndex]);
            if (numbers != NULL)
                numbers = randomizeNumbers(numbers, numberRectList[numberRectListIndex]);
            drawMenu(numberRectList[numberRectListIndex], sortList[sortListIndex]);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(renderer, &rightArea);
            drawRectangles(numbers, numberRectList[numberRectListIndex], -1);
            return;
        }

        oldMetrics = metrics;

        metrics.memAccess = 0;
        metrics.comparisons = 0;
        metrics.timeElapsed = 0;
        metrics.memAccessSort = 0;
        metrics.comparisonsSort = 0;
        metrics.timeElapsedSort = 0;

        startSort = SDL_GetPerformanceCounter();
        switch (sortListIndex) {
            case 0:
                metrics = selectionSort(numberRectList[numberRectListIndex]);
                break;
            case 1:
                metrics = insertionSort(numberRectList[numberRectListIndex]);
                break;
            case 2:
                metrics = bubbleSort(numberRectList[numberRectListIndex]);
                break;
            case 3:
                metrics = quicksort(numberRectList[numberRectListIndex]);
                break;
            default:
                break;
        }
        endSort = SDL_GetPerformanceCounter();
        metrics.timeElapsedSort = (double)(endSort - startSort) / SDL_GetPerformanceFrequency();

        drawMenu(numberRectList[numberRectListIndex], sortList[sortListIndex]);
    }
}

// Enable the easteregg
void enableEasteregg(SDL_Rect rightArea){
    if (easteregg && textureFond) {
        SDL_RenderCopy(renderer, textureFond, NULL, &rightArea);
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rightArea);
    }
}