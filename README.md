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

## Sortings

ROMAIN EXPLIQUE LES SORT

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