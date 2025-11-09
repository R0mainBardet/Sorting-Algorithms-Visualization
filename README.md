# Baptiste LORTON and Romain Bardet

## Run the project

### Libraries

- **SDL2** : for window and rendering management
- **SDL2_ttf** : for TrueType font support
- **SDL2_image** : for loading images
- **SDL2_mixer** : for audio playback

### Compilation

```bash
gcc main.c utils.c sorting.c visual.c -o sorting `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_image -lSDL2_mixer
```

### Run

```bash
./sorting
```

## Sorting Algorithms

This application implements and visualizes nine different sorting algorithms:

### Selection Sort
- Works by repeatedly finding the minimum element from the unsorted portion and placing it at the beginning
- Time Complexity: O(n²) in all cases
- Space Complexity: O(1)
- Best suited for small arrays
- Minimizes the number of swaps compared to bubble sort

### Insertion Sort
- Builds the final sorted array one item at a time
- Time Complexity: O(n²) in average and worst case, O(n) in best case
- Space Complexity: O(1)
- Very efficient for small data sets
- Adaptive: performance improves if data is already partially sorted

### Bubble Sort
- Simple algorithm that repeatedly steps through the list, compares adjacent elements and swaps them if they are in the wrong order
- Time Complexity: O(n²) in average and worst case, O(n) in best case
- Space Complexity: O(1)
- Easy to understand and implement
- Not suitable for large datasets

### Quicksort
- Uses divide-and-conquer strategy with a pivot element
- Time Complexity: O(n log n) average case, O(n²) worst case
- Space Complexity: O(log n)
- One of the fastest sorting algorithms in practice
- Very efficient for large datasets

### Merge Sort
- Uses the divide-and-conquer strategy to sort the array
- Time Complexity: O(n log n) in all cases
- Space Complexity: O(n)
- Stable sorting algorithm
- Efficient for large datasets but requires additional memory

### Bogo Sort (Permutation Sort)
- Randomly shuffles the array until it's sorted
- Time Complexity: O(n × n!) average case, unbounded worst case
- Space Complexity: O(1)
- Not practical for real use
- Included as an educational example of an inefficient algorithm

### Heap Sort
- Uses a binary heap data structure
- Time Complexity: O(n log n) in all cases
- Space Complexity: O(1)
- In-place sorting algorithm
- Efficient for large datasets

### Radix Sort
- Non-comparative integer sorting algorithm
- Time Complexity: O(d × n) where d is the number of digits
- Space Complexity: O(n + k) where k is the range of values
- Very efficient for integers or strings
- Sorts data digit by digit

### Bucket Sort
- Distributes elements into buckets then sorts them individually
- Time Complexity: O(n + k) average case, O(n²) worst case
- Space Complexity: O(n + k)
- Very efficient when input is uniformly distributed
- Good for floating-point numbers in a known range

### Performance Metrics
Each sorting algorithm's performance is measured using three metrics:
- **Memory Access**: Number of times the algorithm reads or writes to memory
- **Comparisons**: Number of times two elements are compared
- **Time Elapsed**: Actual time taken to complete the sort

The metrics are displayed in real-time and compared with the previous sort, helping users understand the efficiency differences between algorithms.

## How to ue the application

### With the keyboard

Press **T** to toggle the type of numbers between **int** and **float**.

- If the values are integers → bars go from blue to pink
- If the values are floats → bars go from purple to green

Press **R** to randomize the list of numbers.

Press **Spacebar** to pause or resume the sorting.

### In the window

Change the number of values to sort using the arrows under "**Values: n**".

Select the sorting algorithm using the arrows under "**Name of the sort**".

Click the **Start** button to launch the sort and visualize it live.

- At the end of the sort, the button label changes to **Random** so you can shuffle the numbers again.

### Metrics

The top-left corner displays **metrics** that allow you to compare the number of **memory accesses**, **comparisons**, and **elapsed time** between the **current** sort and the **previous** sort.

## Easteregg

### Clue

*I am not crazy! I know he swapped those numbers, I knew it was 1216! One after Magna Carta, as if I could ever make such a mistake! Never! Never!*

### Solution

- Select 1261 values and click on "**1261**".
- The number of values automatically changes to **1216**.
- The background changes to a picture of **Saul Goodman**.
- The **intro music** of **Better Call Saul** plays.

Click on 1216 to go back to a black background. 

### Reference

This is from the episode "**Chicanery**" (S3.E5) of **Better Call Saul**. In this scene, **Chuck McGill**, the brother of **Jimmy McGill** (Saul Goodman), says this iconic sentence.