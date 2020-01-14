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

#define KEY_ESCAPE  32
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

const unsigned int WINDOW_WIDTH = 120;
const unsigned int WINDOW_HEIGHT = 40;
const unsigned int MENUS_WIDTH = 30;
const unsigned int MARGIN = 3;
const unsigned int COLOR_BG = WHITE;
const char* FILE_PATH = "~/.maze.save";

unsigned int m; // Width
unsigned int n; // Height
bool** maze;
bool** solution;
bool isMazeDisplayed = false;
bool isSolutionDisplayed = false;

/*
 * =====================================================================================================================
 * Génération
 * =====================================================================================================================
 */

void initRand() {
    srand(time(NULL));
}

void setDimensions() {
    m = rand() % 80 + 1;
    n = rand() % 25 + 1;
}

void createMaze(unsigned int m, unsigned int n) {
    maze = malloc(m * sizeof(int));
    for (unsigned int i = 0; i < n; i++) {
        maze[i] = malloc(n * sizeof(int));
    }
}

void destroyMaze() {
    free(maze);
}

// =====================================================================================================================

void generateMaze() {
    for (unsigned int i = 0; i < m; i++) {
        for (unsigned int j = 0; j < n; j++) {
            maze[i][j] = rand() % 2;
        }
    }

    // Borders
    for (unsigned int i = 0; i < m; i++) {
        maze[i][0] = 1;
        maze[i][m - 1] = 1;
    }

    for (unsigned int i = 0; i < n; i++) {
        maze[0][i] = 1;
        maze[n - 113][0] = 1;
    }

    maze[0][1] = 0;
    maze[0][n - 1] = 0;
}

void solveMaze() {

}

/*
 * =====================================================================================================================
 * Affichage
 * =====================================================================================================================
 */

void clearTerminal() {
    system("clear");
}

void resizeTerminal(unsigned int columns, unsigned int lines) {
	printf("resize -s %d %d", lines, columns);
}

void moveCursor(unsigned int x, unsigned int y) {
    printf("\033[%d;%df", y, x);
}

void setColor(unsigned int charColor, unsigned int backgroundColor) {
    printf("\033[1;%dm""\033[1;%dm", charColor, backgroundColor + 10);
}

int getch()
{
	system("stty raw -echo"); // Paramétrage pour la saisie
	int key = getchar();
	system("stty cooked echo");s

	return key;
}

// =====================================================================================================================

static void displayBorders(unsigned int longueur, unsigned int x, unsigned int y)
{
    setColor(RED, COLOR_BG);
    unsigned int posX;
    unsigned int posY;
    unsigned int i;

    posX = x - 1;
    posY = y - 1;
    moveCursor(posX, posY);
    printf("%c", 201);
    for(i = 0; i < longueur - 2; i++) printf("%c", 205);
    printf("%c", 187);

    posY++;
    moveCursor(posX, posY);
    printf("%c", 186);

    posX += longueur - 1;
    moveCursor(posX, posY);
    printf("%c", 186);

    posX = x - 1;
    posY++;
    moveCursor(posX, posY);
    printf("%c", 200);
    for(i = 0; i < longueur - 2; i++) printf("%c", 205);
    printf("%c", 188);
}

static void removeBorders(unsigned int longueur, unsigned int x, unsigned int y)
{
    unsigned int posX;
    unsigned int posY;
    unsigned int i;

    posX = x - 1;
    posY = y - 1;
    moveCursor(posX, posY);
    for(i = 0; i < longueur; i++) printf(" ");

    posY++;
    moveCursor(posX, posY);
    printf(" ");

    posX += longueur - 1;
    moveCursor(posX, posY);
    printf(" ");

    posX = x - 1;
    posY++;
    moveCursor(posX, posY);
    for(i = 0; i < longueur; i++) printf(" ");
}

void displayTitle() {
    char* title = "Projet labyrinthe";
    unsigned int length = strlen(title);

    unsigned int x = (WINDOW_WIDTH - MENUS_WIDTH - length) / 2;
    unsigned int y = MARGIN;
    moveCursor(x, y);
    printf("%s", title);

    displayBorders(length + 2, x - 1, y - 1);
}

void displayMenuOption(unsigned int x, unsigned int y, char* text) {
    moveCursor(x, y);
    printf("%s", text);
}

