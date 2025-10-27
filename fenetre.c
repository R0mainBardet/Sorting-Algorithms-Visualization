#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

typedef struct {
    SDL_Rect rect;
    SDL_Color color;
} Button;

void drawButton(SDL_Renderer* renderer, Button* button) {
    SDL_SetRenderDrawColor(renderer, button->color.r, button->color.g, button->color.b, 255);
    SDL_RenderFillRect(renderer, &button->rect);
}

bool isButtonClicked(Button* button, int x, int y) {
    return x >= button->rect.x && x <= button->rect.x + button->rect.w &&
           y >= button->rect.y && y <= button->rect.y + button->rect.h;
}

void drawRectangles(SDL_Renderer* renderer, int* numbers, int n) {
    float rectMenuWidth = 3.0/4.0 * (float)WINDOW_WIDTH;
    float rectWidth = rectMenuWidth / n;
    float x = 0;

    int maxValue = 0;
    for (int i = 0; i < n; i++)
        if (numbers[i] > maxValue) maxValue = numbers[i];

    float scale = (float)WINDOW_HEIGHT / maxValue;

    for (int i = 0; i < n; i++) {
        SDL_Rect rect;
        rect.h = numbers[i] * scale;
        rect.y = WINDOW_HEIGHT - rect.h;
        rect.x = WINDOW_WIDTH-rectMenuWidth + (int)x;
        rect.w = (int)(x + rectWidth) - (int)x;

        SDL_SetRenderDrawColor(renderer, (i * 255) / n, 100, 150, 255);
        SDL_RenderFillRect(renderer, &rect);

        x += rectWidth;
    }

    SDL_RenderPresent(renderer);
}

void drawMenu(SDL_Renderer* renderer) {
    float rectMenuWidth = 1.0/4.0 * (float)WINDOW_WIDTH;
    SDL_Rect menuRect;

    menuRect.x = 0;
    menuRect.y = 0;
    menuRect.w = rectMenuWidth;
    menuRect.h = WINDOW_HEIGHT;

    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &menuRect);
    SDL_RenderPresent(renderer);
}

int* generateIntegers(int n) {
    int* arr = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }
    return arr;
}

int* randomizeIntegers(int* arr, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
    return arr;
}

void selectionSort(int* arr, int n, SDL_Renderer* renderer) {
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
            SDL_Delay(10);
            continue;
        }

        int minIndex = i;

        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }

        int temp = arr[i];
        arr[i] = arr[minIndex];
        arr[minIndex] = temp;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect rightArea = { WINDOW_WIDTH / 4, 0, 3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT };
        SDL_RenderFillRect(renderer, &rightArea);

        drawRectangles(renderer, arr, n);
    }
}



int main(int argc, char* argv[]) {

    SDL_Window* window = SDL_CreateWindow(
        "Romain la grosse salope",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erreur SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;
    int n = 10000;

    Button myButton = {{20, 20, 150, 50}, {255, 0, 0}};

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderClear(renderer);

    drawMenu(renderer);
    drawButton(renderer, &myButton);

    SDL_RenderPresent(renderer);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                if (isButtonClicked(&myButton, mouseX, mouseY)) {
                    int* numbers = generateIntegers(n);
                    int* randomNumbers = randomizeIntegers(numbers, n);
                    selectionSort(randomNumbers, n, renderer);
                }
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

/*

gcc fenetre.c -o fenetre `sdl2-config --cflags --libs`
./fenetre

*/