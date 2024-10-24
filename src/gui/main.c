#include "sudoku_gui.h"
#include "sudoku.h"

// Global masks from sudoku.h
int rowMask[MAX_SIZE], colMask[MAX_SIZE], boxMask[MAX_SIZE];

int main(int argc, char *argv[]) {
    // Initialize GTK
    gtk_init(&argc, &argv);
    
    // Create and show the main window
    create_sudoku_window();
    
    // Start the GTK main loop
    gtk_main();
    return 0;
}