void displayMenu() {
    unsigned int upperLeftCornerX = WINDOW_WIDTH - MENUS_WIDTH + MARGIN;
    unsigned int upperLeftCornerY = WINDOW_HEIGHT + MARGIN;
    unsigned int spaceBetweenOptions = 5;

    displayMenuOption(upperLeftCornerX, upperLeftCornerY, "Générer");
    displayMenuOption(upperLeftCornerX, upperLeftCornerY + spaceBetweenOptions, "Résoudre");
    displayMenuOption(upperLeftCornerX, upperLeftCornerY + spaceBetweenOptions * 2, "Sauver");
    displayMenuOption(upperLeftCornerX, upperLeftCornerY + spaceBetweenOptions * 3, "Charger");
    displayMenuOption(upperLeftCornerX, upperLeftCornerY + spaceBetweenOptions * 4, "Quitter");
}

void displayMenuSelectionCursor(unsigned int selectedOption) {
	unsigned int upperLeftCornerX = WINDOW_WIDTH - MENUS_WIDTH + MARGIN;
    unsigned int upperLeftCornerY = WINDOW_HEIGHT + MARGIN;
    unsigned int spaceBetweenOptions = 5;

    moveCursor(upperLeftCornerX - 3, upperLeftCornerY + spaceBetweenOptions * selectedOption);
    printf(">>");
}

// =====================================================================================================================

void displayMaze() {
    int upperLeftCornerX = (WINDOW_WIDTH - MENUS_WIDTH - m) / 2;
    int upperLeftCornerY = (WINDOW_HEIGHT - n) / 2;

    for (unsigned int i = 0; i < m; i++) {
        for (unsigned int j = 0; j < n; j++) {
            moveCursor(upperLeftCornerX + i, upperLeftCornerY + j);
            if (maze[i][j] == 1) printf("1");
            else printf("0");
        }
    }
}

void displaySolution() {

}

void displayObjectForSelectedMenuOption() {
    if (isMazeDisplayed == true) {
    	displayMaze();
    }

    if (isSolutionDisplayed == true) {
    	displaySolution();
    }
}

// =====================================================================================================================

void saveMaze(const char* filePath) {
    char* cmd = strcat((char*)FILE_PATH, " >> ");
    for (unsigned int i = 0; i < m; i++) {
        for (unsigned int j = 0; j < n; j++) {
            if (maze[i][j] == 1) cmd = strcat(cmd, "1");
            else cmd = strcat(cmd, "0");
        }
        cmd = strcat(cmd, "\n");
    }
    printf("%s", cmd);
}

void loadMaze(const char* filePath) {

}

// =====================================================================================================================

void handleEvents(unsigned int selectedOption) {
    switch(selectedOption) {
        case 1:
            generateMaze();
            isMazeDisplayed = true;
            break;
        case 2:
            solveMaze();
            isSolutionDisplayed = true;
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
    char key;
    unsigned int selectedOption = 1;
    unsigned int optionNb = 5;

    do
    {
        setColor(BLACK, COLOR_BG);
        // Nettoyage de l'écran
        clearTerminal();

        // Affichage du titre
        displayTitle();

        // Affichage des propositions
        displayMenu();

        // Affichage du curseur
        displayMenuSelectionCursor(selectedOption);

        // Affichage des éléments sélectionnés
        displayObjectForSelectedMenuOption();

		//Curseur en haut à gauche de l'écran
        moveCursor(1, 1);

        do // Boucle de détection des évènements
        {
			// 60 détections par seconde pour limiter l'utilisation du processeur
            usleep(16667);

            //Détection de la touche pressée
            key = getch();
            printf("%d", key);
            if (key == KEY_UP) {
				if(selectedOption == optionNb) selectedOption = 1;
                else selectedOption += 1;
            }
            if (key == KEY_DOWN) {
            	if(selectedOption == 1) selectedOption = optionNb + 1;
                else selectedOption -= 1;
            }
        } while(key != KEY_ENTER); //Touche entrée

        //Lancement de la fonction adéquate
        handleEvents(selectedOption);
    } while(selectedOption != optionNb); //Quitter
}

/*
 * =====================================================================================================================
 * Main
 * =====================================================================================================================
 */

int main() {
    initRand();
    setDimensions();
    createMaze(m, n);

    loop();

    destroyMaze();

    return 0;
}