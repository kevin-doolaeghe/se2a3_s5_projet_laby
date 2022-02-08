# Programmation structurée - Projet

### Génération aléatoire de labyrinthes :

**Kevin** - Interface Homme-Machine, sauvegarde et chargement\
**God-Belange** - Génération et résolution\
**Date de création** - 24/01/2020

### Prérequis :
Utiliser la commande `cd` afin de se placer dans le dossier du fichier source

## Compiler le programme :

* Méthode n°1 :
```
clang -W -Wall laby.c -o laby -lm
```

* Méthode n°2 :
```
gcc -W -Wall laby.c -o laby -lm
```

## Démarrer le programme :

```
./laby
```
Si les droits pour exécuter le programme sont insuffisants, utiliser la commande `chmod`.

### Compilation et démarrage du programme

Le fichier `run` peut être exécuté :
```
./run
```
Il permet de compiler puis de démarrer le programme.

## Utilisation :

Si le terminal, n'est pas en plein écran, la fenêtre sera redimensionnée.\
Pour naviguer dans le programme, il faut utiliser les touches directionnelles haut et bas ainsi que la touche entrée pour valider.

## Options :
* **Générer** : Génère un labyrinthe de taille aléatoire.
* **Résoudre** : Résoud le labyrinthe chargé en mémoire.
* **Sauver** : Sauvegarde le labyrinthe courant dans un fichier dont vous choisissez le nom. La sauvegarde d'un labyrinthe écrase les données du fichier que vous avez choisi.
* **Charger** : Charge le labyrinthe contenu dans le fichier que vous avez choisi si celui-ci existe.
* **Quitter** : Quitte le programme.

### Exemples fournis :
Les fichiers **laby1**, **laby2** et **laby3** sont des exemples de labyrinthes.\
Pour les utiliser, il faut charger le fichier en entrant le nom dans le programme avec l'option "Charger".

### Exemple de fichier de sauvegarde :
```
3

111111111111111111111111111111111111111111111111111
000010000010000000100000100000000000000000000010001
101011111010111011101110111111101011111111111110101
100000001010100010000010100000001000000000100000101
101111101010101110111110101111111111111010101111101
100000100010101000100010001000000000001010100010101
101110111110101011101011111111101111111010111010101
100010100010100000001000001000000000100010100010001
101010101010111111111111111011111110101111101111111
101010001000001000001000000010000010100000101000001
101011111111111011101011111110111011111110101111101
101010000010000000101010001000101000100010100000101
101011101110111111101010101011101110101110111110101
101000100010100000100010100000000010100000001000101
101110111010101110111110111111111010111111111011101
101010000010101010000000100000001010000010001010001
101011111110101011111111111011101111101110101010111
100010000000101000000000000010100000100000100010101
101110111111101110111111111110111110111111111110101
100000100000001000100000100010000010000000100000101
101111101111111011101011101010111010111110101111101
100000001000000010001000101010100010000010000010001
101111111011111110111110111010111110111011111110111
100000000000000000000010000010000000001000000000000
111111111111111111111111111111111111111111111111111
```
