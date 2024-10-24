CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wall -g -I./src/core
LDFLAGS = `pkg-config --libs gtk+-3.0` -lm

# Source files from different directories
CORE_SOURCES = src/core/sudoku.c
GUI_SOURCES = src/gui/sudoku_gui.c src/gui/main.c
SOURCES = $(CORE_SOURCES) $(GUI_SOURCES)

# Object files
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = sudoku_solver

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
	rm -f src/core/*.o src/gui/*.o