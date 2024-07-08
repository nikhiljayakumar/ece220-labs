/*
 * countLiveNeighbor
 * Inputs:
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * row: the row of the cell that needs to count alive neighbors.
 * col: the col of the cell that needs to count alive neighbors.
 * Output:
 * return the number of alive neighbors. There are at most eight neighbors.
 * Pay attention for the edge and corner cells, they have less neighbors.
 */


// First created countLiveNeighbor with provided pseudocode, checks the amount of alive neighbors
// Second created updateBoard which uses countLiveNeighbors to update the board
// Third created aliveStable which returns true if the next generation is the exact same to the current generation

int countLiveNeighbor(int* board, int boardRowSize, int boardColSize, int row, int col){

    int liveNeighbors = 0;
    int center = boardColSize * row + col;

    int curCell;

    for (int i = row-1; i <= row+1; i++){
        if (i >= 0 && i < boardRowSize){ // checks if rows surrounding are within board bounds
            for (int j = col-1; j <= col+1; j++){
                if (j >= 0 && j < boardColSize){ // checks if cols surrounding are within board bounds
                    curCell = i * boardColSize + j;
                    if (curCell != center){
                        if (board[curCell]) liveNeighbors++; // if not center cell and is alive then increment liveNeighbors
                    }
                }
            }
        }
    }
    return liveNeighbors;
}
/*
 * Update the game board to the next step.
 * Input: 
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: board is updated with new values for next step.
 */
void updateBoard(int* board, int boardRowSize, int boardColSize) {

    int liveNeighbors, curIdx;
    int originalBoard[boardRowSize*boardColSize];

    for (int i = 0; i < boardRowSize * boardColSize; i++){
        originalBoard[i] = board[i]; // make copy of current generation so that parts of new generation don't affect other parts
    }

    for (int i = 0; i < boardRowSize; i++){
        for(int j = 0; j < boardColSize; j++){
            liveNeighbors = countLiveNeighbor(originalBoard, boardRowSize, boardColSize, i, j); // count neighbors of current cell
            curIdx = i * boardColSize + j;
            if (board[curIdx] == 1){
                if (liveNeighbors == 2 || liveNeighbors == 3) board[curIdx] = 1; // if alive and 2 or 3 neighbors then still alive, otherwise die
                else board[curIdx] = 0;
            }
            else{
                if (liveNeighbors == 3) board[curIdx] = 1; // if dead and 3 neighbors then become alive, otherwise stay dead
            }
        }
    }

}

/*
 * aliveStable
 * Checks if the alive cells stay the same for next step
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: return 1 if the alive cells for next step is exactly the same with 
 * current step.
 * return 0 if the alive cells change for the next step.
 */ 
int aliveStable(int* board, int boardRowSize, int boardColSize){

    int liveNeighbors, curIdx;

    // basically exact same as updateBoard function just doesn't update the board

    int originalBoard[boardRowSize*boardColSize];
    for (int i = 0; i < boardRowSize * boardColSize; i++){
        originalBoard[i] = board[i];
    }

    for (int i = 0; i < boardRowSize; i++){
        for(int j = 0; j < boardColSize; j++){
            liveNeighbors = countLiveNeighbor(originalBoard, boardRowSize, boardColSize, i, j);
            curIdx = i * boardColSize + j;
            if (board[curIdx] == 1){
                if (!(liveNeighbors == 2 || liveNeighbors == 3)) return 0; // if alive->dead then return 0 because then its not the exact same
            }
            else{
                if (liveNeighbors == 3) return 0; // if dead->alive then return 0 because its not the exact same
            }
        }
    }
    return 1; // went through whole board and checked if any values are going to be updated, if reached this point then exactly the same, and return true

}

				
				
			

