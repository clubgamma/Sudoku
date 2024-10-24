#include "sudoku.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void inputBoard(Sudoku *s) {
    printf("Enter the size of your Sudoku puzzle (4, 9, 16, 25): ");
    scanf("%d", &s->size);

    while (s->size != 4 && s->size != 9 && s->size != 16 && s->size != 25) {
        printf("Error: Unsupported size. Please enter 4, 9, 16, or 25: ");
        scanf("%d", &s->size);
    }

    while (getchar() != '\n');

    printf("Enter your Sudoku puzzle (use 0 or space for empty cells, and press Enter when a row is done):\n");
    for (int i = 0; i < s->size; i++) {
        int validRow = 0;
        while (!validRow) {
            printf("Row %d: ", i + 1);

            char rowCheck[MAX_SIZE + 1] = {0};
            char line[MAX_SIZE + 1];
            fgets(line, sizeof(line), stdin);
            line[strcspn(line, "\n")] = 0;

            if (strlen(line) != s->size) {
                printf("Error: Each line must contain exactly %d digits or spaces.\n", s->size);
                continue;
            }

            validRow = 1; // Assume the row is valid until proven otherwise
            for (int j = 0; j < s->size; j++) {
                char input = line[j];

                if (input == '0' || input == ' ') {
                    s->board[i][j] = ' ';
                } else if (input >= '1' && input <= '0' + s->size) {
                    if (rowCheck[input - '0']) {
                        printf("Error: Duplicate number '%c' found in row %d. Please correct it.\n", input, i + 1);
                        validRow = 0; // Mark this row as invalid
                        break; // Break out of the for loop to re-prompt
                    }
                    rowCheck[input - '0'] = 1;
                    s->board[i][j] = input;
                } else {
                    printf("Error: Invalid input '%c'. Please enter numbers between 1 and %d, or 0/space for empty.\n", input, s->size);
                    validRow = 0; // Mark this row as invalid
                    break; // Break out of the for loop to re-prompt
                }
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

    // Determine grid size
    char line[MAX_SIZE + 1];
    int rowCount = 0;
    int colCount = 0;

    // First count the number of rows and find the longest line to determine column count
    while (fgets(line, sizeof(line), file)) {
        rowCount++;
        int length = strcspn(line, "\n"); 
        if (length > colCount) {
            colCount = length;
        }
    }

    fseek(file, 0, SEEK_SET);

    if (rowCount != colCount || (rowCount != 4 && rowCount != 9 && rowCount != 16 && rowCount != 25)) {
        printf("Error: Invalid grid size detected. Expected 4, 9, 16, or 25, but got %d.\n", rowCount);
        fclose(file);
        return 0;
    }

    // Set the size of Sudoku
    s->size = rowCount;

    // Read the rest of the file to initialize the board
    for (int i = 0; i < s->size; i++) {
        fgets(line, sizeof(line), file);
        line[strcspn(line, "\n")] = 0; 

        for (int j = 0; j < s->size; j++) {
            char input = line[j];

            if (input == '0' || input == ' ') {
                s->board[i][j] = ' ';  
            } else if (input >= '1' && input <= '0' + s->size) {
                s->board[i][j] = input;
            } else {
                printf("Error: Invalid character '%c' found in file. Only digits 1-%d and spaces/0 are allowed.\n", input, s->size);
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
    for (int i = 0; i < s->size; i++) {
        for (int j = 0; j < s->size; j++) {
            if (s->board[i][j] != ' ') {
                clueCount++;
            }
        }
    }
    return clueCount;
}

int main(int argc, char *argv[]) {

    int playing = 1;

    while(playing) {
    Sudoku s;
    int steps = 0;
    int choice;

    memset(rowMask, 0, sizeof(rowMask));
    memset(colMask, 0, sizeof(colMask));
    memset(boxMask, 0, sizeof(boxMask));

    if (argc == 2) {
        if (!inputBoardFromFile(&s, argv[1])) {
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
        printf("Do you want to restart the game or exit?\n");
        printf("Press 1 to restart or 0 to exit: ");
        scanf("%d", &choice);
        
        if (choice == 0) {
            playing = 0; 
        }   
}
    printf("Thanks for playing!\n");
    return 0;
}
