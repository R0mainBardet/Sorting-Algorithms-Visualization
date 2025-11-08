#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL_ttf.h>

#include "globals.h"
#include "visual.h"
#include "utils.h"

// Selection sort
Metrics selectionSort(int n) {
    bool running = true;
    bool paused = false;
    Uint64 start = 0;
    Uint64 end = 0;
    start = SDL_GetPerformanceCounter();

    for (int i = 0; i < n - 1 && running; i++) {


        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                appQuit = true;
            }

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
            metrics.memAccessSort += 2;
            metrics.comparisons++;
            metrics.comparisonsSort++;
            if (numbers[j] < numbers[minIndex]) {
                minIndex = j;
            }
        }

        metrics.memAccess += 4;
        metrics.memAccessSort += 4;
        float temp = numbers[i];
        numbers[i] = numbers[minIndex];
        numbers[minIndex] = temp;
        
        SDL_Rect rightArea = { WINDOW_WIDTH / 4, 0, 3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT };
        enableEasteregg(rightArea);

        drawRectangles(numbers, n, numbers[i]);

        SDL_RenderPresent(renderer);
        
        end = SDL_GetPerformanceCounter();
        metrics.timeElapsed = (double)(end - start) / SDL_GetPerformanceFrequency();
        drawMenu(numberRectList[numberRectListIndex], sortList[sortListIndex]);
    }

    return metrics;
}


// Insertion sort
Metrics insertionSort(int n) {
    bool running = true;
    bool paused = false;
    Uint64 start = 0;
    Uint64 end = 0;
    start = SDL_GetPerformanceCounter();

    SDL_Rect rightArea = { WINDOW_WIDTH / 4, 0, 3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT };

    for (int i = 1; i < n && running; i++) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                appQuit = true;
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                paused = !paused;
        }

        if (paused) {
            i--;
            continue;
        }

        float key = numbers[i];
        int j = i - 1;

        while (j >= 0 && numbers[j] > key) {
            metrics.memAccess += 4;
            metrics.memAccessSort += 4;
            metrics.comparisons++;
            metrics.comparisonsSort++;
            numbers[j + 1] = numbers[j];
            j--;

            if (easteregg){
                SDL_RenderCopy(renderer, textureFond, NULL, &rightArea);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &rightArea);
            }

            drawRectangles(numbers, n, key);
        }

        numbers[j + 1] = key;

        enableEasteregg(rightArea);

        drawRectangles(numbers, n, key);

        end = SDL_GetPerformanceCounter();
        metrics.timeElapsed = (double)(end - start) / SDL_GetPerformanceFrequency();
        drawMenu(numberRectList[numberRectListIndex], sortList[sortListIndex]);
    }
    return metrics;
}

// Bubble sort
Metrics bubbleSort(int n) {
    bool running = true;
    bool paused = false;
    Uint64 start = 0;
    Uint64 end = 0;
    start = SDL_GetPerformanceCounter();

    for (int i = 0; i < n - 1 && running; i++) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                appQuit = true;
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                paused = !paused;
        }

        if (paused) {
            i--;
            bool stillPaused = true;
            while (stillPaused) {
                SDL_Event e;
                while (SDL_PollEvent(&e)) {
                    if (e.type == SDL_QUIT) {
                        running = false;
                        appQuit = true;
                        stillPaused = false;
                    }
                    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
                        paused = false;
                        stillPaused = false;
                    }
                }
                SDL_Delay(50);
            }
            continue;
        }

        for (int j = 0; j < n - i - 1; j++) {
            metrics.memAccess += 2;
            metrics.memAccessSort += 2;
            metrics.comparisons++;
            metrics.comparisonsSort++;
            if (numbers[j] > numbers[j + 1]) {
                metrics.memAccess += 4;
                metrics.memAccessSort += 4;
                float temp = numbers[j];
                numbers[j] = numbers[j + 1];
                numbers[j + 1] = temp;
            }
        }

        SDL_Rect rightArea = { WINDOW_WIDTH / 4, 0, 3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT };
        enableEasteregg(rightArea);

        drawRectangles(numbers, n, numbers[n - i - 1]);

        SDL_RenderPresent(renderer);

        end = SDL_GetPerformanceCounter();
        metrics.timeElapsed = (double)(end - start) / SDL_GetPerformanceFrequency();
        drawMenu(numberRectList[numberRectListIndex], sortList[sortListIndex]);
    }

    return metrics;
}

