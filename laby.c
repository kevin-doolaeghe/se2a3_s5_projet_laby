#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <string.h>

/*
 * =====================================================================================================================
 * Initialisation
 * =====================================================================================================================
 */

#define KEY_ESCAPE  27
#define KEY_ENTER   13
#define KEY_UP      65
#define KEY_DOWN    66
#define KEY_LEFT    68
#define KEY_RIGHT   67

#define BLACK       30
#define RED         31
#define GREEN       32
#define YELLOW      33
#define BLUE        34
#define PURPLE      35
#define CYAN        36
#define WHITE       37

const unsigned int COLOR_FONT_DEFAULT = BLACK;
const unsigned int COLOR_BG = WHITE;

const unsigned int WINDOW_WIDTH = 150;
const unsigned int WINDOW_HEIGHT = 40;
const unsigned int MENU_WIDTH = 30;
const unsigned int MENU_HEIGHT = 13;
const unsigned int MARGIN = 3;

const char *FILE_PATH = "maze.save";

unsigned int m; // Width
unsigned int n; // Height
bool **maze;
bool **solution;

bool isMazeCreated = false;
bool isSolutionCreated = false;

/*
 * =====================================================================================================================
 * Génération
 * =====================================================================================================================
 */

void initRand() {
    srand(time(NULL));
}

void setDimensions() {
    m = rand() % 26 + 30; // rand() % 19 + 60
    n = rand() % 12 + 14; // rand() % 10 + 15

    if (m % 2 == 0) m++;
    if (n % 2 == 0) n++;
}

void createMaze(unsigned int m, unsigned int n) {
    maze = (bool **) malloc(m * sizeof(bool *)); // Columns
    for (unsigned int i = 0; i < m; i++) {
        maze[i] = (bool *) malloc(n * sizeof(bool)); // Lines
    }

    isMazeCreated = true;
}

void destroyMaze(unsigned int m, unsigned int n) {
    for (unsigned int i = 0; i < m; i++) free(maze[i]);
    free(maze);

    isMazeCreated = false;
}

void createSolution(unsigned int m, unsigned int n) {
    solution = (bool **) malloc(m * sizeof(bool *));
    for (unsigned int i = 0; i < m; i++) {
        solution[i] = (bool *) malloc(n * sizeof(bool));
    }

    isSolutionCreated = true;
}

void destroySolution(unsigned int m, unsigned int n) {
    for (unsigned int i = 0; i < m; i++) free(solution[i]);
    free(solution);

    isSolutionCreated = false;
}

// =====================================================================================================================

bool isCellFree(bool **maze, unsigned int m, unsigned int n, unsigned int x, unsigned int y) {
    if (maze[x - 1][y] == 1 && maze[x][y - 1] == 1 && maze[x + 1][y] == 1 && maze[x][y + 1] == 1) {
        return true;
    } else {
        return false;
    }
}

bool isUpAvailable(bool **maze, unsigned int m, unsigned int n, unsigned int x, unsigned int y) {
    bool isUpAvailable = false;
    if (y > 1) {
        if (isCellFree(maze, m, n, x, y - 2) == true) isUpAvailable = true;
    }

    return isUpAvailable;
}

bool isDownAvailable(bool **maze, unsigned int m, unsigned int n, unsigned int x, unsigned int y) {
    bool isDownAvailable = false;
    if (y < n - 2) {
        if (isCellFree(maze, m, n, x, y + 2) == true) isDownAvailable = true;
    }

    return isDownAvailable;
}

bool isLeftAvailable(bool **maze, unsigned int m, unsigned int n, unsigned int x, unsigned int y) {
    bool isLeftAvailable = false;
    if (x > 1) {
        if (isCellFree(maze, m, n, x - 2, y) == true) isLeftAvailable = true;
    }

    return isLeftAvailable;
}

bool isRightAvailable(bool **maze, unsigned int m, unsigned int n, unsigned int x, unsigned int y) {
    bool isRightAvailable = false;
    if (x < m - 2) {
        if (isCellFree(maze, m, n, x + 2, y) == true) isRightAvailable = true;
    }

    return isRightAvailable;
}

