#include <stdio.h>
#include "sudoku.h"

int rowMask[SIZE], colMask[SIZE], boxMask[SIZE];

int isValid(Sudoku *s, int row, int col, int num) {
    return !(rowMask[row] & (1 << (num - 1)) || 
             colMask[col] & (1 << (num - 1)) || 
             boxMask[(row / 3) * 3 + (col / 3)] & (1 << (num - 1)));
}

void placeOrRemoveNumber(Sudoku *s, int row, int col, int num, int place) {
    if (place) {
        s->board[row][col] = num + '0';
        rowMask[row] |= (1 << (num - 1));
        colMask[col] |= (1 << (num - 1));
        boxMask[(row / 3) * 3 + (col / 3)] |= (1 << (num - 1));
    } else {
        s->board[row][col] = ' ';
        rowMask[row] &= ~(1 << (num - 1));
        colMask[col] &= ~(1 << (num - 1));
        boxMask[(row / 3) * 3 + (col / 3)] &= ~(1 << (num - 1));
    }
}

int solveSudoku(Sudoku *s, int *steps) {
    int row, col;
    // Find an empty cell
    for (row = 0; row < SIZE; row++) {
        for (col = 0; col < SIZE; col++) {
            if (s->board[row][col] == ' ') {
                for (int num = 1; num <= 9; num++) {
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
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            printf("%c ", s->board[row][col]);
            if ((col + 1) % 3 == 0 && col < SIZE - 1) {
                printf("| ");
            }
        }
        printf("\n");
        if ((row + 1) % 3 == 0 && row < SIZE - 1) {
            printf("---------------------\n");
        }
    }
}
int validateInitialBoard(Sudoku *s) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (s->board[row][col] != ' ') {
                int num = s->board[row][col] - '0';
                s->board[row][col] = ' ';
                if (!isValid(s, row, col, num)) {
                    s->board[row][col] = num + '0';
                    return 0;
                }
                s->board[row][col] = num + '0';
            }
        }
    }
    return 1;
}
