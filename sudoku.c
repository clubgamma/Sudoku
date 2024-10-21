#include <stdio.h>
#include "sudoku.h"

int isValid(Sudoku *s, int row, int col, int num) {
    char ch = num + '0';

    for (int x = 0; x < SIZE; x++) {
        if (s->board[row][x] == ch) {
            return 0;
        }
    }

    for (int x = 0; x < SIZE; x++) {
        if (s->board[x][col] == ch) {
            return 0;
        }
    }

    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (s->board[i + startRow][j + startCol] == ch) {
                return 0;
            }
        }
    }

    return 1;
}

int findEmpty(Sudoku *s, int *row, int *col) {
    for (*row = 0; *row < SIZE; (*row)++) {
        for (*col = 0; *col < SIZE; (*col)++) {
            if (s->board[*row][*col] == ' ') {
                return 1;
            }
        }
    }
    return 0;
}

int solveSudoku(Sudoku *s, int *steps) {
    int row, col;

    if (!findEmpty(s, &row, &col)) {
        return 1;
    }

    for (int num = 1; num <= 9; num++) {
        if (isValid(s, row, col, num)) {
            s->board[row][col] = num + '0';
            (*steps)++;

            if (solveSudoku(s, steps)) {
                return 1;
            }

            s->board[row][col] = ' ';
        }
    }

    return 0;
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

const char* classifyDifficulty(int clueCount, int backtrackingSteps) {
    int score = 40 - clueCount + (backtrackingSteps / 10);

    if (score < 10) {
        return "Easy";
    } else if (score >= 10 && score <= 19) {
        return "Medium";
    } else if (score >= 20 && score <= 29) {
        return "Hard";
    } else {
        return "Expert";
    }
}
