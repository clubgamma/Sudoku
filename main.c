#include "sudoku.h"
#include <stdio.h>

void inputBoard(Sudoku *s) {
    printf("Enter your Sudoku puzzle (use 0 or space for empty cells):\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            char input = getchar();

            if (input == '\n') {
                j--; 
                continue;
            }

            if (input == '0' || input == ' ') {
                s->board[i][j] = ' '; 
            } else {
                s->board[i][j] = input;  
            }
        }
    }
}

int main() {
    Sudoku s;
    int steps = 0; 

    inputBoard(&s);

    if (solveSudoku(&s, &steps)) {
        printGrid(&s);
        printf("Sudoku solved in %d steps.\n", steps);
    } else {
        printf("No solution exists.\n");
    }

    return 0;
}
