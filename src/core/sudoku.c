#include "sudoku.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int rowMask[MAX_SIZE], colMask[MAX_SIZE], boxMask[MAX_SIZE];

typedef struct {
    int row;
    int col;
} Cell;

int getBoxIndex(int size, int row, int col) {
    int boxSize = (int)sqrt(size);
    return (row / boxSize) * boxSize + (col / boxSize);
}

int isValid(Sudoku *s, int row, int col, int num) {
    return !(rowMask[row] & (1 << (num - 1)) || 
             colMask[col] & (1 << (num - 1)) || 
             boxMask[getBoxIndex(s->size, row, col)] & (1 << (num - 1)));
}

void updateCellAvailability(Sudoku *s, Cell *emptyCells, int *emptyCount) {
    *emptyCount = 0;
    for (int row = 0; row < s->size; row++) {
        for (int col = 0; col < s->size; col++) {
            if (s->board[row][col] == ' ') {
                emptyCells[(*emptyCount)++] = (Cell){row, col};
            }
        }
    }
}

int findBestCell(Sudoku *s, Cell *emptyCells, int emptyCount) {
    int bestIndex = -1;
    int minOptions = s->size + 1;

    for (int i = 0; i < emptyCount; i++) {
        int row = emptyCells[i].row;
        int col = emptyCells[i].col;

        int options = 0;
        for (int num = 1; num <= s->size; num++) {
            if (isValid(s, row, col, num)) {
                options++;
            }
        }

        if (options < minOptions) {
            minOptions = options;
            bestIndex = i;
        }

        if (minOptions == 1) {
            break; // MRV heuristic: stop if we find a cell with only 1 option
        }
    }
    return bestIndex;
}

int optimizedSolveSudoku(Sudoku *s, int *steps) {
    Cell emptyCells[MAX_SIZE * MAX_SIZE];
    int emptyCount;

    // Initialize list of empty cells
    updateCellAvailability(s, emptyCells, &emptyCount);

    if (emptyCount == 0) {
        return 1;  // Puzzle is already complete
    }

    int cellIndex = findBestCell(s, emptyCells, emptyCount);
    if (cellIndex == -1) {
        return 0;  // No valid moves, puzzle unsolvable
    }

    int row = emptyCells[cellIndex].row;
    int col = emptyCells[cellIndex].col;

    for (int num = 1; num <= s->size; num++) {
        if (isValid(s, row, col, num)) {
            (*steps)++;
            // Directly set the number and update masks
            s->board[row][col] = num + '0';
            rowMask[row] |= (1 << (num - 1));
            colMask[col] |= (1 << (num - 1));
            boxMask[getBoxIndex(s->size, row, col)] |= (1 << (num - 1));

            // Recurse
            if (optimizedSolveSudoku(s, steps)) {
                return 1;
            }

            // Backtrack: remove the number and clear masks
            s->board[row][col] = ' ';
            rowMask[row] &= ~(1 << (num - 1));
            colMask[col] &= ~(1 << (num - 1));
            boxMask[getBoxIndex(s->size, row, col)] &= ~(1 << (num - 1));
        }
    }

    return 0;  // No valid solution found
}

void printGrid(Sudoku *s) {
    int boxSize = (int)sqrt(s->size);
    for (int row = 0; row < s->size; row++) {
        for (int col = 0; col < s->size; col++) {
            printf("%c ", s->board[row][col]);
            if ((col + 1) % boxSize == 0 && col < s->size - 1) {
                printf("| ");
            }
        }
        printf("\n");
        if ((row + 1) % boxSize == 0 && row < s->size - 1) {
            for (int i = 0; i < s->size + boxSize - 1; i++) {
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
                if (num < 1 || num > s->size) {
                    return 0;
                }
                if (!isValid(s, i, j, num)) {
                    return 0;
                }
                // Place initial numbers
                rowMask[i] |= (1 << (num - 1));
                colMask[j] |= (1 << (num - 1));
                boxMask[getBoxIndex(s->size, i, j)] |= (1 << (num - 1));
            }
        }
    }
    return 1;
}

const char* classifyDifficulty(int clueCount, int backtrackingSteps) {
    // Categorize based on clues and backtracking steps in one pass
    if (clueCount > 40) {
        return (backtrackingSteps < 50) ? "Easy" : "Medium";
    } else if (clueCount > 30) {
        return (backtrackingSteps < 100) ? "Medium" : "Hard";
    } else {
        return "Hard";
    }
}

