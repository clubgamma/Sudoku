#include "sudoku_gui.h"
#include <ctype.h>
#include <math.h>

static SudokuGUI gui;

static void update_grid_size(int new_size) {
    // Remove existing grid widgets
    if (gui.grid != NULL) {
        gtk_widget_destroy(gui.grid);
    }

    // Create new grid
    gui.grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(gui.grid), 2);
    gtk_grid_set_column_spacing(GTK_GRID(gui.grid), 2);

    // Calculate box size for borders
    int box_size = (int)sqrt(new_size);

    // Create entries for each cell
    for (int i = 0; i < new_size; i++) {
        for (int j = 0; j < new_size; j++) {
            GtkWidget *entry = gtk_entry_new();
            gtk_entry_set_max_length(GTK_ENTRY(entry), 2);
            gtk_entry_set_width_chars(GTK_ENTRY(entry), 2);
            gtk_widget_set_hexpand(entry, TRUE);
            gtk_widget_set_vexpand(entry, TRUE);

            // Add borders for box separation
            GtkStyleContext *context = gtk_widget_get_style_context(entry);
            gtk_style_context_add_class(context, "sudoku-cell");
            
            if (i % box_size == 0) {
                gtk_style_context_add_class(context, "top-border");
            }
            if (j % box_size == 0) {
                gtk_style_context_add_class(context, "left-border");
            }

            gui.entries[i][j] = entry;
            gtk_grid_attach(GTK_GRID(gui.grid), entry, j, i, 1, 1);
        }
    }

    // Add the grid to the window
    GtkWidget *main_box = gtk_widget_get_parent(gui.size_combo);
    gtk_box_pack_start(GTK_BOX(main_box), gui.grid, TRUE, TRUE, 10);
    gtk_widget_show_all(gui.window);

    // Update the Sudoku struct
    gui.sudoku.size = new_size;
}

static void on_size_changed(GtkComboBox *widget, gpointer data) {
    const char *size_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(gui.size_combo));
    int new_size = atoi(size_text);
    update_grid_size(new_size);
}

static void clear_board() {
    for (int i = 0; i < gui.sudoku.size; i++) {
        for (int j = 0; j < gui.sudoku.size; j++) {
            gtk_entry_set_text(GTK_ENTRY(gui.entries[i][j]), "");
            gui.sudoku.board[i][j] = ' ';
        }
    }
    gtk_label_set_text(GTK_LABEL(gui.status_label), "Board cleared");
}

static void solve_clicked(GtkButton *button, gpointer data) {
    // Read values from entries into Sudoku structure
    for (int i = 0; i < gui.sudoku.size; i++) {
        for (int j = 0; j < gui.sudoku.size; j++) {
            const char *text = gtk_entry_get_text(GTK_ENTRY(gui.entries[i][j]));
            if (text[0] == '\0' || text[0] == ' ') {
                gui.sudoku.board[i][j] = ' ';
            } else {
                gui.sudoku.board[i][j] = text[0];
            }
        }
    }

    // Reset masks
    memset(rowMask, 0, sizeof(rowMask));
    memset(colMask, 0, sizeof(colMask));
    memset(boxMask, 0, sizeof(boxMask));

    // Validate and solve
    if (!validateInitialBoard(&gui.sudoku)) {
        gtk_label_set_text(GTK_LABEL(gui.status_label), "Invalid puzzle configuration");
        return;
    }

    int steps = 0;
    if (solveSudoku(&gui.sudoku, &steps)) {
        // Update GUI with solution
        for (int i = 0; i < gui.sudoku.size; i++) {
            for (int j = 0; j < gui.sudoku.size; j++) {
                char text[2] = {gui.sudoku.board[i][j], '\0'};
                gtk_entry_set_text(GTK_ENTRY(gui.entries[i][j]), text);
            }
        }
        char status[100];
        snprintf(status, sizeof(status), "Solved in %d steps!", steps);
        gtk_label_set_text(GTK_LABEL(gui.status_label), status);
    } else {
        gtk_label_set_text(GTK_LABEL(gui.status_label), "No solution exists");
    }
}

static void add_css_styles() {
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        ".sudoku-cell { padding: 5px; }"
        ".top-border { border-top: 2px solid black; }"
        ".left-border { border-left: 2px solid black; }"
        "entry { font-size: 16px; }", -1, NULL);
    
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    
    g_object_unref(provider);
}

void create_sudoku_window(void) {
    // Create main window
    gui.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(gui.window), "Sudoku Solver");
    gtk_container_set_border_width(GTK_CONTAINER(gui.window), 10);
    g_signal_connect(gui.window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create main vertical box
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(gui.window), main_box);

    // Create size selector
    GtkWidget *size_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *size_label = gtk_label_new("Grid Size:");
    gui.size_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(gui.size_combo), "4");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(gui.size_combo), "9");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(gui.size_combo), "16");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(gui.size_combo), "25");
    gtk_combo_box_set_active(GTK_COMBO_BOX(gui.size_combo), 1); // Default to 9x9
    
    gtk_box_pack_start(GTK_BOX(size_box), size_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(size_box), gui.size_combo, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(main_box), size_box, FALSE, FALSE, 5);

    // Create buttons
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gui.solve_button = gtk_button_new_with_label("Solve");
    gui.clear_button = gtk_button_new_with_label("Clear");
    g_signal_connect(gui.solve_button, "clicked", G_CALLBACK(solve_clicked), NULL);
    g_signal_connect(gui.clear_button, "clicked", G_CALLBACK(clear_board), NULL);
    g_signal_connect(gui.size_combo, "changed", G_CALLBACK(on_size_changed), NULL);
    
    gtk_box_pack_start(GTK_BOX(button_box), gui.solve_button, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(button_box), gui.clear_button, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(main_box), button_box, FALSE, FALSE, 5);

    // Create status label
    gui.status_label = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(main_box), gui.status_label, FALSE, FALSE, 5);

    // Add CSS styles
    add_css_styles();

    // Create initial grid (9x9 by default)
    update_grid_size(9);

    // Show all widgets
    gtk_widget_show_all(gui.window);
}