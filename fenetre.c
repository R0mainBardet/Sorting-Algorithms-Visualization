#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

typedef struct {
    SDL_Rect rect;
    SDL_Color color;
} Button;

typedef struct {
    long long memAccess;
    long long comparisons;
    double timeElapsed;
} Metrics;

void drawButton(SDL_Renderer* renderer, Button* button) {
    SDL_SetRenderDrawColor(renderer, button->color.r, button->color.g, button->color.b, 255);
    SDL_RenderFillRect(renderer, &button->rect);
}

bool isButtonClicked(Button* button, int x, int y) {
    return x >= button->rect.x && x <= button->rect.x + button->rect.w &&
           y >= button->rect.y && y <= button->rect.y + button->rect.h;
}

void drawRectangles(SDL_Renderer* renderer, int* numbers, int n, int actualValue) {
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
        if (numbers[i] == actualValue) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, (i * 255) / n, 100, 150, 255);
        }
        SDL_RenderFillRect(renderer, &rect);

        x += rectWidth;
    }

    SDL_RenderPresent(renderer);
}

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

    return buttons;
}

void renderTextInButton(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color, SDL_Rect buttonRect) {
    SDL_Surface* surf = TTF_RenderText_Blended(font, text, color);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);

    SDL_Rect dest;
    dest.w = surf->w;
    dest.h = surf->h;
    dest.x = buttonRect.x + (buttonRect.w - dest.w) / 2;
    dest.y = buttonRect.y + (buttonRect.h - dest.h) / 2;

    SDL_RenderCopy(renderer, tex, NULL, &dest);

    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}

void renderMetrics(SDL_Renderer* renderer, TTF_Font* font, Metrics metrics, Metrics oldMetrics, SDL_Rect TextMetricsRect) {

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color green = {0, 255, 0, 255};
    SDL_Color red = {255, 0, 0, 255};

    char buffer[128];

    int lineHeight = TTF_FontHeight(font);
    int lineSpacing = 10;
    int groupSpacing = 20;

    int totalHeight = (lineHeight * 2 + groupSpacing) * 3;
    int startY = TextMetricsRect.y*1.5;

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderFillRect(renderer, &TextMetricsRect);

    long long diffMem = metrics.memAccess - oldMetrics.memAccess;
    sprintf(buffer, "Mem Access: %lld", metrics.memAccess);
    SDL_Surface* surfMain1 = TTF_RenderText_Blended(font, buffer, white);
    SDL_Texture* texMain1 = SDL_CreateTextureFromSurface(renderer, surfMain1);

    int x1 = TextMetricsRect.x + (TextMetricsRect.w - surfMain1->w) / 2;
    SDL_Rect rect1 = {x1, startY, surfMain1->w, surfMain1->h};
    SDL_RenderCopy(renderer, texMain1, NULL, &rect1);

    if (oldMetrics.memAccess != 0 && diffMem != 0) {
        sprintf(buffer, "%+lld", diffMem);
        SDL_Color diffColor = diffMem > 0 ? red : green;
        SDL_Surface* surfDiff1 = TTF_RenderText_Blended(font, buffer, diffColor);
        SDL_Texture* texDiff1 = SDL_CreateTextureFromSurface(renderer, surfDiff1);

        int x1Diff = TextMetricsRect.x + (TextMetricsRect.w - surfDiff1->w) / 2;
        SDL_Rect rectDiff1 = {x1Diff, rect1.y + lineHeight + 2, surfDiff1->w, surfDiff1->h};
        SDL_RenderCopy(renderer, texDiff1, NULL, &rectDiff1);

        SDL_FreeSurface(surfDiff1);
        SDL_DestroyTexture(texDiff1);
    }

    SDL_FreeSurface(surfMain1);
    SDL_DestroyTexture(texMain1);

    long long diffComp = metrics.comparisons - oldMetrics.comparisons;
    sprintf(buffer, "Comparisons: %lld", metrics.comparisons);
    SDL_Surface* surfMain2 = TTF_RenderText_Blended(font, buffer, white);
    SDL_Texture* texMain2 = SDL_CreateTextureFromSurface(renderer, surfMain2);

    int y2 = startY + (lineHeight * 2 + groupSpacing);
    int x2 = TextMetricsRect.x + (TextMetricsRect.w - surfMain2->w) / 2;
    SDL_Rect rect2 = {x2, y2, surfMain2->w, surfMain2->h};
    SDL_RenderCopy(renderer, texMain2, NULL, &rect2);

    if (oldMetrics.comparisons != 0 && diffComp != 0) {
        sprintf(buffer, "%+lld", diffComp);
        SDL_Color diffColor = diffComp > 0 ? red : green;
        SDL_Surface* surfDiff2 = TTF_RenderText_Blended(font, buffer, diffColor);
        SDL_Texture* texDiff2 = SDL_CreateTextureFromSurface(renderer, surfDiff2);

        int x2Diff = TextMetricsRect.x + (TextMetricsRect.w - surfDiff2->w) / 2;
        SDL_Rect rectDiff2 = {x2Diff, rect2.y + lineHeight + 2, surfDiff2->w, surfDiff2->h};
        SDL_RenderCopy(renderer, texDiff2, NULL, &rectDiff2);

        SDL_FreeSurface(surfDiff2);
        SDL_DestroyTexture(texDiff2);
    }

    SDL_FreeSurface(surfMain2);
    SDL_DestroyTexture(texMain2);

    double diffTime = metrics.timeElapsed - oldMetrics.timeElapsed;
    sprintf(buffer, "Time : %.6fs", metrics.timeElapsed);
    SDL_Surface* surfMain3 = TTF_RenderText_Blended(font, buffer, white);
    SDL_Texture* texMain3 = SDL_CreateTextureFromSurface(renderer, surfMain3);

    int y3 = startY + 2 * (lineHeight * 2 + groupSpacing);
    int x3 = TextMetricsRect.x + (TextMetricsRect.w - surfMain3->w) / 2;
    SDL_Rect rect3 = {x3, y3, surfMain3->w, surfMain3->h};
    SDL_RenderCopy(renderer, texMain3, NULL, &rect3);

    if (oldMetrics.timeElapsed != 0 && diffTime != 0) {
        sprintf(buffer, "%+.6fs", diffTime);
        SDL_Color diffColor = diffTime > 0 ? red : green;
        SDL_Surface* surfDiff3 = TTF_RenderText_Blended(font, buffer, diffColor);
        SDL_Texture* texDiff3 = SDL_CreateTextureFromSurface(renderer, surfDiff3);

        int x3Diff = TextMetricsRect.x + (TextMetricsRect.w - surfDiff3->w) / 2;
        SDL_Rect rectDiff3 = {x3Diff, rect3.y + lineHeight + 2, surfDiff3->w, surfDiff3->h};
        SDL_RenderCopy(renderer, texDiff3, NULL, &rectDiff3);
        SDL_FreeSurface(surfDiff3);
        SDL_DestroyTexture(texDiff3);
    }

    SDL_FreeSurface(surfMain3);
    SDL_DestroyTexture(texMain3);
}

