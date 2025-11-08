# New sorting algorithms to be added after heapSort in sorting.c

// Radix Sort
Metrics radixSort(int n) {
    bool running = true;
    bool paused = false;
    Uint64 start = SDL_GetPerformanceCounter();
    SDL_Rect rightArea = { WINDOW_WIDTH / 4, 0, 3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT };

    // Find the maximum number to know number of digits
    float max = numbers[0];
    for (int i = 1; i < n; i++) {
        metrics.memAccess++;
        metrics.memAccessSort++;
        if (numbers[i] > max) {
            max = numbers[i];
        }
    }

    // Do counting sort for every digit
    float* output = malloc(n * sizeof(float));
    if (!output) return metrics;

    // Since we're dealing with floats, we'll multiply by 1000 to handle up to 3 decimal places
    int scale = 1000;
    for (int exp = 1; max * scale / exp > 0 && running; exp *= 10) {
        int count[10] = {0};

        // Store count of occurrences
        for (int i = 0; i < n; i++) {
            metrics.memAccess++;
            metrics.memAccessSort++;
            int digit = ((int)(numbers[i] * scale) / exp) % 10;
            count[digit]++;
        }

        // Change count[i] so that count[i] contains actual
        // position of this digit in output[]
        for (int i = 1; i < 10; i++) {
            count[i] += count[i - 1];
        }

        // Build the output array
        for (int i = n - 1; i >= 0; i--) {
            metrics.memAccess += 2;
            metrics.memAccessSort += 2;
            int digit = ((int)(numbers[i] * scale) / exp) % 10;
            output[count[digit] - 1] = numbers[i];
            count[digit]--;

            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT)
                    running = false;
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                    paused = !paused;
            }

            while (paused && running) {
                SDL_Delay(10);
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT)
                        running = false;
                    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                        paused = !paused;
                }
            }

            if (!running) break;
        }

        // Copy the output array to numbers[]
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

    // Create buckets
    int num_buckets = n;
    struct Node {
        float data;
        struct Node* next;
    };
    struct Node** buckets = malloc(num_buckets * sizeof(struct Node*));
    if (!buckets) return metrics;

    for (int i = 0; i < num_buckets; i++) {
        buckets[i] = NULL;
    }

    // Find range of values
    float max_val = numbers[0], min_val = numbers[0];
    for (int i = 1; i < n; i++) {
        metrics.memAccess++;
        metrics.memAccessSort++;
        if (numbers[i] > max_val) max_val = numbers[i];
        if (numbers[i] < min_val) min_val = numbers[i];
    }
    float range = (max_val - min_val) / num_buckets;

    // Put array elements in different buckets
    for (int i = 0; i < n && running; i++) {
        metrics.memAccess++;
        metrics.memAccessSort++;
        int bucket_index = (int)((numbers[i] - min_val) / range);
        if (bucket_index >= num_buckets) bucket_index = num_buckets - 1;

        struct Node* newNode = malloc(sizeof(struct Node));
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
            if (event.type == SDL_QUIT)
                running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                paused = !paused;
        }

        while (paused && running) {
            SDL_Delay(10);
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT)
                    running = false;
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                    paused = !paused;
            }
        }

        if (!running) break;
    }

    // Put back elements from buckets into array
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
                if (event.type == SDL_QUIT)
                    running = false;
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                    paused = !paused;
            }

            while (paused && running) {
                SDL_Delay(10);
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT)
                        running = false;
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