bool isBlocked(bool **maze, unsigned int m, unsigned int n, unsigned int x, unsigned int y) {
    bool isBlocked = true;

    if (isUpAvailable(maze, m, n, x, y) == true
    || isDownAvailable(maze, m, n, x, y) == true
    || isLeftAvailable(maze, m, n, x, y) == true
    || isRightAvailable(maze, m, n, x, y) == true) {
        isBlocked = false;
    }

    return isBlocked;
}

void fillMaze() {
    for (unsigned int i = 0; i < m; i++) {
        for (unsigned int j = 0; j < n; j++) {
            if (i % 2 == 1 && j % 2 == 1) {
                if (isCellFree(maze, m, n, i, j) == true) {
                    unsigned int direction = 0;
                    unsigned int x = i;
                    unsigned int y = j;

                    bool isRunning = true;

                    while (isRunning) {
                        unsigned int temp = direction;
                        do { // Empèche le retour en arrière
                            direction = rand() % 4;
                        } while (direction == temp);

                        switch (direction) {
                        case 0: // Haut
                            if (y != 1) {
                                maze[x][y - 1] = 0;
                                if (isCellFree(maze, m, n, x, y - 2) == true) {
                                    y -= 2;
                                } else {
                                    isRunning = false;
                                }
                            } else {
                                isRunning = false;
                            }
                            break;
                        case 1: // Bas
                            if (y != n - 2) {
                                maze[x][y + 1] = 0;
                                if (isCellFree(maze, m, n, x, y + 2) == true) {
                                    y += 2;
                                } else {
                                    isRunning = false;
                                }
                            } else {
                                isRunning = false;
                            }
                            break;
                        case 2: // Gauche
                            if (x != 1) {
                                maze[x - 1][y] = 0;
                                if (isCellFree(maze, m, n, x - 2, y) == true) {
                                    x -= 2;
                                } else {
                                    isRunning = false;
                                }
                            } else {
                                isRunning = false;
                            }
                            break;
                        case 3: // Droite
                            if (x != m - 2) {
                                maze[x + 1][y] = 0;
                                if (isCellFree(maze, m, n, x + 2, y) == true) {
                                    x += 2;
                                } else {
                                    isRunning = false;
                                }
                            } else {
                                isRunning = false;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
}

void generateMaze() {
    if (isMazeCreated == true) destroyMaze(m, n);
    if (isSolutionCreated == true) destroySolution(m, n);

    setDimensions();
    createMaze(m, n);

    for (unsigned int i = 0; i < m; i++) {
        for (unsigned int j = 0; j < n; j++) {
            if (i % 2 == 1 && j % 2 == 1) maze[i][j] = 0;
            else maze[i][j] = 1;
        }
    }

    fillMaze();

    maze[0][1] = 0;
    maze[m - 1][n - 2] = 0;
}

void fillSolution() {
    
}

void solveMaze() {
    if (isSolutionCreated == true) destroySolution(m, n);

    if (isMazeCreated == true) {
        createSolution(m, n);

        for (unsigned int i = 0; i < m; i++) {
            for (unsigned int j = 0; j < n; j++) {
                solution[i][j] = 0;
            }
        }

        fillSolution();
    }
}

/*
 * =====================================================================================================================
 * Affichage
 * =====================================================================================================================
 */

void clearTerminal() {
    system("clear");
}

void resizeTerminal(unsigned int lines, unsigned int columns) {
    char* lines_str = (char *) malloc(3 * sizeof(char));
    sprintf((char*) lines_str,"%d",lines);
    char* columns_str = (char *) malloc(3 * sizeof(char));
    sprintf((char*) columns_str,"%d",columns);

    char* cmd = (char *) malloc(40 * sizeof(char));;
    cmd = strcat(cmd, "resize -s ");
    cmd = strcat(cmd, lines_str);
    cmd = strcat(cmd, " ");
    cmd = strcat(cmd, columns_str);

	system(cmd);
}

void moveCursor(unsigned int x, unsigned int y) {
    printf("\033[%d;%df", y, x);
}

void setColor(unsigned int charColor, unsigned int backgroundColor) {
    printf("\033[1;%dm""\033[1;%dm\n", charColor, backgroundColor + 10);
}

int getch()
{
	system("stty raw -echo"); // Paramétrage pour la saisie
	int key = getchar();
	system("stty cooked echo");

	return key;
}

// =====================================================================================================================

static void displayBorders(unsigned int longueur, unsigned int x, unsigned int y)
{
    unsigned int posX;
    unsigned int posY;
    unsigned int i;

    setColor(RED, COLOR_BG);
    posX = x - 1;
    posY = y - 1;
    moveCursor(posX, posY);
    printf("╔");
    for(i = 0; i < longueur - 2; i++) printf("═");
    printf("╗");

    posY++;
    moveCursor(posX, posY);
    printf("║");

    posX += longueur - 1;
    moveCursor(posX, posY);
    printf("║");

    posX = x - 1;
    posY++;
    moveCursor(posX, posY);
    printf("╚");
    for(i = 0; i < longueur - 2; i++) printf("═");
    printf("╝");
    setColor(COLOR_FONT_DEFAULT, COLOR_BG);
}

void displayTitle() {
    char *title = " Projet labyrinthe ";
    unsigned int length = strlen(title);

    setColor(BLUE, COLOR_BG);
    unsigned int x = (WINDOW_WIDTH - MENU_WIDTH - length) / 2;
    unsigned int y = MARGIN;
    moveCursor(x, y);
    printf("%s", title);
    setColor(COLOR_FONT_DEFAULT, COLOR_BG);

    displayBorders(length + 2, x, y);
}

void displayMenuOption(unsigned int x, unsigned int y, char* text) {
    setColor(BLUE, COLOR_BG);
    moveCursor(x, y);
    printf("%s", text);
    setColor(COLOR_FONT_DEFAULT, COLOR_BG);
}

void displayMenu() {
    unsigned int upperLeftCornerX = WINDOW_WIDTH - MENU_WIDTH + MARGIN;
    unsigned int upperLeftCornerY = (WINDOW_HEIGHT - MENU_HEIGHT) / 2;

    displayMenuOption(upperLeftCornerX, upperLeftCornerY, "Générer");
    displayMenuOption(upperLeftCornerX, upperLeftCornerY + MARGIN, "Résoudre");
    displayMenuOption(upperLeftCornerX, upperLeftCornerY + MARGIN * 2, "Sauver");
    displayMenuOption(upperLeftCornerX, upperLeftCornerY + MARGIN * 3, "Charger");
    displayMenuOption(upperLeftCornerX, upperLeftCornerY + MARGIN * 4, "Quitter");
}

void displayMenuSelectionCursor(unsigned int selectedOption) {
	unsigned int upperLeftCornerX = WINDOW_WIDTH - MENU_WIDTH + MARGIN;
    unsigned int upperLeftCornerY = (WINDOW_HEIGHT - MENU_HEIGHT) / 2;

    setColor(RED, COLOR_BG);
    for (unsigned int i = 0; i < 5; i++) {
        moveCursor(upperLeftCornerX - 3, upperLeftCornerY + MARGIN * i);
        printf("  ");
    }

    moveCursor(upperLeftCornerX - 3, upperLeftCornerY + MARGIN * (selectedOption - 1));
    printf(">>");
    setColor(COLOR_FONT_DEFAULT, COLOR_BG);
}

// =====================================================================================================================

void displayMaze() {
    if (isMazeCreated == true) {
        int upperLeftCornerX = (WINDOW_WIDTH - MENU_WIDTH - 2 * m) / 2;
        int upperLeftCornerY = (WINDOW_HEIGHT - n) / 2;

        setColor(GREEN, COLOR_BG);
        for (unsigned int i = 0; i < m; i++) {
            for (unsigned int j = 0; j < n; j++) {
                moveCursor(upperLeftCornerX + 2 * i, upperLeftCornerY + j);
                if (maze[i][j] == 1) printf("▓▓");
                else printf("  ");
            }
        }
        setColor(COLOR_FONT_DEFAULT, COLOR_BG);
    }
}

void displaySolution() {
    if (isSolutionCreated == true) {
        int upperLeftCornerX = (WINDOW_WIDTH - MENU_WIDTH - m) / 2;
        int upperLeftCornerY = (WINDOW_HEIGHT - n) / 2;

        setColor(RED, COLOR_BG);
        for (unsigned int i = 0; i < m; i++) {
            for (unsigned int j = 0; j < n; j++) {
                moveCursor(upperLeftCornerX + 2 * i, upperLeftCornerY + j);
                if (solution[i][j] == 1) printf("██");
            }
        }
        setColor(COLOR_FONT_DEFAULT, COLOR_BG);
    }
}

// =====================================================================================================================

void saveMaze(const char *filePath) {
    if (isMazeCreated == true) {
        FILE *file = fopen(filePath, "w+");
        
        if (file != NULL) {
            char c;

            fputc(m, file);
            fputc('\n', file);
            fputc(n, file);
            fputc('\n', file);

            for (unsigned int i = 0; i < n; i++) {
                for (unsigned int j = 0; j < m; j++) {
                    if (maze[j][i] == 1) c = '1';
                    else c = '0';

                    fputc(c, file);
                }
                fputc('\n', file);
            }

            fclose(file);
        } else {
            // File not found
        }
    }
}

void loadMaze(const char *filePath) {
    FILE *file = fopen(filePath, "r");

    if (file != NULL) {
        if (isMazeCreated == true) destroyMaze(m, n);
        if (isSolutionCreated == true) destroySolution(m, n);

        char c;

        while ((c = fgetc(file)) != '\n') {
            m = (unsigned int) c;
        }

        while ((c = fgetc(file)) != '\n') {
            n = (unsigned int) c;
        }

        createMaze(m, n);

        for (unsigned int i = 0; i < n; i++) {
            for (unsigned int j = 0; j < m + 1; j++) {
                c = fgetc(file);
                if (c == '1') maze[j][i] = 1;
                if (c == '0') maze[j][i] = 0;
            }
        }

        fclose(file);
    } else {
        // File not found
    }
}

// =====================================================================================================================

void handleEvents(unsigned int selectedOption) {
    switch(selectedOption) {
        case 1:
            generateMaze();
            break;
        case 2:
            solveMaze();
            break;
        case 3:
            saveMaze(FILE_PATH);
            break;
        case 4:
            loadMaze(FILE_PATH);
            break;
    }
}

void loop() {
    // Initialisation
    char key = 0;
    unsigned int selectedOption = 1;
    unsigned int optionNb = 5;

    do
    {
        setColor(COLOR_FONT_DEFAULT, COLOR_BG);

        // Nettoyage de l'écran
        clearTerminal();

        // Affichage du titre
        displayTitle();

        // Affichage des propositions
        displayMenu();

        // Affichage des éléments sélectionnés
        displayMaze();
        displaySolution();

        do // Boucle de détection des évènements
        {
            // Affichage du curseur
            displayMenuSelectionCursor(selectedOption);

            // Curseur en haut à gauche de l'écran
            moveCursor(1, 1);

			// 60 détections par seconde pour limiter l'utilisation du processeur
            usleep(16667);

            // Détection de la touche pressée
            key = getch();

            if (key == KEY_UP) {
				if (selectedOption == 1) selectedOption = optionNb;
                else selectedOption -= 1;
            }
            if (key == KEY_DOWN) {
            	if (selectedOption == optionNb) selectedOption = 1;
                else selectedOption += 1;
            }
        } while(key != KEY_ENTER); // Touche entrée

        // Lancement de la fonction adéquate
        handleEvents(selectedOption);
    } while(selectedOption != optionNb); // Quitter
}

void run() {
    resizeTerminal(WINDOW_HEIGHT, WINDOW_WIDTH);
    initRand();

    loop();

    if (isMazeCreated == true) destroyMaze(m, n);
    if (isSolutionCreated == true) destroySolution(m, n);
    setColor(WHITE, BLACK);
    clearTerminal();
}

/*
 * =====================================================================================================================
 * Main
 * =====================================================================================================================
 */

int main() {
    run();

    return 0;
}