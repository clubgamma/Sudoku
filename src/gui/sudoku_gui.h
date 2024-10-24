#ifndef SUDOKU_GUI_H
#define SUDOKU_GUI_H

#include <gtk/gtk.h>
#include "sudoku.h"

typedef struct {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *size_combo;
    GtkWidget *solve_button;
    GtkWidget *clear_button;
    GtkWidget *status_label;
    GtkWidget *entries[MAX_SIZE][MAX_SIZE];
    GtkWidget *difficulty_label;
    Sudoku sudoku;
} SudokuGUI;

void create_sudoku_window(void);

#endif // SUDOKU_GUI_H