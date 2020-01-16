#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

const int m=25;
const int n=69;
int entree;
int sortie;

void createpath(bool maze[m][n],int x, int y) {

   int x1, y1;
   int x2, y2;
   int dx, dy;
   int dir, count;

   dir = rand() % 4;
   count = 0;
   while(count < 4) {
      dx = 0; dy = 0;
      switch(dir) {
      case 0:  dx = 1;  break;
      case 1:  dy = 1;  break;
      case 2:  dx = -1; break;
      default: dy = -1; break;
      }
      x1 = x + dx;
      y1 = y + dy;
      x2 = x1 + dx;
      y2 = y1 + dy;
      if( x2 > 0 && x2 < n-1 && y2 > 0 && y2 < m-1 && maze[y1 ][x1] ==1 && maze[y2][x2] == 1)
      {  
         maze[y1 ][x1] =0;
         maze[y2][x2]  = 0;
         x = x2; y = y2;
         dir = rand() % 4;
         count = 0;
       
      } else {
         dir = (dir + 1) % 4;
         count += 1;
      }
    
   }

}
void generateMaze(bool maze[m][n])
{
    int x, y;
  //init maze
   for (int i = 0; i < m; i++) 
   {
       for (int j = 0; j < n; j++)
        { 
           maze[i][j] =1;  
        }
   }
  
   // random number generator. 
   srand(time(0));

   //Create path and ensure that we pass in all coins 
   for(y = 1; y < m; y += 2) 
   {
      for(x = 1; x < n; x += 2)
      {
         createpath(maze,x, y); 
      }
   }
   // entrer and exit of the laby
     maze[0 ][ 1] = 0;
    maze[(m - 1)][ (n - 2)] = 0;

 }
void solvemaze(bool maze[m][n],bool solution[m][n])
{
   int dir, count;
   int x, y;
   int dx, dy;
   int forward;
   printf("yes");
   //init maze
   for (int i = 0; i < m; i++) 
   {
       for (int j = 0; j < n; j++)
        { 
           solution[i][j] =0;  
        }
   }
  

   
   /* Remove the entry and exit. */
   maze[0 ][ 1] = 1;
   maze[(m - 1)][ (n - 2)] = 1;

   forward = 1;
   dir = 0;
   count = 0;
   x = 1;
   y = 1;
   while(x != n - 2 || y != m - 2) {
      dx = 0; dy = 0;
      switch(dir) {
      case 0:  dx = 1;  break;
      case 1:  dy = 1;  break;
      case 2:  dx = -1; break;
      default: dy = -1; break;
      }
      if(   (forward  && maze[(y + dy)] [ (x + dx)] == 0)
         || (!forward && solution[(y + dy)][ (x + dx)] == 1)) {
         solution[y ] [ x] = forward ? 1 : 0;
         x += dx;
         y += dy;
         forward = 1;
         count = 0;
         dir = 0;
      } else {
         dir = (dir + 1) % 4;
         count += 1;
         if(count > 3) {
            forward = 0;
            count = 0;
         }
      }
   }

   /* Replace the entry and exit. */
   maze[0 ][ 1] = 0;
   maze[(m - 1)][ (n - 2)] = 0;
   solution[0 ] [1] = 2;
   solution[(m - 1) ]  [(n - 2)] = 2;
   }
// 
   /* Replace the entry and exit. */
    

// Display maze
void showmaze(bool maze[m][n])
{
    int x, y;
   for(y = 0; y < m; y++) {
      for(x = 0; x < n; x++) {
         switch(maze[y ] [ x]) {
         case 1:  printf("[]");  break;
         default: printf("  ");  break;
         }
      }
      printf("\n");
   }
}

int main() {

    bool  maze[m][n];
    bool solution[m][n];
    generateMaze(maze);  
    solvemaze(maze,solution);
    showmaze(maze);

    return 0;
}