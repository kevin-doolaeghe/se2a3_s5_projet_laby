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

void destroyMaze(unsigned int m) {
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

void destroySolution(unsigned int m) {
    for (unsigned int i = 0; i < m; i++) free(solution[i]);
    free(solution);

    isSolutionCreated = false;
}

// =====================================================================================================================

/*
 * God-Belange
 */

void createPath(bool **maze, unsigned int m, unsigned int n, unsigned int x, unsigned int y) {
    unsigned int direction = rand() % 4;
    unsigned int count = 0;
    bool isMoving = false;

    while (count < 4) {
        switch (direction) {
        case 0: // Gauche
            // Si les cellules n'ont pas été visitées et qu'il n'y a pas de murs
            if (y - 1 != 0 && y - 2 != 0 && maze[x][y - 1] ==1 && maze[x][y - 2] == 1) {
                maze[x][y - 1] = 0;
                maze[x][y - 2] = 0;
                y = y - 2;
                isMoving = true;
            }
            break;
        case 1: // Bas
            if (x + 1 != m && x + 2 != m && maze[x + 2][y] == 1 && maze[x + 1][y] == 1) {
                maze[x + 1][y] =0;
                maze[x + 2][y] = 0;
                x = x + 2;
                isMoving = true;
            }
            break;
        case 2: //Droite
            if (y + 1 != n && y + 2 != n && maze[x][y + 1] == 1 && maze[x][y + 2] == 1) {
                maze[x][y + 1] = 0;
                maze[x][y + 2] = 0;
                y = y + 2;
                isMoving = true;
            }
            break;
        case 3: // Haut
            if (x - 1 != 0 && x - 2 != 0 && maze[x - 1][y] == 1 && maze[x - 2][y] == 1) {
                maze[x - 1][y] = 0;
                maze[x - 2][y] = 0;
                x = x - 2;
                isMoving = true;
            }
            break;
        }

        if (isMoving == true) { // Si on s'est déplacé
            direction = rand() % 4;
            count = 0;
            isMoving = false;
        } else { // Tente une autre direction
            direction = (direction + 1) % 4;
            count += 1;
        }
    }
}

void generateMaze() {
    if (isMazeCreated == true) destroyMaze(m);
    if (isSolutionCreated == true) destroySolution(m);

    // Génère m et n aléatoirement
    setDimensions();
    createMaze(m, n);

    // Initialisation
    for (unsigned int i = 0; i < m; i++) {
        for (unsigned int j = 0; j < n; j++) {
            maze[i][j] = 1;
        }
    }

    // Remplissage
    for (unsigned int x = 1; x < m; x += 2) {
        for (unsigned int y = 1; y < n; y += 2) {
            createPath(maze, m, n, x, y); 
        }
    }

    // Entrée et sortie
    maze[0][1] = 0;
    maze[m - 1][n - 2] = 0;
}

bool fillSolution(bool **maze, bool **solution, unsigned int m, unsigned int n, unsigned int x, unsigned int y) {
    // Si arrivé à la sortie
    if ((x == m - 1) && (y == n - 2)) {
        solution[x][y] = 1;

        return 1;
    }
    
    // Si passage
    if (x < m && y < n && solution[x][y] == 0 && maze[x][y] == 0) { 
        solution[x][y] = 1;

        // Bas
        if (fillSolution(maze, solution, m, n, x + 1, y)) return 1;
        
        // Droite
        if (fillSolution(maze ,solution, m, n, x , y + 1)) return 1;
        
        // Haut
        if (fillSolution(maze, solution, m, n, x - 1, y)) return 1;
        
        // Gauche
        if (fillSolution(maze, solution, m, n, x, y - 1)) return 1;
        
        // Bloqué : demi-tour
        solution[x][y] = 0;

        return 0;
    }

    return 0;
}

void solveMaze() {
    if (isSolutionCreated == true) destroySolution(m);

    if (isMazeCreated == true) {
        createSolution(m, n);

        // Initialisation
        for (unsigned int i = 0; i < m; i++) {
            for (unsigned int j = 0; j < n; j++) {
                solution[i][j] = 0;
            }
        }

        // Recherche de la sortie
        fillSolution(maze, solution, m, n, 0, 1);
    }
}

/*
 * =====================================================================================================================
 * Affichage
 * =====================================================================================================================
 */

void clearTerminal() {
    system("clear"); // Efface l'écran
}

void resizeTerminal(unsigned int lines, unsigned int columns) {
    // Redimensionne le terminal si il n'est pas en plein écran

    // Convertie en caractère le nombre de lignes et de colonnes
    char lines_str[10] = "";
    sprintf(lines_str, "%d", lines);
    char columns_str[10] = "";
    sprintf(columns_str, "%d", columns);

    // Fabrication de la commande
    char* cmd = (char *) malloc(40 * sizeof(char));
    cmd = strcat(cmd, "resize -s ");
    cmd = strcat(cmd, lines_str);
    cmd = strcat(cmd, " ");
    cmd = strcat(cmd, columns_str);

    // Envoi de la commande
	system(cmd);

    free(cmd);
}

void moveCursor(unsigned int x, unsigned int y) {
    // Déplace le curseur à l'écran
    printf("\033[%d;%df", y, x);
}

void setColor(unsigned int charColor, unsigned int backgroundColor) {
    // Change la couleur du caractère et du fond de caractère
    printf("\033[1;%dm""\033[1;%dm\n", charColor, backgroundColor + 10);
}

int getch() {
    system("stty raw -echo"); // Pas d'affichage des caractères
    int key = getchar();
    system("stty cooked echo");

    return key;
}

// =====================================================================================================================

/*
 * Kevin
 */

void displayBorders(unsigned int longueur, unsigned int x, unsigned int y) {
    // Affichage d'un encadré
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
    // Calcul des coordonnées de l'emplacement du menu
    unsigned int upperLeftCornerX = WINDOW_WIDTH - MENU_WIDTH + MARGIN;
    unsigned int upperLeftCornerY = (WINDOW_HEIGHT - MENU_HEIGHT) / 2;

    displayMenuOption(upperLeftCornerX, upperLeftCornerY, "Générer");
    displayMenuOption(upperLeftCornerX, upperLeftCornerY + MARGIN, "Résoudre");
    displayMenuOption(upperLeftCornerX, upperLeftCornerY + MARGIN * 2, "Sauver");
    displayMenuOption(upperLeftCornerX, upperLeftCornerY + MARGIN * 3, "Charger");
    displayMenuOption(upperLeftCornerX, upperLeftCornerY + MARGIN * 4, "Quitter");
}

void displayMenuSelectionCursor(unsigned int selectedOption) {
    // Affichage du curseur de sélection du menu
	unsigned int upperLeftCornerX = WINDOW_WIDTH - MENU_WIDTH + MARGIN;
    unsigned int upperLeftCornerY = (WINDOW_HEIGHT - MENU_HEIGHT) / 2;

    setColor(RED, COLOR_BG);
    // Suppression de l'ancien curseur
    for (unsigned int i = 0; i < 5; i++) {
        moveCursor(upperLeftCornerX - 3, upperLeftCornerY + MARGIN * i);
        printf("  ");
    }

    // Affichage du curseur
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
        // On parcourt le tableau
        for (unsigned int i = 0; i < m; i++) {
            for (unsigned int j = 0; j < n; j++) {
                // On se place aux coordonnées d'une case pour l'afficher
                moveCursor(upperLeftCornerX + 2 * i, upperLeftCornerY + j);
                if (maze[i][j] == 1) printf("▓▓"); // Si mur
                else printf("  "); // Si chemin
            }
        }
        setColor(COLOR_FONT_DEFAULT, COLOR_BG);
    }
}

