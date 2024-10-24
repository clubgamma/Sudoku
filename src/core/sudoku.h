#ifndef SUDOKU_H
#define SUDOKU_H

#define MAX_SIZE 25 

typedef struct {
    char board[MAX_SIZE][MAX_SIZE];
    int size; 
} Sudoku;

// Core game logic functions
int isValid(Sudoku *s, int row, int col, int num);
int solveSudoku(Sudoku *s, int *steps);
void printGrid(Sudoku *s);
void placeOrRemoveNumber(Sudoku *s, int row, int col, int num, int place);
int validateInitialBoard(Sudoku *s);
const char* classifyDifficulty(int clueCount, int backtrackingSteps);
int getBoxIndex(int size, int row, int col);

// Global masks for optimization
extern int rowMask[MAX_SIZE], colMask[MAX_SIZE], boxMask[MAX_SIZE];

#endif