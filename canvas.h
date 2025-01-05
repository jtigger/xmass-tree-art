#ifndef CANVAS_H
#define CANVAS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CANVAS_HEIGHT 50
#define CANVAS_WIDTH 200

// ANSI color codes as integers
#define RED     31
#define GREEN   32
#define YELLOW  33
#define BLUE    34
#define MAGENTA 35
#define CYAN    36
#define WHITE   37
#define BRIGHT_RED     91
#define BRIGHT_GREEN   92
#define BRIGHT_YELLOW  93
#define BRIGHT_BLUE    94
#define BRIGHT_MAGENTA 95
#define BRIGHT_CYAN    96
#define BRIGHT_WHITE   97


// The smallest unit of display on the canvas: a character with a color.
typedef struct {
    char glyph;  // character to display
    int color;   // ANSI color code
} Pixel;

// Descriptor of a tree that can be added to a canvas.
typedef struct {
    char* id;    // unique identifier for the tree
    int x, y;    // position of the left-side of the trunk of the tree on the canvas
    int height;  // height of the leafy part of the tree
    Pixel fill;  // character to use for the tree's body/leaves
} Tree;

// A list of `Tree`s that have been added to the canvas, in order of appearance.
typedef struct CanvasLayer CanvasLayer;
struct CanvasLayer
{
    Tree* tree;
    CanvasLayer* next;
};


// ==============================
// = Canvas functions
// ===  

// Prints the canvas to the terminal, pixel by pixel.
void canvas_print(Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH]);

// Clears the canvas by setting all pixels to the default color and blank space.
void canvas_wipe(Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH]);

// Writes `pixel` to the canvas at `rel_row` and `rel_col`.
// Writing out-of-bounds is a no-op.
void canvas_write(Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH], int rel_row, int rel_col, Pixel pixel);

// Writes a `tree` to the canvas placing the top-left of the tree's trunk at `tree.x` and `tree.y`.
void canvas_write_tree(Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH], Tree tree);

// Positions the terminal cursor to the top left of the canvas
// Assumes that the cursor is at the bottom left of the canvas.
void reset_cursor();


// ==============================
// = CanvasLayer functions
// ===

// Finds the `Tree` (by id) in `layers` and replaces it with `tree`. 
// If `tree` is not found, appends a new `CanvasLayer` with `tree`.
CanvasLayer* add_or_update_tree(CanvasLayer* trees, Tree *tree);

// Creates a new `CanvasLayer` containing `tree`.
CanvasLayer* new_layer(Tree *tree);

// Writes each `CanvasLayer` — in order — onto `canvas`
void write_layers_on_canvas(Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH], CanvasLayer* trees);

#endif // CANVAS_H 