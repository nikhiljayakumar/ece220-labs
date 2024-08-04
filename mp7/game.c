#include "game.h"
/* First create make_game() function that initalizes cols, rows to passed parameters, and initializes score to 0 and all cells to -1.
 * Then create remake game which does the same as make_game but to an already existing game passed as a parameter.
 * Then write get_cell() which returns the address of the cell in the row,col passed as a parameter.
 * Then write move_w,s,a,d() which have the following algorithm. I will describe it for move_w().
 *  - Since all elements are moving up, lets first iterate through each column.
 *  - In each column, there are two situtations where cells will be changed: when cells (of same number) needed to be added together, or when there is an empty space above a numbered cell.
 *  - Also, a merged cell cannot be merged again in the same move, so need to have a boolean array that keeps track of any merged cells. 
 *  - So iterate through the rows of the current column and merge cells if they can be or move a cell up by 1 if it can be. We will call one loop through all the rows of the column one "pass".
 *  - If there was any change (merge or move), then reset the rows and start again. Keep going until a pass doesn't change the column at all. This ensures all cells are in their final state.
 * Using the same algorithm (swap rows/cols or direction cells are moving accordingly), finish move_s,a,d().
 * Then write legal_move_check() which simply creates an exact copy of the board and check if any move changes the board (using prev. functions). If no then no legal move.
*/

game * make_game(int rows, int cols)
/*! Create an instance of a game structure with the given number of rows
    and columns, initializing elements to -1 and return a pointer
    to it. (See game.h for the specification for the game data structure) 
    The needed memory should be dynamically allocated with the malloc family
    of functions.
*/
{
    //Dynamically allocate memory for game and cells (DO NOT modify this)
    game * mygame = malloc(sizeof(game));
    mygame->cells = malloc(rows*cols*sizeof(cell));

    //YOUR CODE STARTS HERE:  Initialize all other variables in game struct
    mygame->rows = rows;
    mygame->cols = cols;
    mygame->score = 0;

    for (int i = 0; i < rows*cols; i++){
        mygame->cells[i] = -1;                // set rows,cols to passed parameter, score to 0, and cells to -1
    }

    return mygame;
}

void remake_game(game ** _cur_game_ptr,int new_rows,int new_cols)
/*! Given a game structure that is passed by reference, change the
	game structure to have the given number of rows and columns. Initialize
	the score and all elements in the cells to -1. Make sure that any 
	memory previously allocated is not lost in this function.	
*/
{
	/*Frees dynamically allocated memory used by cells in previous game,
	 then dynamically allocates memory for cells in new game.  DO NOT MODIFY.*/
	free((*_cur_game_ptr)->cells);
	(*_cur_game_ptr)->cells = malloc(new_rows*new_cols*sizeof(cell));

	 //YOUR CODE STARTS HERE:  Re-initialize all other variables in game struct
    (*_cur_game_ptr)->rows = new_rows;
    (*_cur_game_ptr)->cols = new_cols;
    (*_cur_game_ptr)->score = 0;  // since double pointer, de-reference first and then use ->

    for (int i = 0; i < new_rows*new_cols; i++){
        (*_cur_game_ptr)->cells[i] = -1;         // same exact as make_game(), just use board already passed as parameter.
    }

	return;	
}

void destroy_game(game * cur_game)
/*! Deallocate any memory acquired with malloc associated with the given game instance.
    This includes any substructures the game data structure contains. Do not modify this function.*/
{
    free(cur_game->cells);
    free(cur_game);
    cur_game = NULL;
    return;
}

cell * get_cell(game * cur_game, int row, int col)
/*! Given a game, a row, and a column, return a pointer to the corresponding
    cell on the game. (See game.h for game data structure specification)
    This function should be handy for accessing game cells. Return NULL
	if the row and col coordinates do not exist.
*/
{
    //YOUR CODE STARTS HERE
    if (cur_game->rows > row && row >= 0){
        if (cur_game->cols > col && col >= 0){
            return &(cur_game->cells[row*cur_game->cols+col]); // return address of selected cell, use row major order formula
        }
    }
    return NULL;
}

