#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "globals.h"
#include "utils.h"
#include "sorting.h"
#include "visual.h"
#include "stats.h"

#define MIX_DEFAULT_FORMAT AUDIO_S16SYS

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;
bool easteregg = false;


int numberRectList[] = {10, 50, 100, 250, 500, 1000, 1261, 5000, 10000};
char* sortList[] = {"Selection Sort", "Insertion Sort"};
int sortListIndex = 0;
int numberRectListIndex = 0;
Metrics oldMetrics = {0, 0, 0};
Metrics metrics = {0, 0, 0};
Button listButtons[6];

int main(int argc, char* argv[]) {

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

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erreur SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("arial.ttf", WINDOW_WIDTH * 0.03);
    if (!font) {
        printf("Erreur TTF_OpenFont: %s\n", TTF_GetError());
    }

    SDL_Texture* textureFond = chargerTexture(renderer, "fond.jpg");
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

    bool running = true;
    SDL_Event event;

    SDL_RenderClear(renderer);
    drawMenu(renderer, listButtons, numberRectList[numberRectListIndex], sortList[sortListIndex], font, oldMetrics, metrics);
    SDL_RenderPresent(renderer);
    drawRectangles(renderer, generateIntegers(numberRectList[numberRectListIndex]), numberRectList[numberRectListIndex], -1);
    SDL_Rect rightArea = { WINDOW_WIDTH / 4, 0, 3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT };
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_WINDOWEVENT){
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    WINDOW_WIDTH = event.window.data1;
                    WINDOW_HEIGHT = event.window.data2;
                    font = TTF_OpenFont("arial.ttf", WINDOW_WIDTH * 0.03);
                    SDL_Rect rightArea = { WINDOW_WIDTH / 4, 0, 3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT };
                    if (easteregg && textureFond) {
                        SDL_RenderCopy(renderer, textureFond, NULL, &rightArea);
                    } else {
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderFillRect(renderer, &rightArea);
                    }
                    drawMenu(renderer, listButtons, numberRectList[numberRectListIndex], sortList[sortListIndex], font, oldMetrics, metrics);
                    drawRectangles(renderer, generateIntegers(numberRectList[numberRectListIndex]), numberRectList[numberRectListIndex], -1);
                    SDL_RenderPresent(renderer);
                }
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                if (isButtonClicked(&listButtons[0], mouseX, mouseY)) {
                    numberRectListIndex = (numberRectListIndex - 1 + (sizeof(numberRectList)/sizeof(numberRectList[0]))) % (sizeof(numberRectList)/sizeof(numberRectList[0]));
                    drawMenu(renderer, listButtons, numberRectList[numberRectListIndex], sortList[sortListIndex], font, oldMetrics, metrics);
                    if (easteregg && textureFond) {
                        SDL_RenderCopy(renderer, textureFond, NULL, &rightArea);
                    } else {
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderFillRect(renderer, &rightArea);
                    }
                    drawRectangles(renderer, generateIntegers(numberRectList[numberRectListIndex]), numberRectList[numberRectListIndex], -1);
                }
                if (isButtonClicked(&listButtons[1], mouseX, mouseY)) {
                    numberRectListIndex = (numberRectListIndex + 1) % (sizeof(numberRectList)/sizeof(numberRectList[0]));
                    drawMenu(renderer, listButtons, numberRectList[numberRectListIndex], sortList[sortListIndex], font, oldMetrics, metrics);
                    if (easteregg && textureFond) {
                        SDL_RenderCopy(renderer, textureFond, NULL, &rightArea);
                    } else {
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderFillRect(renderer, &rightArea);
                    }
                    drawRectangles(renderer, generateIntegers(numberRectList[numberRectListIndex]), numberRectList[numberRectListIndex], -1);
                }
                if (isButtonClicked(&listButtons[2], mouseX, mouseY)) {
                    sortListIndex = (sortListIndex + 1) % (sizeof(sortList)/sizeof(sortList[0]));
                    drawMenu(renderer, listButtons, numberRectList[numberRectListIndex], sortList[sortListIndex], font, oldMetrics, metrics);
                }
                if (isButtonClicked(&listButtons[3], mouseX, mouseY)) {
                    sortListIndex = (sortListIndex - 1 + (sizeof(sortList)/sizeof(sortList[0]))) % (sizeof(sortList)/sizeof(sortList[0]));
                    drawMenu(renderer, listButtons, numberRectList[numberRectListIndex], sortList[sortListIndex], font, oldMetrics, metrics);
                }
                if (numberRectListIndex == 6 && isButtonClicked(&listButtons[5], mouseX, mouseY)) {
                    easteregg = !easteregg;
                    if (numberRectList[6] == 1261){
                        easteregg = false;
                        numberRectList[6] = 1216;
                        playMusic("easteregg.mp3");
                        SDL_RenderCopy(renderer, textureFond, NULL, &rightArea);
                    } else {
                        numberRectList[6] = 1261;
                        stopMusic();
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderFillRect(renderer, &rightArea);
                    }
                    drawMenu(renderer, listButtons, numberRectList[numberRectListIndex], sortList[sortListIndex], font, oldMetrics, metrics);
                    drawRectangles(renderer, generateIntegers(numberRectList[numberRectListIndex]), numberRectList[numberRectListIndex], -1);
                }
                if (isButtonClicked(&listButtons[4], mouseX, mouseY)) {
                    oldMetrics = metrics;
                    metrics.memAccess = 0;
                    metrics.comparisons = 0;
                    metrics.timeElapsed = 0;
                    int* numbers = generateIntegers(numberRectList[numberRectListIndex]);
                    int* randomNumbers = randomizeIntegers(numbers, numberRectList[numberRectListIndex]);
                    Uint64 start = 0;
                    Uint64 end = 0;
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
                        default:
                            break;
                    }
                    metrics.timeElapsed = (double)(end - start) / SDL_GetPerformanceFrequency();
                    drawMenu(renderer, listButtons, numberRectList[numberRectListIndex], sortList[sortListIndex], font, oldMetrics, metrics);
                }
            }
        }
    }

    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

/*

gcc main.c utils.c sorting.c visual.c stats.c -o sorting `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_image -lSDL2_mixer
./sorting

*/