#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "globals.h"
#include "sorting.h"
#include "visual.h"

// Create all the buttons
Button* createButtons(Button* buttons) {

    buttons[0].rect.x = WINDOW_WIDTH / 40.0;
    buttons[0].rect.y = (4.2/8.0) * WINDOW_HEIGHT;
    buttons[0].rect.w = (float)WINDOW_WIDTH/11.42857;
    buttons[0].rect.h = WINDOW_HEIGHT / 12.0;
    buttons[0].color = (SDL_Color){0, 0, 0};

    buttons[1].rect.x = buttons[0].rect.x + buttons[0].rect.w + buttons[0].rect.x;
    buttons[1].rect.y = (4.2/8.0) * WINDOW_HEIGHT;
    buttons[1].rect.w = (float)WINDOW_WIDTH/11.42857;
    buttons[1].rect.h = WINDOW_HEIGHT / 12.0;
    buttons[1].color = (SDL_Color){0, 0, 0};

    buttons[2].rect.x = WINDOW_WIDTH / 40.0;
    buttons[2].rect.y = (6.0/8.0) * WINDOW_HEIGHT;
    buttons[2].rect.w = (float)WINDOW_WIDTH/11.42857;
    buttons[2].rect.h = WINDOW_HEIGHT / 12.0;
    buttons[2].color = (SDL_Color){0, 0, 0};

    buttons[3].rect.x = buttons[2].rect.x + buttons[2].rect.w + buttons[2].rect.x;
    buttons[3].rect.y = (6.0/8.0) * WINDOW_HEIGHT;
    buttons[3].rect.w = (float)WINDOW_WIDTH/11.42857;
    buttons[3].rect.h = WINDOW_HEIGHT / 12.0;
    buttons[3].color = (SDL_Color){0, 0, 0};

    buttons[4].rect.x = WINDOW_WIDTH / 40.0;
    buttons[4].rect.y = (7.0/8.0)*WINDOW_HEIGHT;
    buttons[4].rect.w = (float)WINDOW_WIDTH/4.0 - buttons[0].rect.x*2.0;
    buttons[4].rect.h = WINDOW_HEIGHT / 12.0;
    buttons[4].color = (SDL_Color){255, 0, 0};

    buttons[5].rect.x = WINDOW_WIDTH / 40.0;
    buttons[5].rect.y = (3.4 / 8.0) * WINDOW_HEIGHT;
    buttons[5].rect.w = (float)WINDOW_WIDTH/4.0 - buttons[5].rect.x*2.0;
    buttons[5].rect.h = WINDOW_HEIGHT / 12.0;
    buttons[5].color = (SDL_Color){0, 0, 0};

    return buttons;
}

// Check if a button is clicked
bool isButtonClicked(Button* button, int x, int y) {
    return x >= button->rect.x && x <= button->rect.x + button->rect.w &&
           y >= button->rect.y && y <= button->rect.y + button->rect.h;
}

// Generate the list of numbers
int* generateIntegers(int n) {
    int* arr = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }
    return arr;
}

// Randomize the list of numbers
int* randomizeIntegers(int* arr, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
    return arr;
}

// Launch the selected sort
void launchSort(SDL_Texture* textureFond, TTF_Font* font, SDL_Renderer* renderer, Button* listButtons) {
    if (isButtonClicked(&listButtons[4], mouseX, mouseY)) {
        oldMetrics = metrics;
        metrics.memAccess = 0;
        metrics.comparisons = 0;
        metrics.timeElapsed = 0;
        int* numbers = generateIntegers(numberRectList[numberRectListIndex]);
        int* randomNumbers = randomizeIntegers(numbers, numberRectList[numberRectListIndex]);
        // Variables to record the process time
        Uint64 start = 0;
        Uint64 end = 0;
        // Launch the sort and get the corresponding metrics
        switch (sortListIndex) {
            case 0:
                start = SDL_GetPerformanceCounter();
                metrics = selectionSort(textureFond, font, randomNumbers, numberRectList[numberRectListIndex], renderer, metrics);
                end = SDL_GetPerformanceCounter();
                break;
            case 1:
                start = SDL_GetPerformanceCounter();
                metrics = insertionSort(textureFond, font, randomNumbers, numberRectList[numberRectListIndex], renderer, metrics);
                end = SDL_GetPerformanceCounter();
                break;
            case 2:
                start = SDL_GetPerformanceCounter();
                metrics = bubbleSort(textureFond, font, randomNumbers, numberRectList[numberRectListIndex], renderer, metrics);
                end = SDL_GetPerformanceCounter();
                break;
            case 3:
                start = SDL_GetPerformanceCounter();
                metrics = quicksort(textureFond, font, randomNumbers, numberRectList[numberRectListIndex], renderer, metrics);
                end = SDL_GetPerformanceCounter();
                break;
            default:
                break;
        }
        metrics.timeElapsed = (double)(end - start) / SDL_GetPerformanceFrequency();
        drawMenu(renderer, listButtons, numberRectList[numberRectListIndex], sortList[sortListIndex], font, oldMetrics, metrics);
    }
}