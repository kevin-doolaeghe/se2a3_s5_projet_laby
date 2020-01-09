#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <curses.h>

const unsigned int WINDOW_WIDTH = 120;
const unsigned int WINDOW_HEIGHT = 40;
const char* FILE_PATH = "~/.maze.save";

unsigned int m; // Width
unsigned int n; // Height
bool** maze;
bool** solution;

//************************************

void initRand() {
    srand(time(NULL));
}

void setDimensions() {

}

void createMaze(int m, int n) {
    maze = malloc(m * sizeof(int));
    for (int i = 0; i < n; i++) {
        maze[i] = malloc(n * sizeof(int));
    }
}

void destroyMaze(bool** maze) {
    free(maze);
}

void generateMaze() {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            tab[i][j] = rand() % 2;
        }
    }

    // Borders
    for (int i = 0; i < m; i++) {
        maze[i][0] = 1;
        maze[i][m - 1] = 1;
    }

    for (int i = 0; i < n; i++) {
        maze[0][i] = 1;
        maze[n - 1][0] = 1;
    }

    maze[0][1] = 0;
    maze[0][n - 1] = 0;
}

void solveMaze() {

}

void clearTerminal() {
    system("clear");
}

void resizeTerminal(unsigned int columns, unsigned int lines) {
    system("resize -s " + lines + " " + columns);
}

void displayMaze() {
    int upperLeftCornerX = (WINDOW_WIDTH - m) / 2;
    int upperLeftCornerY = (WINDOW_HEIGHT - n) / 2;

    move(upperLeftCornerX, upperLeftCornerY);
    refresh();
}

void displayMenus() {

}

void displaySolution() {

}

void saveMaze(char* filePath) {

}

void loadMaze(char* filePath) {

}

void loop() {
    do {

    } while ();
}

//************************************

int main() {
    initRand();
    setDimensions();
    createMaze(m, n);

    clearTerminal();
    resizeTerminal(WINDOW_WIDTH, WINDOW_HEIGHT);

    loop();

    destroyMaze();

    return 0;
}
