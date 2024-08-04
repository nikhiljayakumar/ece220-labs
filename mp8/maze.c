#include <stdio.h>
#include <stdlib.h>
#include "maze.h"

/*
First write createMaze function
 - first open a file and use malloc to create the maze struct memory spot
 - then use fscanf() to read dimensions and use malloc() to create the cells memory spot
 - then iterate through the rows and use malloc() for each *cells memory spot
 - use fgetc() to get each character in the board, use an extra fgetc() to catch the newline between the loops,
 - also check and store location of start and end within loop
Then create destroyMaze() which first frees each row of cells then the cells then the entire struct
Then create printMaze() which goes through each row and prints it using cells[i]
Then create solveMazeDFS() which uses the algorithm described in the readme
 - also restores the 'S' when returning 1, and checks if its not the start or end if it is not a empty space.
*/

/*
 * createMaze -- Creates and fills a maze structure from the given file
 * INPUTS:       fileName - character array containing the name of the maze file
 * OUTPUTS:      None 
 * RETURN:       A filled maze structure that represents the contents of the input file
 * SIDE EFFECTS: None
 */
maze_t * createMaze(char * fileName)
{
    FILE* f = fopen(fileName, "r");
    if (f == NULL){
        fclose(f);
        return NULL; // cannot open file for read
    }
    maze_t* maze = (maze_t*) malloc(sizeof(maze_t)); // entire struct memory
    fscanf(f, "%d %d\n", &maze->width, &maze->height); // get dimensions
    maze->cells = (char**) malloc(sizeof(char*) * maze->height);
    char cur;

    for (int i = 0; i < maze->height; i++){
        maze->cells[i] = (char*) malloc(sizeof(char) * maze->width);
        for (int j = 0; j < maze->width; j++){
            cur = (char) fgetc(f);
            if (cur == 'S'){
                maze->startRow = i;
                maze->startColumn = j;
            }
            else if (cur == 'E'){
                maze->endRow = i;
                maze->endColumn = j;
            }
            maze->cells[i][j] = cur;
        }
        fgetc(f); // newline character
    }
    return maze;
    
}

/*
 * destroyMaze -- Frees all memory associated with the maze structure, including the structure itself
 * INPUTS:        maze -- pointer to maze structure that contains all necessary information 
 * OUTPUTS:       None
 * RETURN:        None
 * SIDE EFFECTS:  All memory that has been allocated for the maze is freed
 */
void destroyMaze(maze_t * maze)
{
    for (int i = 0; i < maze->height; i++)
        free(maze->cells[i]); // first free each cell row because if free struct first then losing this information
    free(maze->cells);
    free(maze);
}

/*
 * printMaze --  Prints out the maze in a human readable format (should look like examples)
 * INPUTS:       maze -- pointer to maze structure that contains all necessary information 
 *               width -- width of the maze
 *               height -- height of the maze
 * OUTPUTS:      None
 * RETURN:       None
 * SIDE EFFECTS: Prints the maze to the console
 */
void printMaze(maze_t * maze)
{
    for (int i = 0; i < maze->height; i++){
        printf("%s\n", maze->cells[i]); // print each row with a newline seperate
    }
}

/*
 * solveMazeManhattanDFS -- recursively solves the maze using depth first search,
 * INPUTS:               maze -- pointer to maze structure with all necessary maze information
 *                       col -- the column of the cell currently beinging visited within the maze
 *                       row -- the row of the cell currently being visited within the maze
 * OUTPUTS:              None
 * RETURNS:              0 if the maze is unsolvable, 1 if it is solved
 * SIDE EFFECTS:         Marks maze cells as visited or part of the solution path
 */ 
int solveMazeDFS(maze_t * maze, int col, int row)
{
    if (col < 0 || row < 0 || col >= maze->width || row >= maze->height) // if out of bounds then return 0
        return 0;

    char curCell = maze->cells[row][col];
    if (curCell != EMPTY)
        if (curCell != START && curCell != END)
            return 0; // if an not an empty cell AND not the start and end cell then return 0

    if (curCell == END){
        maze->cells[maze->startRow][maze->startColumn] = START; // if reached end restore start spot (cause got updated with ~ or *)
        return 1;
    }

    maze->cells[row][col] = PATH; // try this to be the path

    if (solveMazeDFS(maze, col-1, row)) // try left
        return 1;
    if (solveMazeDFS(maze, col+1, row)) // try right
        return 1;
    if (solveMazeDFS(maze, col, row-1)) // try up
        return 1;
    if (solveMazeDFS(maze, col, row+1)) // try down
        return 1;

    maze->cells[row][col] = VISITED; // if this not the path backtrack and return false

    return 0;
}