int move_w(game * cur_game)
/*!Slides all of the tiles in cur_game upwards. If a tile matches with the 
   one above it, the tiles are merged by adding their values together. When
   tiles merge, increase the score by the value of the new tile. A tile can 
   not merge twice in one turn. If sliding the tiles up does not cause any 
   cell to change value, w is an invalid move and return 0. Otherwise, return 1. 
*/
{
    // algorithm described in summary at beginning
    int anyChange = 0; // used for return value
    int colSize = cur_game->cols;
    int rowSize = cur_game->rows;
    int swap;
    int changed[rowSize]; // check if two cells have already been merged
    
    for (int col = 0; col < colSize; col++){
        swap = 1; // initate pass through the rows of a selected column
        for (int i = 0; i < rowSize; i++) changed[i] = 0;
        
        while (swap){
            swap = 0; // set to 0 before pass, if anything changed swap = 1

            for (int row = 0; row < rowSize-1; row++){ // since loop accesses row+1, cap out at rowSize-1

                if ((*get_cell(cur_game, row, col) == (*get_cell(cur_game, row+1, col))) && (*get_cell(cur_game, row+1, col) != -1)){ // if two cells of the same number are next to each other add them
                    if (changed[row] || changed[row+1]){ // if cells have already been merged then skip
                        continue;
                    }else{
                        *get_cell(cur_game, row, col) *= 2;  // merge
                        cur_game->score += *get_cell(cur_game, row, col);  // add to score
                        *get_cell(cur_game, row+1, col) = -1; // cell used in merge becomes empty
                        changed[row] = 1; // set to merged
                        swap = 1; // swap occurred, restart pass from row = 0
                        anyChange = 1; // now return value set to 1
                        break; // start from row=0 again
                    }
                }else if ((*get_cell(cur_game, row, col) == -1) && ((*get_cell(cur_game, row+1, col) != -1 ))){ // if the cell above is -1 and the current cell is numbered
                        *get_cell(cur_game, row, col) = *get_cell(cur_game, row+1, col); 
                        *get_cell(cur_game, row+1, col) = -1;
                        swap = 1;
                        anyChange = 1;
                        break; // same idea, just move the value up by one row
                }
            }
        }
    }
    
    return anyChange == 1 ? 1 : 0;
};

int move_s(game * cur_game) //slide down
{
    // same algorithm just iterate from bottom row up, and check if row below instead of above
    int anyChange = 0;
    int colSize = cur_game->cols;
    int rowSize = cur_game->rows;
    int swap;
    int changed[rowSize];
    
    for (int col = 0; col < colSize; col++){
        swap = 1;
        for (int i = 0; i < rowSize; i++) changed[i] = 0;
        
        while (swap){
            swap = 0;

            for (int row = rowSize-1; row > 0; row--){ // start from max row

                if ((*get_cell(cur_game, row, col) == (*get_cell(cur_game, row-1, col))) && (*get_cell(cur_game, row-1, col) != -1)){ // row-1 for move_s()
                    if (changed[row] || changed[row-1]){
                        continue;
                    }else{
                        *get_cell(cur_game, row, col) *= 2;  
                        cur_game->score += *get_cell(cur_game, row, col);  
                        *get_cell(cur_game, row-1, col) = -1; 
                        changed[row] = 1;
                        swap = 1;
                        anyChange = 1;
                        break;
                    }
                }else if ((*get_cell(cur_game, row, col) == -1) && ((*get_cell(cur_game, row-1, col) != -1 ))){
                        *get_cell(cur_game, row, col) = *get_cell(cur_game, row-1, col); 
                        *get_cell(cur_game, row-1, col) = -1;
                        swap = 1;
                        anyChange = 1;
                        break;
		}
            }
        }
    }
    
    return anyChange == 1 ? 1 : 0;
};

