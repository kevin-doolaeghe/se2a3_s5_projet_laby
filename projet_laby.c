#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

/*
 * =====================================================================================================================
 * Initialisation
 * =====================================================================================================================
 */

#define KEY_ESCAPE  0x001b
#define KEY_ENTER   0x000a
#define KEY_UP      0x0105
#define KEY_DOWN    0x0106
#define KEY_LEFT    0x0107
#define KEY_RIGHT   0x0108

#define BLACK       30
#define RED         31
#define GREEN       32
#define YELLOW      33
#define BLUE        34
#define PURPLE      35
#define CYAN        36
#define WHITE       37

//#define color(paramChar, paramBackground) printf("\033[1;%dm""\033[1;%dm", paramChar, paramBackground+10)
//#define gotoxy(x,y) printf("\033[%d;%df", y, x)

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

void createMaze(int m, int n) {
    maze = malloc(m * sizeof(int));
    for (int i = 0; i < n; i++) {
        maze[i] = malloc(n * sizeof(int));
    }
}

void destroyMaze(bool** maze) {
    free(maze);
}

// =====================================================================================================================

void generateMaze() {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            maze[i][j] = rand() % 2;
        }
    }

    // Borders
    for (int i = 0; i < m; i++) {
        maze[i][0] = 1;
        maze[i][m - 1] = 1;
    }

    for (int i = 0; i < n; i++) {
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
    system("resize -s " + lines + " " + columns);
}

void moveCursor(unsigned int x, unsigned int y) {
    printf("\033[%d;%df", y, x);
}

void setColor(unsigned int charColor, unsigned int backgroundColor) {
    printf("\033[1;%dm""\033[1;%dm", charColor, backgroundColor + 10);
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
    printf("%s", text);

    displayBorders(length + 2, x - 1, y - 1);
}

void displayMenuOption(unsigned int x, unsigned int y, char* text) {
    moveCursor(x, y);
    printf("%s", text);

    unsigned int length = strlen(text);
    displayBorders(length + 2, x - 1, y - 1);
}

void displayMenus() {
    unsigned int upperLeftCornerX = WINDOW_WIDTH - MENUS_WIDTH + MARGIN;
    unsigned int upperLeftCornerY = WINDOW_HEIGHT + MARGIN;
    unsigned int spaceBetweenOptions = 5;

    displayMenuOption(upperLeftCornerX, upperLeftCornerY, "Générer");
    displayMenuOption(upperLeftCornerX, upperLeftCornerY + spaceBetweenOptions, "Résoudre");
    displayMenuOption(upperLeftCornerX, upperLeftCornerY + spaceBetweenOptions * 2, "Sauver");
    displayMenuOption(upperLeftCornerX, upperLeftCornerY + spaceBetweenOptions * 3, "Charger");
    displayMenuOption(upperLeftCornerX, upperLeftCornerY + spaceBetweenOptions * 4, "Quitter");
}

void displaySelectedMenuOption(unsigned int selectedOption) {
    switch(selectedOption)
    {
        case 1:
            generateMaze();
            displayMaze();
            break;
        case 2:
            solveMaze();
            displaySolution();
            break;
        case 3:
            saveMaze(FILE_PATH);
            break;
        case 4:
            loadMaze(FILE_PATH);
            break;
    }
}

// =====================================================================================================================

void saveMaze(char* filePath) {
    char* cmd = FILE_PATH + " >> ";
    for (unsigned int i = 0; i < m; i++) {
        for (unsigned int j = 0; j < n; j++) {
            if (maze[i][j] == 1) cmd += "1";
            else cmd += "0";
        }
        cmd += "\n";
    }
    printf("%s", cmd);
}

void loadMaze(char* filePath) {

}

// =====================================================================================================================

void loop() {
    //Initialisation
    char key1, key2;
    unsigned int selectedOption = 1;
    unsigned int i;

    do
    {
        setColor(BLACK, COLOR_BG);
        clear();

        //Affichage du titre
        displayTitle();

        //Affichage des propositions
        displayMenus();

        do //Boucle de décalage du curseur selon le choix
        {
            //Effacage du curseur
            for(i = 6; i < nbProp * 2 + 8; i++)
            {
                moveCursor(7, i);
                printf("  ");
            }

            //Affichage du curseur
            moveCursor(7, selection * 2 + 4);
            color(12, 15);
            printf(">>");
            color(0, 15);

            //Curseur en haut à gauche de l'écran
            moveCursor(1, 1);

            //Détection de la touche pressée
            key2 = 0; //Initialisation
            key1 = getch();
            if(key1 == -32) key2 = getch(); //Touches directionnelles = 2 touches pressées
            if(key2 == 72) //Flèche du haut
            {
                if(selection == 1) selection = nbProp + 1;
                else selection -= 1;
            }
            if(key2 == 80) //Flèche du bas
            {
                if(selectedOption == 5) selectedOption = 1;
                else selectedOption += 1;
            }
        } while(key1 != KEY_ENTER); //Touche entrée

        //Lancement de la fonction adéquate
        handleEvents(selectedOption);
    } while(selectedOption != 5); //Quitter
}

void handleEvents(unsigned int selectedOption) {
    switch(selectedOption) {
        case 1:
            generateMaze();
            displayMaze();
            break;
        case 2:
            solveMaze();
            displaySolution();
            break;
        case 3:
            saveMaze(FILE_PATH);
            break;
        case 4:
            loadMaze(FILE_PATH);
            break;
    }
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