void displaySolution() {
    if (isSolutionCreated == true) {
        int upperLeftCornerX = (WINDOW_WIDTH - MENU_WIDTH - 2 * m) / 2;
        int upperLeftCornerY = (WINDOW_HEIGHT - n) / 2;

        setColor(RED, COLOR_BG);
        // On parcourt le tableau de la solution
        for (unsigned int i = 0; i < m; i++) {
            for (unsigned int j = 0; j < n; j++) {
                // Affichage de la solution
                moveCursor(upperLeftCornerX + 2 * i, upperLeftCornerY + j);
                if (solution[i][j] == 1) printf("▒▒");
            }
        }
        setColor(COLOR_FONT_DEFAULT, COLOR_BG);
    }
}

// =====================================================================================================================

void saveMaze() {
    if (isMazeCreated == true) {
        unsigned int upperLeftCornerX = WINDOW_WIDTH - MENU_WIDTH + MARGIN;
        unsigned int upperLeftCornerY = (WINDOW_HEIGHT - MENU_HEIGHT) / 2;
        char path[20] = "";

        // Saisie du nom du fichier
        setColor(RED, COLOR_BG);
        moveCursor(upperLeftCornerX, upperLeftCornerY + 2 * MARGIN + 1);
        printf("Nom du fichier :");
        moveCursor(upperLeftCornerX, upperLeftCornerY + 2 * MARGIN + 2);
        if (fgets(path, 20, stdin) != NULL) {
            char *posEnd = strchr(path, '\n');
            if (posEnd != NULL) *posEnd = '\0';
        }
        setColor(COLOR_FONT_DEFAULT, COLOR_BG);

        // Ouverture du fichier en écriture avec suppression du contenu précédent
        // Création du fichier si il n'existe pas
        FILE *file = fopen(path, "w+");
        
        if (file != NULL) { // Si le fichier est ouvert
            char c;

            // Ajout de m au fichier
            fputc(m, file);
            fputc('\n', file);
            // Ajout de n au fichier
            fputc(n, file);
            fputc('\n', file);

            // Ecriture du labyrinthe courant dans le fichier
            for (unsigned int i = 0; i < n; i++) {
                for (unsigned int j = 0; j < m; j++) {
                    if (maze[j][i] == 1) c = '1';
                    else c = '0';

                    fputc(c, file);
                }
                fputc('\n', file);
            }

            fclose(file); // Fermeture du fichier
        }
    }
}