int move_a(game * cur_game) //slide left
{
    // swap columns and rows moving left so start from col = 0
    int anyChange = 0;
    int colSize = cur_game->cols;
    int rowSize = cur_game->rows;
    int swap;
    int changed[colSize];
    
    for (int row = 0; row < rowSize; row++){
        swap = 1;
        for (int j = 0; j < colSize; j++) changed[j] = 0;
        
        while (swap){
            swap = 0;

            for (int col = 0; col < colSize-1; col++){

                if ((*get_cell(cur_game, row, col) == (*get_cell(cur_game, row, col+1))) && (*get_cell(cur_game, row, col+1) != -1)){ // same thing as move_w(), just swap cols and rows
                    if (changed[row] || changed[col+1]){
                        continue;
                    }else{
                        *get_cell(cur_game, row, col) *= 2;   
                        cur_game->score += *get_cell(cur_game, row, col); 
                        *get_cell(cur_game, row, col+1) = -1; 
                        changed[col] = 1;
                        swap = 1;
                        anyChange = 1;
                        break;
                    }
                }else if ((*get_cell(cur_game, row, col) == -1) && ((*get_cell(cur_game, row, col+1) != -1 ))){
                        *get_cell(cur_game, row, col) = *get_cell(cur_game, row, col+1); 
                        *get_cell(cur_game, row, col+1) = -1;
                        swap = 1;
                        anyChange = 1;
                        break;
                }
            }
        }
    }
    
    return anyChange == 1 ? 1 : 0;
};

int move_d(game * cur_game){ //slide to the right
    // same thing as move_s() just change col+1 to col-1
    int anyChange = 0;
    int colSize = cur_game->cols;
    int rowSize = cur_game->rows;
    int swap;
    int changed[colSize];
    
    for (int row = 0; row < rowSize; row++){
        swap = 1;
        for (int j = 0; j < colSize; j++) changed[j] = 0;
        
        while (swap){
            swap = 0;

            for (int col = colSize-1; col > 0; col--){

                if ((*get_cell(cur_game, row, col) == (*get_cell(cur_game, row, col-1))) && (*get_cell(cur_game, row, col-1) != -1)){ // same thing as move_s() just change col+1 to col-1
                    if (changed[row] || changed[col-1]){
                        continue;
                    }else{
                        *get_cell(cur_game, row, col) *= 2; 
                        cur_game->score += *get_cell(cur_game, row, col);  
                        *get_cell(cur_game, row, col-1) = -1; 
                        changed[col] = 1;
                        swap = 1;
                        anyChange = 1;
                        break;
                    }
                }else if ((*get_cell(cur_game, row, col) == -1) && ((*get_cell(cur_game, row, col-1) != -1))){
                        *get_cell(cur_game, row, col) = *get_cell(cur_game, row, col-1); 
                        *get_cell(cur_game, row, col-1) = -1;
                        swap = 1;
                        anyChange = 1;
                        break;
		}
            }
        }
    }
    
    return anyChange == 1 ? 1 : 0;
};

int legal_move_check(game * cur_game)
/*! Given the current game check if there are any legal moves on the board. There are
    no legal moves if sliding in any direction will not cause the game to change.
	Return 1 if there are possible legal moves, 0 if there are none.
 */
{
    game* copy = malloc(sizeof(game));
    copy->cells = malloc(cur_game->rows*cur_game->cols*sizeof(cell));

    copy->rows = cur_game->rows;
    copy->cols = cur_game->cols;
    copy->score = cur_game->score;     
    for (int i = 0; i < cur_game->rows*cur_game->cols; i++){
        copy->cells[i] = cur_game->cells[i];
    }    // allocate memory and make exact copy of board

    return move_w(copy) || move_a(copy) || move_s(copy) || move_d(copy); // if any move possible return 1
    
}
/*! code below is provided and should not be changed */

