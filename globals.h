#include <SDL2/SDL.h>
#include <stdbool.h>

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern bool easteregg;

typedef struct {
    SDL_Rect rect;
    SDL_Color color;
} Button;

typedef struct {
    long long memAccess;
    long long comparisons;
    double timeElapsed;
} Metrics;