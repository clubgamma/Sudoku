#include "sudoku_gui.h"
#include "sudoku.h"


int main(int argc, char *argv[]) {
    // Initialize GTK
    gtk_init(&argc, &argv);
    
    // Create and show the main window
    create_sudoku_window();
    
    // Start the GTK main loop
    gtk_main();
    return 0;
}