void loadMaze() {
	unsigned int upperLeftCornerX = WINDOW_WIDTH - MENU_WIDTH + MARGIN;
    unsigned int upperLeftCornerY = (WINDOW_HEIGHT - MENU_HEIGHT) / 2;
    char path[20] = {0};

    // Saisie du nom du fichier
    setColor(RED, COLOR_BG);
    moveCursor(upperLeftCornerX, upperLeftCornerY + 3 * MARGIN + 1);
    printf("Nom du fichier :");
    moveCursor(upperLeftCornerX, upperLeftCornerY + 3 * MARGIN + 2);
    if (fgets(path, 20, stdin) != NULL) {
        char *posEnd = strchr(path, '\n');
        if (posEnd != NULL) *posEnd = '\0';
    }
    setColor(COLOR_FONT_DEFAULT, COLOR_BG);

    // Ouverture du fichier en mode lecture
    FILE *file = fopen(path, "r");

    if (file != NULL) { // Si fichier ouvert
        if (isMazeCreated == true) destroyMaze(m);
        if (isSolutionCreated == true) destroySolution(m);

        char c;

        // Lecture de m
        while ((c = fgetc(file)) != '\n') {
            m = (unsigned int) c;
        }
        // Lecture de n
        while ((c = fgetc(file)) != '\n') {
            n = (unsigned int) c;
        }

        createMaze(m, n);

        // Lecture du labyrinthe
        for (unsigned int i = 0; i < n; i++) {
            for (unsigned int j = 0; j < m + 1; j++) {
                c = fgetc(file);
                if (c == '1') maze[j][i] = 1;
                if (c == '0') maze[j][i] = 0;
            }
        }

        fclose(file);
    }
}

// =====================================================================================================================

void handleEvents(unsigned int selectedOption) {
    switch(selectedOption) { // Effectue une action selon la sélection
        case 1:
            generateMaze();
            break;
        case 2:
            solveMaze();
            break;
        case 3:
            saveMaze();
            break;
        case 4:
            loadMaze();
            break;
    }
}

void loop() {
    // Initialisation
    char key = 0;
    unsigned int selectedOption = 1;
    unsigned int optionNb = 5;

    do {
        /*
         * AFFICHAGE
         */
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

        /*
         * Boucle de détection des touches
         */
        do {
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

        /*
         * Gestion des événements
         */
        handleEvents(selectedOption);
    } while(selectedOption != optionNb); // Quitter
}

void run() {
    initRand();
    resizeTerminal(WINDOW_HEIGHT, WINDOW_WIDTH);

    loop(); // Boucle principale du programme

    if (isMazeCreated == true) destroyMaze(m);
    if (isSolutionCreated == true) destroySolution(m);
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