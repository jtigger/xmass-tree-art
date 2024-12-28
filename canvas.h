#ifndef CANVAS_H
#define CANVAS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CANVAS_HEIGHT 25
#define CANVAS_WIDTH 60

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


typedef struct {
    char glyph;  // character to display
    int color;   // ANSI color code
} Pixel;

typedef struct {
    char* id;         // unique identifier for the tree
    int x, y;         // position of the left-side of the trunk of the tree on the canvas
    int height;       // height of the leafy part of the tree
    Pixel leaf_char;  // character to use for the leaves
} Tree;

typedef struct CanvasLayer CanvasLayer;
struct CanvasLayer
{
    Tree* tree;
    CanvasLayer* next;
};


// ==============================
// = Canvas functions
// ===  


void canvas_print(Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH]);
void canvas_wipe(Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH]);
void canvas_write(Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH], int rel_row, int rel_col, Pixel pixel);
void canvas_write_tree(Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH], Tree tree);


// ==============================
// = CanvasLayer functions
// ===

CanvasLayer* add_or_update_tree(CanvasLayer* trees, Tree *tree);
CanvasLayer* new_layer(Tree *tree);
void write_layers_on_canvas(Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH], CanvasLayer* trees);


#endif // CANVAS_H 