#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_NAME "Sorting algorithm visualization"
#define INITIAL_WIDTH 800
#define INITIAL_HEIGHT 600
#define FONT_PATH "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"
#define FONT_SIZE 16

typedef enum {
    ALGO_BUBBLE,
    ALGO_SELECTION,
    ALGO_INSERTION,
    ALGO_QUICK,
    ALGO_MERGE,
    ALGO_COUNT
} Algorithm;

const char* algo_names[ALGO_COUNT] = {
    "Bubble", "Selection", "Insertion", "QuickSort", "MergeSort"
};

typedef struct {
    long comparisons;
    long accesses;
    Uint32 start_time;
    Uint32 elapsed_time;
    int running;
} Stats;

void stats_reset(Stats *s) {
    s->comparisons = 0;
    s->accesses = 0;
    s->elapsed_time = 0;
    s->running = 0;
}

void stats_start(Stats *s) {
    s->start_time = SDL_GetTicks();
    s->running = 1;
}

void stats_update(Stats *s) {
    if (s->running) s->elapsed_time = SDL_GetTicks() - s->start_time;
}

void randomize_tableau(int *tableau, int n) {
    for (int i = 0; i < n; i++) {
        int j = rand() % n;
        int tmp = tableau[i];
        tableau[i] = tableau[j];
        tableau[j] = tmp;
    }
}

void draw_text_centered(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, int w, int h, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text, color);
    if (!surface) return;
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = { x + (w - surface->w)/2, y + (h - surface->h)/2, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void draw_left(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text, color);
    if (!surface) return;
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void draw(SDL_Renderer *renderer, TTF_Font *font, int *tab, int n, int ww, int wh, int ci, int cj, Algorithm algo, Stats *stats) {
    SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
    SDL_RenderClear(renderer);

    int mw = ww / 4;
    SDL_Rect menu = {0, 0, mw, wh};
    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
    SDL_RenderFillRect(renderer, &menu);

    SDL_Color dark = {40,40,40,255}, black = {0,0,0,255}, white={255,255,255,255};

    draw_text_centered(renderer, font, "Sorting Visualizer", 10, 10, mw-20, 40, dark);
    draw_text_centered(renderer, font, "Algo", 10, 60, mw-20, 28, black);
    draw_text_centered(renderer, font, algo_names[algo], 10, 90, mw-20, 28, dark);

    char buf[128];
    snprintf(buf, sizeof(buf), "N = %d", n); draw_text_centered(renderer, font, buf, 10, 160, mw-20, 28, dark);
    snprintf(buf, sizeof(buf), "Comparisons: %ld", stats->comparisons); draw_left(renderer, font, buf, 10, 200, black);
    snprintf(buf, sizeof(buf), "Memory accesses: %ld", stats->accesses); draw_left(renderer, font, buf, 10, 220, black);
    snprintf(buf, sizeof(buf), "Time: %u ms", stats->elapsed_time); draw_left(renderer, font, buf, 10, 240, black);

    const char* controls[] = {
        "Q: Quit",
        "R: Randomize",
        "Space: Start Sorting",
        "Left/Right: Change Algorithm",
        "Up/Down: Change Size"
    };
    for(int i=0;i<5;i++) draw_left(renderer,font,controls[i],10,280+i*20,black);

    int gx = mw, gw = ww - mw, gh = wh;
    float wbar = (float)gw / n, px = gx;
    for (int i = 0; i < n; i++) {
        SDL_Rect bar = { (int)px, gh - (tab[i]*gh)/n, (int)(wbar+0.5f), (tab[i]*gh)/n };
        SDL_Color color;
        if(i==ci || i==cj){
            color = white;
        } else {
            float ratio = (float)i / n;
            color.r = (Uint8)(ratio*255);
            color.g = (Uint8)((1-ratio)*128);
            color.b = (Uint8)(255 - ratio*255);
            color.a = 255;
        }
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &bar);
        px += wbar;
    }

    SDL_RenderPresent(renderer);
}

