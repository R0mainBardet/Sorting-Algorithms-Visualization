SDL_Texture* chargerTexture(const char* chemin);

void drawButton(Button* button);

void drawRectangles(float* arr, int n, int actualValue);

void renderTextInButton(const char* text, SDL_Color color, SDL_Rect buttonRect);

void renderMetrics(TTF_Font* font, SDL_Rect TextMetricsRect);

void drawMenu(int numberOfRect, char* sort);

void playMusic(const char* musicFile);

void stopMusic();