// Quick sort
Metrics quicksort(int n) {
    bool running = true;
    bool paused = false;
    Uint64 start = SDL_GetPerformanceCounter();

    SDL_Rect rightArea = { WINDOW_WIDTH / 4, 0, 3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT };

    void quicksortRec(float* numbers, int low, int high) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                appQuit = true;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                paused = !paused;
        }

        if (!running) return;

            while (paused && running) {
            SDL_Delay(10);
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                    appQuit = true;
                }
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                    paused = !paused;
            }
        }

        if (low < high && running) {
            float pivot = numbers[high];
            metrics.memAccess++;
            metrics.memAccessSort++;

            int i = (low - 1);
            for (int j = low; j < high; j++) {
                metrics.memAccess += 2;
                metrics.memAccessSort += 2;
                metrics.comparisons++;
                metrics.comparisonsSort++;

                if (numbers[j] < pivot) {
                    i++;
                    metrics.memAccess += 4;
                    metrics.memAccessSort += 4;
                    float temp = numbers[i];
                    numbers[i] = numbers[j];
                    numbers[j] = temp;
                }

                Uint64 now = SDL_GetPerformanceCounter();
                metrics.timeElapsed = (double)(now - start) / SDL_GetPerformanceFrequency();

                enableEasteregg(rightArea);
                drawRectangles(numbers, n, numbers[j]);
                SDL_RenderPresent(renderer);
                drawMenu(numberRectList[numberRectListIndex], sortList[sortListIndex]);
            }

            float temp = numbers[i + 1];
            numbers[i + 1] = numbers[high];
            numbers[high] = temp;
            int pi = i + 1;

            Uint64 now = SDL_GetPerformanceCounter();
            metrics.timeElapsed = (double)(now - start) / SDL_GetPerformanceFrequency();

            enableEasteregg(rightArea);
            drawRectangles(numbers, n, numbers[pi]);
            SDL_RenderPresent(renderer);
            drawMenu(numberRectList[numberRectListIndex], sortList[sortListIndex]);

            quicksortRec(numbers, low, pi - 1);
            quicksortRec(numbers, pi + 1, high);
        }
    }

    quicksortRec(numbers, 0, n - 1);

    Uint64 end = SDL_GetPerformanceCounter();
    metrics.timeElapsedSort = (double)(end - start) / SDL_GetPerformanceFrequency();

    return metrics;
}

// Bogo Sort
Metrics bogoSort(int n) {
    bool running = true;
    bool paused = false;
    Uint64 start = SDL_GetPerformanceCounter();
    SDL_Rect rightArea = { WINDOW_WIDTH / 4, 0, 3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT };

    while (!isNumberListSorted(numbers, n) && running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                appQuit = true;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                paused = !paused;
        }

        if (paused) {
            SDL_Delay(10);
            continue;
        }

        for (int i = n - 1; i > 0; i--) {
            metrics.memAccess += 2;
            metrics.memAccessSort += 2;
            int j = rand() % (i + 1);
            float temp = numbers[i];
            numbers[i] = numbers[j];
            numbers[j] = temp;
        }

        for (int i = 0; i < n - 1; i++) {
            metrics.comparisons++;
            metrics.comparisonsSort++;
            metrics.memAccess += 2;
            metrics.memAccessSort += 2;
            if (numbers[i] > numbers[i + 1]) {
                break;
            }
        }

        enableEasteregg(rightArea);
        drawRectangles(numbers, n, -1);
        SDL_RenderPresent(renderer);

        Uint64 now = SDL_GetPerformanceCounter();
        metrics.timeElapsed = (double)(now - start) / SDL_GetPerformanceFrequency();
        drawMenu(numberRectList[numberRectListIndex], sortList[sortListIndex]);

        SDL_Delay(50);
    }

    Uint64 end = SDL_GetPerformanceCounter();
    metrics.timeElapsedSort = (double)(end - start) / SDL_GetPerformanceFrequency();

    return metrics;
}