void bubble_sort(SDL_Renderer *r, TTF_Font *f, int *t, int n, int ww, int wh, Algorithm a, Stats *s) {
    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-i-1; j++) {
            s->comparisons++; s->accesses+=2;
            if (t[j] > t[j+1]) { int tmp=t[j]; t[j]=t[j+1]; t[j+1]=tmp; s->accesses+=4; }
            draw(r,f,t,n,ww,wh,j,j+1,a,s); SDL_Delay(1);
            stats_update(s);
        }
}

void selection_sort(SDL_Renderer *r, TTF_Font *f, int *t, int n, int ww, int wh, Algorithm a, Stats *s) {
    for(int i=0;i<n-1;i++){
        int min=i; s->accesses++;
        for(int j=i+1;j<n;j++){
            s->comparisons++; s->accesses++;
            if(t[j]<t[min]) min=j;
            draw(r,f,t,n,ww,wh,i,j,a,s); SDL_Delay(1);
            stats_update(s);
        }
        if(min!=i){ int tmp=t[i]; t[i]=t[min]; t[min]=tmp; s->accesses+=4; draw(r,f,t,n,ww,wh,i,min,a,s); SDL_Delay(1);}
    }
}

void insertion_sort(SDL_Renderer *r, TTF_Font *f, int *t, int n, int ww, int wh, Algorithm a, Stats *s) {
    for(int i=1;i<n;i++){
        int key=t[i]; s->accesses++; int j=i-1;
        while(j>=0){ s->comparisons++; s->accesses++; if(t[j]<=key) break; t[j+1]=t[j]; s->accesses++; j--; draw(r,f,t,n,ww,wh,j,j+1,a,s); SDL_Delay(1); stats_update(s);}
        t[j+1]=key; s->accesses++; draw(r,f,t,n,ww,wh,j+1,i,a,s); SDL_Delay(1); stats_update(s);
    }
}

void quick_sort_recursive(SDL_Renderer *r, TTF_Font *f, int *t, int low, int high, int ww, int wh, Algorithm a, Stats *s) {
    if(low<high){
        int pivot=t[high]; s->accesses++; int i=low-1;
        for(int j=low;j<high;j++){
            s->comparisons++; s->accesses++;
            if(t[j]<=pivot){ i++; int tmp=t[i]; t[i]=t[j]; t[j]=tmp; s->accesses+=4; }
            draw(r,f,t,high+1,ww,wh,i,j,a,s); SDL_Delay(1); stats_update(s);
        }
        int tmp=t[i+1]; t[i+1]=t[high]; t[high]=tmp; s->accesses+=4; draw(r,f,t,high+1,ww,wh,i+1,high,a,s); SDL_Delay(1); stats_update(s);
        int pi=i+1;
        quick_sort_recursive(r,f,t,low,pi-1,ww,wh,a,s);
        quick_sort_recursive(r,f,t,pi+1,high,ww,wh,a,s);
    }
}

void quick_sort(SDL_Renderer *r, TTF_Font *f, int *t, int n, int ww, int wh, Algorithm a, Stats *s) {
    quick_sort_recursive(r,f,t,0,n-1,ww,wh,a,s);
}

void merge(SDL_Renderer *r, TTF_Font *f, int *t, int l, int m, int r_idx, int ww, int wh, Algorithm a, Stats *s) {
    int n1=m-l+1, n2=r_idx-m;
    int *L=malloc(n1*sizeof(int)), *R=malloc(n2*sizeof(int));
    for(int i=0;i<n1;i++){ L[i]=t[l+i]; s->accesses++; }
    for(int j=0;j<n2;j++){ R[j]=t[m+1+j]; s->accesses++; }
    int i=0,j=0,k=l;
    while(i<n1 && j<n2){
        s->comparisons++; s->accesses+=2;
        if(L[i]<=R[j]){ t[k]=L[i]; i++; s->accesses++; } else { t[k]=R[j]; j++; s->accesses++; }
        k++; draw(r,f,t,r_idx+1,ww,wh,k-1,k-1,a,s); SDL_Delay(1); stats_update(s);
    }
    while(i<n1){ t[k]=L[i]; i++; k++; s->accesses++; }
    while(j<n2){ t[k]=R[j]; j++; k++; s->accesses++; }
    free(L); free(R);
}

