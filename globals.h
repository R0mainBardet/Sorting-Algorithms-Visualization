#include <SDL2/SDL.h>
#include <stdbool.h>

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern bool easteregg;
extern bool isInteger;
extern float* numbers;
extern int numberRectList[];
extern char* sortList[];
extern int sortListIndex;
extern int numberRectListIndex;
extern int mouseX;
extern int mouseY;
extern SDL_Texture* textureFond;
extern SDL_Renderer* renderer;
extern TTF_Font* font;
extern TTF_Font* fontMetrics;

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