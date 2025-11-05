#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "globals.h"
#include "utils.h"

// Load the texture
SDL_Texture* chargerTexture(SDL_Renderer* renderer, const char* chemin) {
    SDL_Surface* surface = IMG_Load(chemin);
    if (!surface) {
        printf("IMG_Load Error: %s\n", IMG_GetError());
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

// Draw the button in argument
void drawButton(SDL_Renderer* renderer, Button* button) {
    SDL_SetRenderDrawColor(renderer, button->color.r, button->color.g, button->color.b, 255);
    SDL_RenderFillRect(renderer, &button->rect);
}

// Draw all the bar in the graph with the number list
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

// Put the text in the corresponding rectangle
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

// Create and show the metrics in the menu
void renderMetrics(SDL_Renderer* renderer, TTF_Font* font, Metrics metrics, Metrics oldMetrics, SDL_Rect TextMetricsRect) {

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color green = {0, 255, 0, 255};
    SDL_Color red = {255, 0, 0, 255};

    char buffer[128];

    int lineHeight = TTF_FontHeight(font);
    int lineSpacing = 10;
    int groupSpacing = 30;

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

// Draw the menu and create all the elements it contains
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

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &TextSupportRect);

    for (int i = 0; i < 6; i++) {
        drawButton(renderer, &buttons[i]);
    }

    SDL_Color color = {255, 255, 255, 255};

    char numberOfValuesText[64];
    sprintf(numberOfValuesText, "Values: %d", numberOfRect);
    SDL_Surface* textSurfaceNumberRect = TTF_RenderText_Blended(font, numberOfValuesText, color);
    SDL_Texture* textTextureNumberRect = SDL_CreateTextureFromSurface(renderer, textSurfaceNumberRect);
    SDL_FreeSurface(textSurfaceNumberRect);
    renderTextInButton(renderer, font, numberOfValuesText, color, buttons[5].rect);

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

// Play the music in arguments
void playMusic(const char* musicFile) {
    Mix_Music* music = Mix_LoadMUS(musicFile);
    if (!music) {
        printf("Erreur Mix_LoadMUS: %s\n", Mix_GetError());
        return;
    }
    Mix_PlayMusic(music, 1);
}

// Stop the music
void stopMusic() {
    Mix_HaltMusic();
    Mix_FreeMusic(NULL);
}