
#ifndef SUDOKU_H
#define SUDOKU_H

#define SIZE 9

typedef struct Sudoku {
    char board[SIZE][SIZE];
} Sudoku;

int isValid(Sudoku *s, int row, int col, int num);
int solveSudoku(Sudoku *s, int *steps);
void printGrid(Sudoku *s);
void placeOrRemoveNumber(Sudoku *s, int row, int col, int num, int place);
int validateInitialBoard(Sudoku *s);

extern int rowMask[SIZE], colMask[SIZE], boxMask[SIZE];

#endif