// Merge Sort
Metrics mergeSort(int n) {
    bool running = true;
    bool paused = false;
    Uint64 start = SDL_GetPerformanceCounter();
    SDL_Rect rightArea = { WINDOW_WIDTH / 4, 0, 3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT };

    void merge(float arr[], int left, int mid, int right) {
        int i, j, k;
        int n1 = mid - left + 1;
        int n2 = right - mid;

        float* L = malloc(n1 * sizeof(float));
        float* R = malloc(n2 * sizeof(float));

        for (i = 0; i < n1; i++) {
            metrics.memAccess += 2;
            metrics.memAccessSort += 2;
            L[i] = numbers[left + i];
        }
        for (j = 0; j < n2; j++) {
            metrics.memAccess += 2;
            metrics.memAccessSort += 2;
            R[j] = numbers[mid + 1 + j];
        }

        i = 0;
        j = 0;
        k = left;

        while (i < n1 && j < n2) {
            metrics.comparisons++;
            metrics.comparisonsSort++;
            metrics.memAccess += 2;
            metrics.memAccessSort += 2;

            if (L[i] <= R[j]) {
                numbers[k] = L[i];
                i++;
            } else {
                numbers[k] = R[j];
                j++;
            }
            k++;

            enableEasteregg(rightArea);
            drawRectangles(numbers, n, numbers[k-1]);
            SDL_RenderPresent(renderer);

            Uint64 now = SDL_GetPerformanceCounter();
            metrics.timeElapsed = (double)(now - start) / SDL_GetPerformanceFrequency();
            drawMenu(numberRectList[numberRectListIndex], sortList[sortListIndex]);

            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                    appQuit = true;
                }
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                    paused = !paused;
            }

            while (paused && running) {
                SDL_Delay(10);
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        running = false;
                        appQuit = true;
                    }
                    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                        paused = !paused;
                }
            }

            if (!running) break;
        }

        while (i < n1 && running) {
            metrics.memAccess += 2;
            metrics.memAccessSort += 2;
            numbers[k] = L[i];
            i++;
            k++;

            enableEasteregg(rightArea);
            drawRectangles(numbers, n, numbers[k-1]);
            SDL_RenderPresent(renderer);
        }

        while (j < n2 && running) {
            metrics.memAccess += 2;
            metrics.memAccessSort += 2;
            numbers[k] = R[j];
            j++;
            k++;

            enableEasteregg(rightArea);
            drawRectangles(numbers, n, numbers[k-1]);
            SDL_RenderPresent(renderer);
        }

        free(L);
        free(R);
    }

    void mergeSortRec(float arr[], int left, int right) {
        if (left < right && running) {
            int mid = left + (right - left) / 2;

            mergeSortRec(arr, left, mid);
            mergeSortRec(arr, mid + 1, right);

            merge(arr, left, mid, right);
        }
    }

    mergeSortRec(numbers, 0, n - 1);

    Uint64 end = SDL_GetPerformanceCounter();
    metrics.timeElapsedSort = (double)(end - start) / SDL_GetPerformanceFrequency();

    return metrics;
}

