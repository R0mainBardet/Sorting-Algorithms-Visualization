#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <time.h>

#include "globals.h"
#include "utils.h"
#include "sorting.h"
#include "visual.h"

#define MIX_DEFAULT_FORMAT AUDIO_S16SYS

// Global variables
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;
bool easteregg = false;
bool isInteger = false;
float* numbers = NULL;
int numberRectList[] = {10, 50, 100, 250, 500, 1000, 1261, 5000, 10000};
char* sortList[] = {"Selection Sort", "Insertion Sort", "Bubble Sort", "Quicksort"};
int sortListIndex = 0;
int numberRectListIndex = 0;
Metrics oldMetrics = {0, 0, 0};
Metrics metrics = {0, 0, 0};
Button listButtons[6];
int mouseX = 0;
int mouseY = 0;
SDL_Renderer* renderer = NULL;
SDL_Texture* textureFond = NULL;
TTF_Font* font = NULL;
TTF_Font* fontMetrics = NULL;

int main(int argc, char* argv[]) {

    srand((unsigned int)time(NULL));

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL_Init: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("Erreur TTF_Init: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Sorting Algorithms Visualization",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    if (!window) {
        printf("Erreur SDL_CreateWindow: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erreur SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    font = TTF_OpenFont("arial.ttf", WINDOW_WIDTH * 0.03);
    fontMetrics = TTF_OpenFont("arial.ttf", WINDOW_WIDTH * 0.018);
    if (!font || !fontMetrics) {
        printf("Erreur TTF_OpenFont: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    textureFond = chargerTexture("fond.jpg");
    if (!textureFond) {
        printf("Erreur lors du chargement du fond\n");
        return 1;
    }

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("Erreur SDL_Init: %s\n", SDL_GetError());
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Erreur Mix_OpenAudio: %s\n", Mix_GetError());
        return 1;
    }

    // Initial rendering: clear screen, draw menu, draw rectangles representing numbers, define drawing area for sorting visualization
    bool running = true;
    SDL_Event event;

    if (numbers) free(numbers);
    numbers = generateNumbers(numberRectList[numberRectListIndex]);
    SDL_RenderClear(renderer);
    drawMenu(numberRectList[numberRectListIndex], sortList[sortListIndex]);
    SDL_RenderPresent(renderer);
    drawRectangles(numbers, numberRectList[numberRectListIndex], -1);
    SDL_Rect rightArea = { WINDOW_WIDTH / 4, 0, 3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT };
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_WINDOWEVENT){
                // Window resize
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    WINDOW_WIDTH = event.window.data1;
                    WINDOW_HEIGHT = event.window.data2;
                    TTF_Font* newFont = TTF_OpenFont("arial.ttf", WINDOW_WIDTH * 0.03);
                    if (newFont) {
                        TTF_CloseFont(font);
                        font = newFont;
                    }
                    SDL_Rect rightArea = { WINDOW_WIDTH / 4, 0, 3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT };
                    // Handeling the easteregg
                    enableEasteregg(rightArea);
                    drawMenu(numberRectList[numberRectListIndex], sortList[sortListIndex]);
                    drawRectangles(numbers, numberRectList[numberRectListIndex], -1);
                    SDL_RenderPresent(renderer);
                }
            }
            if (event.type == SDL_KEYDOWN) {
                // Pause the sorting
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
                // Change the numbers type
                if (event.key.keysym.sym == SDLK_t) {
                    isInteger = !isInteger;
                    free(numbers);
                    numbers = generateNumbers(numberRectList[numberRectListIndex]);
                    if (numbers != NULL)
                        numbers = randomizeNumbers(numbers, numberRectList[numberRectListIndex]);
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderFillRect(renderer, &rightArea);
                    drawMenu(numberRectList[numberRectListIndex], sortList[sortListIndex]);
                    drawRectangles(numbers, numberRectList[numberRectListIndex], -1);
                }
                if (event.key.keysym.sym == SDLK_r) {
                    free(numbers);
                    numbers = generateNumbers(numberRectList[numberRectListIndex]);
                    if (numbers != NULL)
                        numbers = randomizeNumbers(numbers, numberRectList[numberRectListIndex]);
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderFillRect(renderer, &rightArea);
                    drawMenu(numberRectList[numberRectListIndex], sortList[sortListIndex]);
                    drawRectangles(numbers, numberRectList[numberRectListIndex], -1);
                }
            }
            // Mouse clicks
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                mouseX = event.button.x;
                mouseY = event.button.y;
                // Change the number of values
                if (isButtonClicked(&listButtons[0])) {
                    numberRectListIndex = (numberRectListIndex - 1 + (sizeof(numberRectList)/sizeof(numberRectList[0]))) % (sizeof(numberRectList)/sizeof(numberRectList[0]));
                    SDL_Rect rightArea = { WINDOW_WIDTH / 4, 0, 3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT };
                    enableEasteregg(rightArea);
                    drawMenu(numberRectList[numberRectListIndex], sortList[sortListIndex]);
                    free(numbers);
                    numbers = generateNumbers(numberRectList[numberRectListIndex]);
                    if (numbers != NULL)
                        numbers = randomizeNumbers(numbers, numberRectList[numberRectListIndex]);
                    drawRectangles(numbers, numberRectList[numberRectListIndex], -1);
                }
                if (isButtonClicked(&listButtons[1])) {
                    numberRectListIndex = (numberRectListIndex + 1) % (sizeof(numberRectList)/sizeof(numberRectList[0]));
                    SDL_Rect rightArea = { WINDOW_WIDTH / 4, 0, 3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT };
                    enableEasteregg(rightArea);
                    drawMenu(numberRectList[numberRectListIndex], sortList[sortListIndex]);
                    free(numbers);
                    numbers = generateNumbers(numberRectList[numberRectListIndex]);
                    if (numbers != NULL)
                        numbers = randomizeNumbers(numbers, numberRectList[numberRectListIndex]);
                    drawRectangles(numbers, numberRectList[numberRectListIndex], -1);
                }
                // Change the sorting
                if (isButtonClicked(&listButtons[2])) {
                    sortListIndex = (sortListIndex + 1) % (sizeof(sortList)/sizeof(sortList[0]));
                    drawMenu(numberRectList[numberRectListIndex], sortList[sortListIndex]);
                }
                if (isButtonClicked(&listButtons[3])) {
                    sortListIndex = (sortListIndex - 1 + (sizeof(sortList)/sizeof(sortList[0]))) % (sizeof(sortList)/sizeof(sortList[0]));
                    drawMenu(numberRectList[numberRectListIndex], sortList[sortListIndex]);
                }
                // Enable the easteregg
                if (numberRectListIndex == 6 && isButtonClicked(&listButtons[5])) {
                    easteregg = !easteregg;
                    if (numberRectList[6] == 1261){
                        numberRectList[6] = 1216;
                        playMusic("easteregg.mp3");
                        SDL_RenderCopy(renderer, textureFond, NULL, &rightArea);
                    } else {
                        numberRectList[6] = 1261;
                        stopMusic();
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderFillRect(renderer, &rightArea);
                    }
                    drawMenu(numberRectList[numberRectListIndex], sortList[sortListIndex]);
                    drawRectangles(numbers, numberRectList[numberRectListIndex], -1);
                }
                // Launch the sort
                launchSort(rightArea);
            }
        }
    }

    // Clear everything
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

/*

gcc main.c utils.c sorting.c visual.c -o sorting `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_image -lSDL2_mixer
./sorting

*/