void drawMenu(SDL_Renderer* renderer, Button* buttons, int numberOfRect, char* sort, TTF_Font* font, Metrics oldMetrics, Metrics metrics) {

    createButtons(buttons);

    float rectMenuWidth = WINDOW_WIDTH / 4.0f;
    SDL_Rect menuRect = {0, 0, rectMenuWidth, WINDOW_HEIGHT};

    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &menuRect);

    SDL_Rect TextSupportRect;
    TextSupportRect.x = WINDOW_WIDTH / 40.0;
    TextSupportRect.y = (5.2 / 8.0) * WINDOW_HEIGHT;
    TextSupportRect.w = rectMenuWidth - TextSupportRect.x * 2.0;
    TextSupportRect.h = WINDOW_HEIGHT / 12.0;

    SDL_Rect TextSupportNumberRect;
    TextSupportNumberRect.x = WINDOW_WIDTH / 40.0;
    TextSupportNumberRect.y = (3.4 / 8.0) * WINDOW_HEIGHT;
    TextSupportNumberRect.w = rectMenuWidth - TextSupportNumberRect.x * 2.0;
    TextSupportNumberRect.h = WINDOW_HEIGHT / 12.0;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &TextSupportRect);
    SDL_RenderFillRect(renderer, &TextSupportNumberRect);

    for (int i = 0; i < 5; i++) {
        drawButton(renderer, &buttons[i]);
    }

    SDL_Color color = {255, 255, 255, 255};

    char numberOfValuesText[64];
    sprintf(numberOfValuesText, "Values: %d", numberOfRect);
    SDL_Surface* textSurfaceNumberRect = TTF_RenderText_Blended(font, numberOfValuesText, color);
    SDL_Texture* textTextureNumberRect = SDL_CreateTextureFromSurface(renderer, textSurfaceNumberRect);
    SDL_FreeSurface(textSurfaceNumberRect);
    renderTextInButton(renderer, font, numberOfValuesText, color, TextSupportNumberRect);

    SDL_DestroyTexture(textTextureNumberRect);

    SDL_Surface* textSurfaceSort = TTF_RenderText_Blended(font, sort, color);
    SDL_Texture* textTextureSort = SDL_CreateTextureFromSurface(renderer, textSurfaceSort);
    SDL_FreeSurface(textSurfaceSort);
    renderTextInButton(renderer, font, sort, color, TextSupportRect);
    SDL_DestroyTexture(textTextureSort);

    SDL_Surface* sPrev = TTF_RenderText_Blended(font, "<", color);
    SDL_Surface* sNext = TTF_RenderText_Blended(font, ">", color);
    SDL_Surface* sStart = TTF_RenderText_Blended(font, "Start", color);

    SDL_Texture* tPrev = SDL_CreateTextureFromSurface(renderer, sPrev);
    SDL_Texture* tNext = SDL_CreateTextureFromSurface(renderer, sNext);
    SDL_Texture* tStart = SDL_CreateTextureFromSurface(renderer, sStart);

    renderTextInButton(renderer, font, "<", color, buttons[0].rect);
    renderTextInButton(renderer, font, ">", color, buttons[1].rect);
    renderTextInButton(renderer, font, "<", color, buttons[2].rect);
    renderTextInButton(renderer, font, ">", color, buttons[3].rect);
    renderTextInButton(renderer, font, "Start", color, buttons[4].rect);

    SDL_FreeSurface(sPrev);
    SDL_FreeSurface(sNext);
    SDL_FreeSurface(sStart);
    SDL_DestroyTexture(tPrev);
    SDL_DestroyTexture(tNext);
    SDL_DestroyTexture(tStart);

    SDL_Rect TextMetricsRect;
    TextMetricsRect.x = WINDOW_WIDTH / 40.0;
    TextMetricsRect.y = WINDOW_HEIGHT / 20.0;
    TextMetricsRect.w = rectMenuWidth - TextMetricsRect.x * 2.0;
    TextMetricsRect.h = WINDOW_HEIGHT / 3.0;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);
    SDL_RenderFillRect(renderer, &TextMetricsRect);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    renderMetrics(renderer, TTF_OpenFont("arial.ttf", WINDOW_WIDTH * 0.018), metrics, oldMetrics, TextMetricsRect);

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

