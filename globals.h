#include <SDL2/SDL.h>
#include <stdbool.h>

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern bool easteregg;
extern int numberRectList[];
extern char* sortList[];
extern int sortListIndex;
extern int numberRectListIndex;
extern int mouseX;
extern int mouseY;

typedef struct {
    SDL_Rect rect;
    SDL_Color color;
} Button;

extern Button listButtons[];

typedef struct {
    long long memAccess;
    long long comparisons;
    double timeElapsed;
} Metrics;

extern Metrics oldMetrics;
extern Metrics metrics;