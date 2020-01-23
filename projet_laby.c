#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

const unsigned int m=27;
const unsigned int n=33;

/* Creation of the maze's path */
///////////////////
void createpath(bool maze[m][n],unsigned int x,unsigned int y)
{
   int dir, count;
   dir = rand() % 4;
   count = 0;
   bool indic =0;

   while(count < 4){


      // ensure that we are not on the wall and  we din't visit the cells then move

       switch(dir)
       { 
           //left  
        case 0:
         if(y-1!=0 && y-2 !=0 && maze[x][y-1] ==1 && maze[x][y-2] == 1)
           { 
           maze[x][y-1] =0;
           maze[x][y-2] = 0;
           y=y-2;
           indic=1;
           }

           break;
           //down
         case 1:  
          if(x+1!=m && x+2 !=m && maze[x+2][y] ==1 && maze[x+1][y] == 1)
           {
              maze[x+1][y] =0;
              maze[x+2][y] = 0; 
              x=x+2;
              indic=1;
           }  
               
          break;
          //right
         case 2:  
          if(y+1!=n && y+2!=n && maze[x ][y+1] ==1 && maze[x][y+2] == 1)
          {
              maze[x][y+1] =0;
              maze[x][y+2] = 0;
              y=y+2;
              indic=1;
          }   
         
          break;
          //up
         case 3 :
          if(x-1!=0 && x-2!=0 && maze[x-1][y] ==1 && maze[x-2][y] == 1)
          {
             maze[x-1][y] =0;
             maze[x-2][y] =0; 
             x=x-2;
             indic =1;
          }

         break;
       }
     // did we move if yes choose a random direction else try other remaining direction
    if (indic==1)
      {
         dir = rand() % 4;
         count = 0;
         indic=0;   
       
      }
    else 
      {
         dir = (dir + 1) % 4;
         count += 1;
      }
    }
}

/* Generation of the maze */
////////////////////////////

void generateMaze(bool maze[m][n],bool solution[m][n])
{
 
    unsigned int x, y;
  //initialize the maze and the solution maze
   for (unsigned int i = 0; i < m; i++) 
   {
       for (unsigned int j = 0; j < n; j++)
        { 
           maze[i][j] =1; 
           solution[i][j]=0; 
        }
   }
  
   // initialize the random number generator. 
   srand(time(0));

   //Create path and ensure that we pass in all cells of the maze
   for(x = 1; x< m; x += 2) 
   {
      for(y = 1; y < n; y += 2)
      {
         createpath(maze,x, y); 
      }
   }
   // entrer and exit  of the maze
     maze[0 ][ 1] = 0;
    maze[(m - 1)][ (n - 2)] = 0;
}

/* Solve the maze */
///////////////////

bool solvemaze(bool maze[m][n],bool solution[m][n],unsigned int  x, unsigned int y)
{
    // did we reach the exit
    if((x==m-1) && (y==n-2))
    {
        solution[x][y] = 1;
        return 1;
    }
    
    //is there a path draw it in solution maze
   if(x<m && y<n && solution[x][y] == 0 && maze[x][y] == 0)
    { 
        solution[x][y] = 1;
        //down
        if(solvemaze(maze,solution,x+1, y))  return 1;
        //right
        if(solvemaze(maze,solution,x, y+1))  return 1;
        //up
        if(solvemaze(maze,solution,x-1, y))  return 1;
        //left
        if(solvemaze(maze,solution,x, y-1))  return 1;
        //no path then back
        solution[x][y] = 0;
        return 0;
    }
    return 0;
}
 
/* Display  maze */
//////////////////
void showmaze(bool maze[m][n])
{   
    unsigned int  x, y;
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

    generateMaze(maze,solution); 
    solvemaze(maze,solution,1,1); 
    showmaze(maze);
    showmaze(solution);

    return 0;
}