#include <SDL2/SDL.h>
#include <stdio.h>

int main() {
    // Create a window
    SDL_Window* window = SDL_CreateWindow("Baptiste la salope",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          800, 600,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    
    // Wait 3 seconds
    SDL_Delay(3000);

    // Destroy window and quit SDL
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