// Heap Sort
Metrics heapSort(int n) {
    bool running = true;
    bool paused = false;
    Uint64 start = SDL_GetPerformanceCounter();
    SDL_Rect rightArea = { WINDOW_WIDTH / 4, 0, 3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT };

    void heapify(float arr[], int n, int i) {
        if (!running) return;

        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        metrics.memAccess += 2;
        metrics.memAccessSort += 2;
        metrics.comparisons++;
        metrics.comparisonsSort++;
        if (left < n && arr[left] > arr[largest])
            largest = left;

        metrics.memAccess += 2;
        metrics.memAccessSort += 2;
        metrics.comparisons++;
        metrics.comparisonsSort++;
        if (right < n && arr[right] > arr[largest])
            largest = right;

        if (largest != i) {
            metrics.memAccess += 4;
            metrics.memAccessSort += 4;
            float temp = arr[i];
            arr[i] = arr[largest];
            arr[largest] = temp;

            enableEasteregg(rightArea);
            drawRectangles(numbers, n, arr[i]);
            SDL_RenderPresent(renderer);

            Uint64 now = SDL_GetPerformanceCounter();
            metrics.timeElapsed = (double)(now - start) / SDL_GetPerformanceFrequency();
            drawMenu(numberRectList[numberRectListIndex], sortList[sortListIndex]);

            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                    appQuit = true;
                }
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                    paused = !paused;
            }

            while (paused && running) {
                SDL_Delay(10);
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        running = false;
                        appQuit = true;
                    }
                    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                        paused = !paused;
                }
            }

            if (!running) return;

            SDL_Delay(10);
            heapify(arr, n, largest);
        }
    }

    for (int i = n / 2 - 1; i >= 0 && running; i--)
        heapify(numbers, n, i);

    for (int i = n - 1; i > 0 && running; i--) {
        metrics.memAccess += 4;
        metrics.memAccessSort += 4;
        float temp = numbers[0];
        numbers[0] = numbers[i];
        numbers[i] = temp;

        enableEasteregg(rightArea);
        drawRectangles(numbers, n, numbers[i]);
        SDL_RenderPresent(renderer);

        Uint64 now = SDL_GetPerformanceCounter();
        metrics.timeElapsed = (double)(now - start) / SDL_GetPerformanceFrequency();
        drawMenu(numberRectList[numberRectListIndex], sortList[sortListIndex]);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                appQuit = true;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                paused = !paused;
        }

        while (paused && running) {
            SDL_Delay(10);
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                    appQuit = true;
                }
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                    paused = !paused;
            }
        }

        if (!running) break;

        SDL_Delay(10);
        heapify(numbers, i, 0);
    }

    Uint64 end = SDL_GetPerformanceCounter();
    metrics.timeElapsedSort = (double)(end - start) / SDL_GetPerformanceFrequency();

    return metrics;
}

// Radix Sort
Metrics radixSort(int n) {
    bool running = true;
    bool paused = false;
    Uint64 start = SDL_GetPerformanceCounter();
    SDL_Rect rightArea = { WINDOW_WIDTH / 4, 0, 3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT };

    float max = numbers[0];
    for (int i = 1; i < n; i++) {
        metrics.memAccess++;
        metrics.memAccessSort++;
        if (numbers[i] > max) {
            max = numbers[i];
        }
    }

    float* output = malloc(n * sizeof(float));
    if (!output) return metrics;

    int scale = 1000;
    for (int exp = 1; (int)(max * scale) / exp > 0 && running; exp *= 10) {
        int count[10] = {0};

        for (int i = 0; i < n; i++) {
            metrics.memAccess++;
            metrics.memAccessSort++;
            int digit = ((int)(numbers[i] * scale) / exp) % 10;
            count[digit]++;
        }

        for (int i = 1; i < 10; i++) {
            count[i] += count[i - 1];
        }

        for (int i = n - 1; i >= 0; i--) {
            metrics.memAccess += 2;
            metrics.memAccessSort += 2;
            int digit = ((int)(numbers[i] * scale) / exp) % 10;
            output[count[digit] - 1] = numbers[i];
            count[digit]--;

            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                    appQuit = true;
                }
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                    paused = !paused;
            }

            while (paused && running) {
                SDL_Delay(10);
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        running = false;
                        appQuit = true;
                    }
                    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                        paused = !paused;
                }
            }

            if (!running) break;
        }

        for (int i = 0; i < n && running; i++) {
            metrics.memAccess += 2;
            metrics.memAccessSort += 2;
            numbers[i] = output[i];

            enableEasteregg(rightArea);
            drawRectangles(numbers, n, numbers[i]);
            SDL_RenderPresent(renderer);

            Uint64 now = SDL_GetPerformanceCounter();
            metrics.timeElapsed = (double)(now - start) / SDL_GetPerformanceFrequency();
            drawMenu(numberRectList[numberRectListIndex], sortList[sortListIndex]);

            SDL_Delay(10);
        }
    }

    free(output);

    Uint64 end = SDL_GetPerformanceCounter();
    metrics.timeElapsedSort = (double)(end - start) / SDL_GetPerformanceFrequency();

    return metrics;
}

