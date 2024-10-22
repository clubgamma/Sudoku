#include <stdio.h>
#include "sudoku.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 

int rowMask[MAX_SIZE], colMask[MAX_SIZE], boxMask[MAX_SIZE];

int getBoxIndex(int size, int row, int col) {
    int boxSize = (int)sqrt(size);
    return (row / boxSize) * boxSize + (col / boxSize);
}

int isValid(Sudoku *s, int row, int col, int num) {
    return !(rowMask[row] & (1 << (num - 1)) || 
             colMask[col] & (1 << (num - 1)) || 
             boxMask[getBoxIndex(s->size, row, col)] & (1 << (num - 1)));
}

void placeOrRemoveNumber(Sudoku *s, int row, int col, int num, int place) {
    if (place) {
        s->board[row][col] = num + '0';
        rowMask[row] |= (1 << (num - 1));
        colMask[col] |= (1 << (num - 1));
        boxMask[getBoxIndex(s->size, row, col)] |= (1 << (num - 1));
    } else {
        s->board[row][col] = ' ';
        rowMask[row] &= ~(1 << (num - 1));
        colMask[col] &= ~(1 << (num - 1));
        boxMask[getBoxIndex(s->size, row, col)] &= ~(1 << (num - 1));
    }
}

int solveSudoku(Sudoku *s, int *steps) {
    int row, col;
    // Find an empty cell
    for (row = 0; row < s->size; row++) {
        for (col = 0; col < s->size; col++) {
            if (s->board[row][col] == ' ') {
                for (int num = 1; num <= s->size; num++) {
                    if (isValid(s, row, col, num)) {
                        placeOrRemoveNumber(s, row, col, num, 1); // Place the number
                        (*steps)++;
                        if (solveSudoku(s, steps)) {
                            return 1; 
                        }
                        placeOrRemoveNumber(s, row, col, num, 0); // Remove the number
                    }
                }
                return 0; // Backtrack
            }
        }
    }
    return 1;
}

void printGrid(Sudoku *s) {
    for (int row = 0; row < s->size; row++) {
        for (int col = 0; col < s->size; col++) {
            printf("%c ", s->board[row][col]);
            if ((col + 1) % (int)sqrt(s->size) == 0 && col < s->size - 1) {
                printf("| ");
            }
        }
        printf("\n");
        if ((row + 1) % (int)sqrt(s->size) == 0 && row < s->size - 1) {
            for (int i = 0; i < s->size + (int)sqrt(s->size) - 1; i++) {
                printf("-");
            }
            printf("\n");
        }
    }
}

int validateInitialBoard(Sudoku *s) {
    memset(rowMask, 0, sizeof(rowMask));
    memset(colMask, 0, sizeof(colMask));
    memset(boxMask, 0, sizeof(boxMask));

    for (int i = 0; i < s->size; i++) {
        for (int j = 0; j < s->size; j++) {
            if (s->board[i][j] != ' ') {
                int num = s->board[i][j] - '0';
                if (!isValid(s, i, j, num)) {
                    return 0; // Invalid number found
                }
                placeOrRemoveNumber(s, i, j, num, 1); // Place the number
            }
        }
    }
    return 1; // All numbers are valid
}

const char* classifyDifficulty(int clueCount, int backtrackingSteps) {
    // Example classification logic
    if (clueCount > 40 && backtrackingSteps < 50) return "Easy";
    if (clueCount > 30 && backtrackingSteps < 100) return "Medium";
    return "Hard";
}