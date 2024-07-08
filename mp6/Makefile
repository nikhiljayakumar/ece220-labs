
mp6: main.c sudoku.c
	g++ -Wall -g -c sudoku.c -o sudoku.o
	g++ -Wall -g -c main.c -o main.o
	g++ -Wall -g main.o sudoku.o sudoku_golden.a -o mp6

clean:
	rm *.o mp6

sudoku1: mp6
	./mp6 sudoku1.txt

sudoku2: mp6
	./mp6 sudoku2.txt

sudoku3: mp6
	./mp6 sudoku3.txt

