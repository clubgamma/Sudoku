#include "sudoku.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void inputBoard(Sudoku *s) {
    printf("Enter your Sudoku puzzle (use 0 or space for empty cells, and press Enter when a row is done)\n");
    for (int i = 0; i < SIZE; i++) {
        printf("Row %d :", i + 1);

        char rowCheck[SIZE + 1] = {0};
        char line[SIZE + 2];
        fgets(line, sizeof(line), stdin);
        line[strcspn(line, "\n")] = 0;

        if (strlen(line) != SIZE) {
            printf("Error: Each line must contain exactly 9 digits or spaces.\n");
            exit(1);
        }

        for (int j = 0; j < SIZE; j++) {
            char input = line[j];

            if (input == '0' || input == ' ') {
                s->board[i][j] = ' ';
            } else if (input >= '1' && input <= '9') {
                if (rowCheck[input - '0']) {
                    printf("Error: Duplicate number");
                    exit(1);
                }
                rowCheck[input - '0'] = 1;
                s->board[i][j] = input;
            } else {
                printf("Error: Invalid input. Please enter numbers between 1-9 or 0 or space for empty.\n");
                exit(1);
            }
        }
    }

    if (!validateInitialBoard(s)) {
        printf("Error: Invalid Sudoku puzzle. Conflicts found. Please check your input.\n");
        exit(1);
    }
}

int inputBoardFromFile(Sudoku *s, char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open the file. Ensure the file exists and is accessible.\n");
        return 0;
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            char input = fgetc(file);

            if (input == EOF) {
                printf("Error: Unexpected end of file. The file may be incomplete or corrupted.\n");
                fclose(file);
                return 0;
            }

            if (input == '\n') {
                j--; 
                continue;
            }

            if (input == '0' || input == ' ') {
                s->board[i][j] = ' ';  // Empty cell
            } else if (input >= '1' && input <= '9') {
                s->board[i][j] = input;  
            } else {
                printf("Error: Invalid character '%c' found in file. Only digits 1-9 and spaces/0 are allowed.\n", input);
                fclose(file);
                return 0;
            }
        }
    }

    fclose(file);

    if (!validateInitialBoard(s)) {
        printf("Error: The puzzle from the file contains conflicts.\n");
        return 0;
    }

    return 1;
}

int countClues(Sudoku *s) {
    int clueCount = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (s->board[i][j] != ' ') {
                clueCount++;
            }
        }
    }
    return clueCount;
}


int main(int argc, char *argv[]) {
    Sudoku s;
    int steps = 0;

    memset(rowMask, 0, sizeof(rowMask));
    memset(colMask, 0, sizeof(colMask));
    memset(boxMask, 0, sizeof(boxMask));

    if (argc == 2) {
        inputBoardFromFile(&s, argv[1]);
        if(!inputBoardFromFile(&s, argv[1])){
            return 0;
        }
    } else {
        inputBoard(&s);
    }

    int clueCount = countClues(&s);

    if (solveSudoku(&s, &steps)) {
        printGrid(&s);
        printf("Sudoku solved in %d steps.\n", steps);
        const char *difficulty = classifyDifficulty(clueCount, steps);
        printf("Puzzle difficulty: %s\n", difficulty);
    } else {
        printf("No solution exists.\n");
    }

    return 0;
}