// Bucket Sort
Metrics bucketSort(int n) {
    bool running = true;
    bool paused = false;
    Uint64 start = SDL_GetPerformanceCounter();
    SDL_Rect rightArea = { WINDOW_WIDTH / 4, 0, 3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT };

    int num_buckets = n > 0 ? n : 1;
    struct Node {
        float data;
        struct Node* next;
    };
    struct Node** buckets = malloc(num_buckets * sizeof(struct Node*));
    if (!buckets) return metrics;

    for (int i = 0; i < num_buckets; i++) {
        buckets[i] = NULL;
    }

    float max_val = numbers[0], min_val = numbers[0];
    for (int i = 1; i < n; i++) {
        metrics.memAccess++;
        metrics.memAccessSort++;
        if (numbers[i] > max_val) max_val = numbers[i];
        if (numbers[i] < min_val) min_val = numbers[i];
    }
    float range = (max_val - min_val);
    if (range == 0.0f) range = 1.0f;

    for (int i = 0; i < n && running; i++) {
        metrics.memAccess++;
        metrics.memAccessSort++;
        int bucket_index = (int)((numbers[i] - min_val) / range * (num_buckets - 1));
        if (bucket_index < 0) bucket_index = 0;
        if (bucket_index >= num_buckets) bucket_index = num_buckets - 1;

        struct Node* newNode = malloc(sizeof(struct Node));
        if (!newNode) { running = false; break; }
        newNode->data = numbers[i];
        newNode->next = NULL;

        if (!buckets[bucket_index]) {
            buckets[bucket_index] = newNode;
        } else {
            struct Node* current = buckets[bucket_index];
            struct Node* prev = NULL;
            while (current && current->data < newNode->data) {
                metrics.comparisons++;
                metrics.comparisonsSort++;
                prev = current;
                current = current->next;
            }
            if (!prev) {
                newNode->next = buckets[bucket_index];
                buckets[bucket_index] = newNode;
            } else {
                newNode->next = prev->next;
                prev->next = newNode;
            }
        }

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                appQuit = true;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                paused = !paused;
        }

        while (paused && running) {
            SDL_Delay(10);
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                    appQuit = true;
                }
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                    paused = !paused;
            }
        }

        if (!running) break;
    }

    int index = 0;
    for (int i = 0; i < num_buckets && running; i++) {
        struct Node* current = buckets[i];
        while (current && running) {
            metrics.memAccess += 2;
            metrics.memAccessSort += 2;
            numbers[index] = current->data;

            enableEasteregg(rightArea);
            drawRectangles(numbers, n, numbers[index]);
            SDL_RenderPresent(renderer);

            Uint64 now = SDL_GetPerformanceCounter();
            metrics.timeElapsed = (double)(now - start) / SDL_GetPerformanceFrequency();
            drawMenu(numberRectList[numberRectListIndex], sortList[sortListIndex]);

            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                    appQuit = true;
                }
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                    paused = !paused;
            }

            while (paused && running) {
                SDL_Delay(10);
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        running = false;
                        appQuit = true;
                    }
                    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                        paused = !paused;
                }
            }

            if (!running) break;

            SDL_Delay(10);
            
            struct Node* temp = current;
            current = current->next;
            free(temp);
            index++;
        }
        buckets[i] = NULL;
    }

    free(buckets);

    Uint64 end = SDL_GetPerformanceCounter();
    metrics.timeElapsedSort = (double)(end - start) / SDL_GetPerformanceFrequency();

    return metrics;
}
