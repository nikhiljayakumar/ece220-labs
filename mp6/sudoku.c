#include "sudoku.h"

//-------------------------------------------------------------------------------------------------
// Start here to work on your MP6
//-------------------------------------------------------------------------------------------------


// First created is_val_in_row which iterates through the provided row to check if val is not present
// Second created is_val_in_row which iterates through the provided col to check if val is not present
// Third created is_val_in_3x3_zone which first gets the top left corner of the 3x3 zone of the provided cell (i,j)
//   then iterates through the 3x3 zone and checks if the val is not present
// Fourth used the previous three functions to check if a val can be placed in a position through the is_val_valid function
// Finally created solve_sudoku with the provided pseudocode

// You are free to declare any private functions if needed.

// Function: is_val_in_row
// Return true if "val" already existed in ith row of array sudoku.
int is_val_in_row(const int val, const int i, const int sudoku[9][9]) {

  assert(i>=0 && i<9);

  // BEG TODO
  for (int k = 0; k < 9; k++){
    if (sudoku[i][k] == val) return 1; // iterates through the provided row to check if val is not present
  }
  return 0;
  // END TODO
}

// Function: is_val_in_col
// Return true if "val" already existed in jth column of array sudoku.
int is_val_in_col(const int val, const int j, const int sudoku[9][9]) {

  assert(j>=0 && j<9);

  // BEG TODO
  for (int k = 0; k < 9; k++){
    if (sudoku[k][j] == val) return 1; // iterates through the provided col to check if val is not present
  }
  
  return 0;
  // END TODO
}

// Function: is_val_in_3x3_zone
// Return true if val already existed in the 3x3 zone corresponding to (i, j)
int is_val_in_3x3_zone(const int val, const int i, const int j, const int sudoku[9][9]) {
   
  assert(i>=0 && i<9);
  
  // BEG TODO
  int topCornerRow = (i / 3) * 3;
  int topCornerCol = (j / 3) * 3; // gets the top left corner of the 3x3 zone of the provided cell (i,j)

  for (int m = topCornerRow; m < topCornerRow+3; m++){
    for (int n = topCornerCol; n < topCornerCol+3; n++){ // iterates through the 3x3 zone (starting from top Left Corner) and checks if the val is not present
      if (sudoku[m][n] == val) return 1;
    }
  }
  return 0;
  // END TODO
}

// Function: is_val_valid
// Return true if the val is can be filled in the given entry.
int is_val_valid(const int val, const int i, const int j, const int sudoku[9][9]) {

  assert(i>=0 && i<9 && j>=0 && j<9);
  
  // BEG TODO
  return !(is_val_in_col(val, j, sudoku) || is_val_in_row(val, i, sudoku) || is_val_in_3x3_zone(val, i, j, sudoku)); // if in column, row, or 3x3 zone, then its not valid
  // END TODO
}

// Procedure: solve_sudoku
// Solve the given sudoku instance.
int solve_sudoku(int sudoku[9][9]) {

  // BEG TODO.
  int i, j;

  int unfilled = 0;

  for (i = 0; i < 9; i++){
    for (j = 0; j < 9; j++){
      if (sudoku[i][j] == 0){
        unfilled = 1; // if unfilled, then break out of both loops
        break;
      }
    }
    if (unfilled) break;
  }
  if (!unfilled) return 1; // if no values unfilled, sudoku is done and return true

  for (int num = 1; num <= 9; num++){
    if (is_val_valid(num, i, j, sudoku)){ // check if value can work
      sudoku[i][j] = num;
      if (solve_sudoku(sudoku)) { // recursive call to see if value can work with other values
        return 1; // if returns true then sudoku complete
      }
      sudoku[i][j] = 0; // if not possible then fill with 0 and try with other num
    }
  }
  return 0; // value can't work with others, go back
  // END TODO.
}

// Procedure: print_sudoku
void print_sudoku(int sudoku[9][9])
{
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      printf("%2d", sudoku[i][j]);
    }
    printf("\n");
  }
}

// Procedure: parse_sudoku
void parse_sudoku(const char fpath[], int sudoku[9][9]) {
  FILE *reader = fopen(fpath, "r");
  assert(reader != NULL);
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      fscanf(reader, "%d", &sudoku[i][j]);
    }
  }
  fclose(reader);
}





