SDL_Texture* chargerTexture(SDL_Renderer* renderer, const char* chemin);

void drawButton(SDL_Renderer* renderer, Button* button);

void drawRectangles(SDL_Renderer* renderer, int* numbers, int n, int actualValue);

void renderTextInButton(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color, SDL_Rect buttonRect);

void renderMetrics(SDL_Renderer* renderer, TTF_Font* font, Metrics metrics, Metrics oldMetrics, SDL_Rect TextMetricsRect);

void drawMenu(SDL_Renderer* renderer, Button* buttons, int numberOfRect, char* sort, TTF_Font* font, Metrics oldMetrics, Metrics metrics);

void playMusic(const char* musicFile);

void stopMusic();