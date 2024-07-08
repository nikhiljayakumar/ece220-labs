#include <stdio.h>
#include <stdlib.h>

/*
 * First loop through the different numbers (zero-indexed) 
 * For each number, solve the product equation for binomial coefficient
 * Print out result, move to next num until row is finished
 */

int main()
{
  int row;

  printf("Enter a row index: ");
  scanf("%d",&row);

  unsigned long a; // holds binomial coefficient

  for (int i = 0; i <= row; i++){
	a = 1; // init to 1 for every new number
	for (int j = 1; j <= i; j++){
		a *= (row + 1 - j);
		a /= j;  // solve product equation for binomial coefficient
	}
	printf("%lu ", a); // print, and continue until all numbers are done
  }

  return 0;
}
