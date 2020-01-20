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

const unsigned int WINDOW_WIDTH = 180;
const unsigned int WINDOW_HEIGHT = 40;
const unsigned int MENU_WIDTH = 20;
const unsigned int MENU_HEIGHT = 13;
const unsigned int MARGIN = 3;

const char* FILE_PATH = "~/maze.save";

unsigned int m; // Width
unsigned int n; // Height
bool **maze = NULL;
bool **solution = NULL;

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
    m = rand() % 30 + 50; // rand() % 80 + 1;
    n = rand() % 12 + 14; // rand() % 25 + 1;

    if (m % 2 == 0) m++;
    if (n % 2 == 0) n++;
}

void createMaze(unsigned int m, unsigned int n) {
    maze = malloc(m * sizeof(*maze)); // Columns >> X
    for (unsigned int i = 0; i < m; i++) {
        maze[i] = malloc(n * sizeof(bool)); // Lines >> Y
    }

    isMazeCreated = true;
}

void destroyMaze(unsigned int m, unsigned int n) {
    for (unsigned int i = 0; i < m; i++) free(maze[i]);

    isMazeCreated = false;
}

void createSolution(unsigned int m, unsigned int n) {
    solution = malloc(m * sizeof(*solution)); // Columns >> X
    for (unsigned int i = 0; i < m; i++) {
        solution[i] = malloc(n * sizeof(bool)); // Lines >> Y
    }

    isSolutionCreated = true;
}

void destroySolution(unsigned int m, unsigned int n) {
    for (unsigned int i = 0; i < m; i++) free(solution[i]);

    isSolutionCreated = false;
}

// =====================================================================================================================

void fillMaze() {
    for (unsigned int i = 0; i < m; i++) {
        for (unsigned int j = 0; j < n; j++) {
            maze[i][j] = rand() % 2;
        }
    }

    // Borders
    for (unsigned int i = 0; i < m; i++) {
        maze[i][0] = 1;
        maze[i][n - 1] = 1;
    }

    for (unsigned int i = 0; i < n; i++) {
        maze[0][i] = 1;
        maze[m - 1][i] = 1;
    }

    maze[0][1] = 0;
    maze[m - 1][n - 2] = 0;
}

void generateMaze() {
    if (isMazeCreated == true) destroyMaze(m, n);
    if (isSolutionCreated == true) destroySolution(m, n);

    setDimensions();
    createMaze(m, n);

    fillMaze();
}

void solveMaze() {
    if (isMazeCreated == true) {
        if (isSolutionCreated == true) destroySolution(m, n);

        createSolution(m, n);
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
    char* lines_str = (char *) malloc(40 * sizeof(char));
    sprintf((char*) lines_str,"%d",lines);
    char* columns_str = (char *) malloc(40 * sizeof(char));
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
	system("stty raw -echo"); // system("stty ignbrk -echo"); // Paramétrage pour la saisie
	int key = getchar();
	system("stty cooked echo"); // system("stty -ignbrk echo");

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
    printf("+"); // printf("%c", 201);
    for(i = 0; i < longueur - 2; i++) printf("-"); // printf("%c", 205);
    printf("+"); // printf("%c", 187);

    posY++;
    moveCursor(posX, posY);
    printf("|"); // printf("%c", 186);

    posX += longueur - 1;
    moveCursor(posX, posY);
    printf("|"); // printf("%c", 186);

    posX = x - 1;
    posY++;
    moveCursor(posX, posY);
    printf("+"); // printf("%c", 200);
    for(i = 0; i < longueur - 2; i++) printf("-"); // printf("%c", 205);
    printf("+"); // printf("%c", 188);
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
                if (maze[i][j] == 1) printf("[]");
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
                if (solution[i][j] == 1) printf("<>");
            }
        }
        setColor(COLOR_FONT_DEFAULT, COLOR_BG);
    }
}

// =====================================================================================================================

void saveMaze(const char *filePath) {
    if (isMazeCreated == true) {
        FILE* file = fopen(filePath, "w+");
        
        if (file != NULL) {
            char c;

            /*
            fputc(m, file);
            fputc("\n", file);
            fputc(n, file);
            fputc("\n", file);
            */

            for (unsigned int i = 0; i < n; i++) {
                for (unsigned int j = 0; j < m; j++) {
                    if (maze[j][i] == 1) c = '1';
                    else c = '0';

                    fputc(c, file);
                }
                fputc('n', file);
            }

        } else {
            // File not found
        }

        fclose(file);
    }
}

void loadMaze(const char *filePath) {
    if (isMazeCreated == true) destroyMaze(m, n);
    if (isSolutionCreated == true) destroySolution(m, n);

    FILE* file = fopen(filePath, "r");

    if (file != NULL) {
        char c;
        unsigned int x = 0;
        unsigned int y = 0;

        while ((c = fgetc(file)) != '\n') {
            m = (unsigned int) c;
        }

        while ((c = fgetc(file)) != '\n') {
            n = (unsigned int) c;
        }

        destroyMaze(m, n);
        createMaze(m, n);

        while ((c = fgetc(file)) != EOF) {
            if (c == '\n') {
                y++;
                x = 0;
            }

            if (c == '1') maze[x][y] = 1;
            else maze[x][y] = 0;

            x++;
        }
    } else {
        // File not found
    }

    fclose(file);
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
            if (key == 27) key = getch();
            if (key == 91) key = getch();

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