void merge_sort_recursive(SDL_Renderer *r, TTF_Font *f, int *t, int l, int r_idx, int ww, int wh, Algorithm a, Stats *s) {
    if(l<r_idx){
        int m=l+(r_idx-l)/2;
        merge_sort_recursive(r,f,t,l,m,ww,wh,a,s);
        merge_sort_recursive(r,f,t,m+1,r_idx,ww,wh,a,s);
        merge(r,f,t,l,m,r_idx,ww,wh,a,s);
    }
}

void merge_sort(SDL_Renderer *r, TTF_Font *f, int *t, int n, int ww, int wh, Algorithm a, Stats *s) {
    merge_sort_recursive(r,f,t,0,n-1,ww,wh,a,s);
}

int main() {
    srand(time(NULL));
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    TTF_Font *font = TTF_OpenFont(FONT_PATH,FONT_SIZE);
    int ww=INITIAL_WIDTH, wh=INITIAL_HEIGHT;
    SDL_Window *win = SDL_CreateWindow(WINDOW_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ww, wh, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_Renderer *r = SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED);

    int n=100; int *t = malloc(n*sizeof(int)); for(int i=0;i<n;i++) t[i]=i+1; randomize_tableau(t,n);
    Algorithm algo=ALGO_BUBBLE; int running=1, sorting=0;
    int ci=-1,cj=-1; SDL_Event e;
    Stats stats; stats_reset(&stats);

    while(running){
        while(SDL_PollEvent(&e)){
            if(e.type==SDL_QUIT) running=0;
            else if(e.type==SDL_WINDOWEVENT && e.window.event==SDL_WINDOWEVENT_RESIZED){ ww=e.window.data1; wh=e.window.data2; }
            else if(e.type==SDL_KEYDOWN){
                SDL_Keycode k = e.key.keysym.sym;
                if(k==SDLK_q) running=0;
                else if(k==SDLK_r){ randomize_tableau(t,n); sorting=0; stats_reset(&stats); }
                else if(k==SDLK_SPACE){ sorting=1; stats_reset(&stats); stats_start(&stats); }
                else if(k==SDLK_LEFT) algo=(algo-1+ALGO_COUNT)%ALGO_COUNT;
                else if(k==SDLK_RIGHT) algo=(algo+1)%ALGO_COUNT;
                else if(k==SDLK_UP){ int nn=n+10; if(nn>2000) nn=2000; if(nn!=n){ free(t); n=nn; t=malloc(n*sizeof(int)); for(int i=0;i<n;i++) t[i]=i+1; randomize_tableau(t,n); sorting=0; stats_reset(&stats);} }
                else if(k==SDLK_DOWN){ int nn=n-10; if(nn<10) nn=10; if(nn!=n){ free(t); n=nn; t=malloc(n*sizeof(int)); for(int i=0;i<n;i++) t[i]=i+1; randomize_tableau(t,n); sorting=0; stats_reset(&stats);} }
            }
        }

        if(sorting){
            switch(algo){
                case ALGO_BUBBLE: bubble_sort(r,font,t,n,ww,wh,algo,&stats); break;
                case ALGO_SELECTION: selection_sort(r,font,t,n,ww,wh,algo,&stats); break;
                case ALGO_INSERTION: insertion_sort(r,font,t,n,ww,wh,algo,&stats); break;
                case ALGO_QUICK: quick_sort(r,font,t,n,ww,wh,algo,&stats); break;
                case ALGO_MERGE: merge_sort(r,font,t,n,ww,wh,algo,&stats); break;
            }
            stats.running=0;
            sorting=0;
        }

        stats_update(&stats);
        draw(r,font,t,n,ww,wh,ci,cj,algo,&stats);
        SDL_Delay(16);
    }

    free(t);
    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(win);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
