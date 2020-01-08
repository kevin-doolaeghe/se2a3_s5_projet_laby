#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

unsigned int m; // Width
unsigned int n; // Height
bool** maze;

//************************************

void initRand() {
    srand(time(NULL));
}

void askDimensions() {
    printf("Entrez une valeur de M (max. 80) : ");
    scanf("%u", &m);

    printf("Entrez une valeur de N (max. 25) : ");
    scanf("%u", &n);
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

void fillMaze() {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            tab[i][j] = rand() % 2;
        }
    }

    // Borders
    for (int i = 0; i < m; i++) {
        tab[i][0] = 1;
        tab[i][m - 1] = 1;
    }

    for (int i = 0; i < n; i++) {
        tab[0][i] = 1;
        tab[n - 1][0] = 1;
    }

    tab[0][1] = 0;
    tab[0][n - 1] = 0;
}

void initMaze() {

}

void clearTerminal() {
    system("clear");
}

void resizeTerminal(int lines, int columns) {
    system("resize -s " + lines + " " + columns);
}

void displayMaze() {

}

void displaySolution() {

}

void loop() {
    do {

    } while ();
}

//************************************

int main() {
    initRand();
    askDimensions();
    createMaze(m, n);

    clearTerminal();
    resizeTerminal();

    loop();

    destroyMaze();

    return 0;
}