void rand_new_tile(game * cur_game)
/*! insert a new tile into a random empty cell. First call rand()%(rows*cols) to get a random value between 0 and (rows*cols)-1.
*/
{
	
	cell * cell_ptr;
    cell_ptr = 	cur_game->cells;
	
    if (cell_ptr == NULL){ 	
        printf("Bad Cell Pointer.\n");
        exit(0);
    }
	
	
	//check for an empty cell
	int emptycheck = 0;
	int i;
	
	for(i = 0; i < ((cur_game->rows)*(cur_game->cols)); i++){
		if ((*cell_ptr) == -1){
				emptycheck = 1;
				break;
		}		
        cell_ptr += 1;
	}
	if (emptycheck == 0){
		printf("Error: Trying to insert into no a board with no empty cell. The function rand_new_tile() should only be called after tiles have succesfully moved, meaning there should be at least 1 open spot.\n");
		exit(0);
	}
	
    int ind,row,col;
	int num;
    do{
		ind = rand()%((cur_game->rows)*(cur_game->cols));
		col = ind%(cur_game->cols);
		row = ind/cur_game->cols;
    } while ( *get_cell(cur_game, row, col) != -1);
        //*get_cell(cur_game, row, col) = 2;
	num = rand()%20;
	if(num <= 1){
		*get_cell(cur_game, row, col) = 4; // 1/10th chance
	}
	else{
		*get_cell(cur_game, row, col) = 2;// 9/10th chance
	}
}

int print_game(game * cur_game) 
{
    cell * cell_ptr;
    cell_ptr = 	cur_game->cells;

    int rows = cur_game->rows;
    int cols = cur_game->cols;
    int i,j;
	
	printf("\n\n\nscore:%d\n",cur_game->score); 
	
	
	printf("\u2554"); // topleft box char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // top box char
	printf("\u2557\n"); //top right char 
	
	
    for(i = 0; i < rows; i++){
		printf("\u2551"); // side box char
        for(j = 0; j < cols; j++){
            if ((*cell_ptr) == -1 ) { //print asterisks
                printf(" **  "); 
            }
            else {
                switch( *cell_ptr ){ //print colored text
                    case 2:
                        printf("\x1b[1;31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4:
                        printf("\x1b[1;32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8:
                        printf("\x1b[1;33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 16:
                        printf("\x1b[1;34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 32:
                        printf("\x1b[1;35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 64:
                        printf("\x1b[1;36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 128:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 256:
                        printf("\x1b[32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 512:
                        printf("\x1b[33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 1024:
                        printf("\x1b[34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 2048:
                        printf("\x1b[35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4096:
                        printf("\x1b[36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8192:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
					default:
						printf("  X  ");

                }

            }
            cell_ptr++;
        }
	printf("\u2551\n"); //print right wall and newline
    }
	
	printf("\u255A"); // print bottom left char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // bottom char
	printf("\u255D\n"); //bottom right char
	
    return 0;
}

int process_turn(const char input_char, game* cur_game) //returns 1 if legal move is possible after input is processed
{ 
	int rows,cols;
	char buf[200];
	char garbage[2];
    int move_success = 0;
	
    switch ( input_char ) {
    case 'w':
        move_success = move_w(cur_game);
        break;
    case 'a':
        move_success = move_a(cur_game);
        break;
    case 's':
        move_success = move_s(cur_game);
        break;
    case 'd':
        move_success = move_d(cur_game);
        break;
    case 'q':
        destroy_game(cur_game);
        printf("\nQuitting..\n");
        return 0;
        break;
	case 'n':
		//get row and col input for new game
		dim_prompt: printf("NEW GAME: Enter dimensions (rows columns):");
		while (NULL == fgets(buf,200,stdin)) {
			printf("\nProgram Terminated.\n");
			return 0;
		}
		
		if (2 != sscanf(buf,"%d%d%1s",&rows,&cols,garbage) ||
		rows < 0 || cols < 0){
			printf("Invalid dimensions.\n");
			goto dim_prompt;
		} 
		
		remake_game(&cur_game,rows,cols);
		
		move_success = 1;
		
    default: //any other input
        printf("Invalid Input. Valid inputs are: w, a, s, d, q, n.\n");
    }

	
	
	
    if(move_success == 1){ //if movement happened, insert new tile and print the game.
         rand_new_tile(cur_game); 
		 print_game(cur_game);
    } 

    if( legal_move_check(cur_game) == 0){  //check if the newly spawned tile results in game over.
        printf("Game Over!\n");
        return 0;
    }
    return 1;
}
