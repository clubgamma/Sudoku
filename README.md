# Sudoku Solver

This project implements a simple Sudoku solver using backtracking in C. The solver reads a Sudoku puzzle, solves it, and prints the solution along with the total steps taken to find the solution.

## NOTE FOR CONTRIBUTORS
If you are a beginner, don't worry. You have already been provided with a basic project structure. Your task now is to manage everything, and you are allowed to modify the structure as needed.

## Table of Contents

- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [File Structure](#file-structure)
- [How It Works](#how-it-works)
- [Contribution Guidelines](#contribution-guidelines-)
- [General Rules](#general-rules)
- [Getting Started](#getting-started-)
- [Avoiding Conflicts](#avoiding-conflicts-syncing-your-fork)

## Features

- Solves a 9x9 Sudoku puzzle.
- Displays the solved Sudoku grid.
- Shows the total number of steps taken to solve the puzzle.
- Uses a simple text-based user interface.

## Simple Sudoku
![example](./EXAMPLE/SIMPLE_SUDOKU.png)

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/clubgamma/Sudoku.git
   cd Sudoku

2. Compile the code using GCC:
   ```bash
   gcc main.c sudoku.c -o sudoku_solver

## Usage
To run the Sudoku solver:
   ```bash
   ./sudoku_solver
  ```


The program initializes a pre-defined Sudoku puzzle and attempts to solve it. Modify the board array in main.c to test different puzzles.

## File Structure
   ```bash
   Sudoku/
   ├── main.c       # Entry point of the Sudoku solver
   ├── sudoku.h     # Header file containing structure and function declarations
   └── sudoku.c     # Implementation of Sudoku solving logic
  ```

## Description of Files
- **main.c:** Contains the main function and initializes the Sudoku puzzle.
- **sudoku.h:** Defines the Sudoku structure and declares functions used in the solver.
- **sudoku.c:** Implements the logic for checking valid moves, solving the Sudoku, and printing the grid.

## How It Works
- The program initializes a Sudoku board with predefined values.
- It attempts to find an empty cell in the board.
- For each empty cell, it tries placing numbers from 1 to 9.
- If placing a number does not violate Sudoku rules, the program makes a recursive call to solve the rest of the board.
- If a valid solution is found, it prints the completed board and the total steps taken.
- If no solution exists, it informs the user.



4. **Keep Updated:**
   - Regularly pull changes from the upstream repository to keep your fork updated.


  
