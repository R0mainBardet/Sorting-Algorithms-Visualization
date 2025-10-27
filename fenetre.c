#include <SDL2/SDL.h>

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

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
    float rectMenuWidth = 3.0/4.0 * (float)WINDOW_WIDTH;
    SDL_Rect menuRect;

    menuRect.x = 0;
    menuRect.y = 0;
    menuRect.w = rectMenuWidth;
    menuRect.h = WINDOW_HEIGHT;

    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &menuRect);
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
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;

        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }

        if (minIndex != i) {
            int temp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = temp;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect rightArea;
        rightArea.x = WINDOW_WIDTH / 4;
        rightArea.y = 0;
        rightArea.w = 3 * WINDOW_WIDTH / 4;
        rightArea.h = WINDOW_HEIGHT;
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

    drawMenu(renderer);

    int n = 1000;

    int* numbers = generateIntegers(n);

    int* randomNumbers = randomizeIntegers(numbers, n);
    selectionSort(randomNumbers, n, renderer);

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

/*

gcc fenetre.c -o fenetre `sdl2-config --cflags --libs`
./fenetre

*/