Metrics selectionSort(int* arr, int n, SDL_Renderer* renderer, Metrics metrics) {
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

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect rightArea = { WINDOW_WIDTH / 4, 0, 3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT };
        SDL_RenderFillRect(renderer, &rightArea);

        drawRectangles(renderer, arr, n, arr[i]);
    }
    return metrics;
}

Metrics insertionSort(int* arr, int n, SDL_Renderer* renderer, Metrics metrics) {
    bool running = true;
    bool paused = false;

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

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_Rect rightArea = { WINDOW_WIDTH / 4, 0, 3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT };
            SDL_RenderFillRect(renderer, &rightArea);

            drawRectangles(renderer, arr, n, key);
        }

        arr[j + 1] = key;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect rightArea = { WINDOW_WIDTH / 4, 0, 3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT };
        SDL_RenderFillRect(renderer, &rightArea);

        drawRectangles(renderer, arr, n, key);
    }
    return metrics;
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

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL_Init: %s\n", SDL_GetError());
    }

    if (TTF_Init() == -1) {
        printf("Erreur TTF_Init: %s\n", TTF_GetError());
    }

    TTF_Font* font = TTF_OpenFont("arial.ttf", WINDOW_WIDTH * 0.03);
    if (!font) {
        printf("Erreur TTF_OpenFont: %s\n", TTF_GetError());
    }

    bool running = true;
    SDL_Event event;
    int numberRectList[] = {10, 50, 100, 250, 500, 1000, 5000, 10000};
    char* sortList[] = {"Selection Sort", "Insertion Sort"};
    int sortListIndex = 0;
    int numberRectListIndex = 0;
    Metrics oldMetrics = {0, 0, 0};
    Metrics metrics = {0, 0, 0};


    SDL_RenderClear(renderer);
    Button listButtons[5];
    drawMenu(renderer, listButtons, numberRectList[numberRectListIndex], sortList[sortListIndex], font, oldMetrics, metrics);
    SDL_RenderPresent(renderer);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_WINDOWEVENT){
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    WINDOW_WIDTH = event.window.data1;
                    WINDOW_HEIGHT = event.window.data2;
                    font = TTF_OpenFont("arial.ttf", WINDOW_WIDTH * 0.03);
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderClear(renderer);
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
                }
                if (isButtonClicked(&listButtons[1], mouseX, mouseY)) {
                    numberRectListIndex = (numberRectListIndex + 1) % (sizeof(numberRectList)/sizeof(numberRectList[0]));
                    drawMenu(renderer, listButtons, numberRectList[numberRectListIndex], sortList[sortListIndex], font, oldMetrics, metrics);
                }
                if (isButtonClicked(&listButtons[2], mouseX, mouseY)) {
                    sortListIndex = (sortListIndex + 1) % (sizeof(sortList)/sizeof(sortList[0]));
                    drawMenu(renderer, listButtons, numberRectList[numberRectListIndex], sortList[sortListIndex], font, oldMetrics, metrics);
                }
                if (isButtonClicked(&listButtons[3], mouseX, mouseY)) {
                    sortListIndex = (sortListIndex - 1 + (sizeof(sortList)/sizeof(sortList[0]))) % (sizeof(sortList)/sizeof(sortList[0]));
                    drawMenu(renderer, listButtons, numberRectList[numberRectListIndex], sortList[sortListIndex], font, oldMetrics, metrics);
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
                            metrics = insertionSort(randomNumbers, numberRectList[numberRectListIndex], renderer, metrics);
                            end = SDL_GetPerformanceCounter();
                            break;
                        case 1:
                            start = SDL_GetPerformanceCounter();
                            metrics = selectionSort(randomNumbers, numberRectList[numberRectListIndex], renderer, metrics);
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

gcc fenetre.c -o fenetre `sdl2-config --cflags --libs` -lSDL2_ttf
./fenetre

*/