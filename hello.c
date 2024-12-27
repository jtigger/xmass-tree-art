#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CANVAS_HEIGHT 25
#define CANVAS_WIDTH 60

// Struct to represent a tree.
typedef struct
{
    char* id;        // unique identifier for the tree
    int x, y;        // position of the tree on the canvas
    int height;      // height of the leafy part of the tree
    char leaf_char;  // character to use for the leaves
} Tree;

typedef struct FutureTree FutureTree;
struct FutureTree
{
    Tree desired;            // the to-be state of the tree
    int when;                // when (in ms since the start) this state should be applied to the canvas
    FutureTree* next; // the next state to be applied to the canvas
};


// ==============
// = Canvas
// ==============

// Print the canvas to the console.
void canvas_print(char canvas[CANVAS_HEIGHT][CANVAS_WIDTH]);

// Set the canvas with spaces.
void canvas_wipe(char canvas[CANVAS_HEIGHT][CANVAS_WIDTH]);

// Write a character to the canvas at the given position, if it is within the canvas bounds.
void canvas_write(char canvas[CANVAS_HEIGHT][CANVAS_WIDTH], int rel_row, int rel_col, char c);

// Write a tree to the canvas at the given height.
void canvas_write_tree(char canvas[CANVAS_HEIGHT][CANVAS_WIDTH], Tree tree);

// Reposition the cursor to the top left corner of the canvas.
void reset_cursor();

// ==============
// = Realizer
// ==============

// Play the sequence of future trees at the appropriate moments.
void realize(char canvas[CANVAS_HEIGHT][CANVAS_WIDTH], FutureTree* future_tree);

FutureTree* start_with(Tree tree);
FutureTree* after(FutureTree* future_tree, int delay, Tree tree);

int main()
{
    char canvas[CANVAS_HEIGHT][CANVAS_WIDTH];

    Tree tree1 = {"tree1", 0, 0, 7, '.'};
    Tree tree2 = {"tree2", 11, 2, 6, '.'};
    Tree tree3 = {"tree3", -4, 6, 8, '.'};
    Tree tree4 = {"tree4", 3, 7, 10, '.'};
    Tree tree5 = {"tree5", 30, 0, 20, '.'};
    Tree tree1_lit = {"tree1", 0, 0, 7, '*'};
    Tree tree2_lit = {"tree2", 11, 2, 6, '*'};
    Tree tree3_lit = {"tree3", -4, 6, 8, '*'};
    Tree tree4_lit = {"tree4", 3, 7, 10, '*'};
    Tree tree5_lit = {"tree5", 30, 0, 20, '*'};

    FutureTree* the_beginning = start_with(tree1);
    FutureTree* then_next;

    then_next = after(the_beginning, 1000, tree2);
    then_next = after(then_next, 1000, tree3);
    then_next = after(then_next, 1000, tree4);
    then_next = after(then_next, 1000, tree5);
    then_next = after(then_next, 2000, tree4_lit);
    then_next = after(then_next, 250, tree4);
    then_next = after(then_next, 2500, tree4_lit);
    then_next = after(then_next, 250, tree4);
    then_next = after(then_next, 3000, tree4_lit);
    then_next = after(then_next, 250, tree4);

    printf("Merry Christmas!\n\n");

    realize(canvas, the_beginning);

    return 0;
}

void canvas_print(char canvas[CANVAS_HEIGHT][CANVAS_WIDTH])
{
    for (int i = 0; i < CANVAS_HEIGHT; i++)
    {
        for (int j = 0; j < CANVAS_WIDTH; j++)
        {
            printf("%c", canvas[i][j]);
        }
        printf("\n");
    }
}

void reset_cursor()
{
    // ANSI escape sequence: move cursor CANVAS_HEIGHT lines up.
    printf("\033[%dA", CANVAS_HEIGHT);
}

void canvas_wipe(char canvas[CANVAS_HEIGHT][CANVAS_WIDTH])
{
    for (int i = 0; i < CANVAS_HEIGHT; i++)
    {
        for (int j = 0; j < CANVAS_WIDTH; j++)
        {
            canvas[i][j] = ' ';
        }
    }
}

void canvas_write(char canvas[CANVAS_HEIGHT][CANVAS_WIDTH], int rel_row, int rel_col, char c)
{
    if (rel_row >= 0 && rel_row < CANVAS_HEIGHT && rel_col >= 0 && rel_col < CANVAS_WIDTH)
    {
        canvas[rel_row][rel_col] = c;
    }
}

void canvas_write_tree(char canvas[CANVAS_HEIGHT][CANVAS_WIDTH], Tree tree)
{
    int width = 2 * tree.height - 1;

    int rel_row = 0;

    for (rel_row = 0; rel_row < tree.height; rel_row++)
    {
        int tree_pixels = 2 * rel_row + 1;
        int rel_col = tree.height - rel_row - 1;
        for (int j = 0; j < tree_pixels; j++, rel_col++)
        {
            canvas_write(canvas, rel_row + tree.y, rel_col + tree.x, tree.leaf_char);
        }
    }

    int start_of_trunk = (width - 1) / 2;
    int thickness_of_trunk = 1;
    int end_of_trunk = start_of_trunk + thickness_of_trunk;

    int rel_col = start_of_trunk - 1;

    canvas_write(canvas, rel_row + tree.y, rel_col + tree.x, '|');
    rel_col++;

    for (int i = start_of_trunk; i < end_of_trunk; i++, rel_col++)
    {
        canvas_write(canvas, rel_row + tree.y, rel_col + tree.x, ' ');
    }
    canvas_write(canvas, rel_row + tree.y, rel_col + tree.x, '|');
    rel_col++;
    rel_row++;

    rel_col = start_of_trunk - 1 - 1;
    canvas_write(canvas, rel_row + tree.y, rel_col + tree.x, '[');
    rel_col++;
    for (int i = start_of_trunk - 1; i < end_of_trunk + 1; i++, rel_col++)
    {
        canvas_write(canvas, rel_row + tree.y, rel_col + tree.x, '_');
    }
    canvas_write(canvas, rel_row + tree.y, rel_col + tree.x, ']');
    rel_col++;
}


typedef struct TreeOnCanvas TreeOnCanvas;
struct TreeOnCanvas
{
    Tree* tree;
    TreeOnCanvas* next;
};

TreeOnCanvas* place(Tree *tree) {
    TreeOnCanvas* tree_on_canvas = (TreeOnCanvas*)malloc(sizeof(TreeOnCanvas));
    tree_on_canvas->tree = tree;
    tree_on_canvas->next = NULL;
    return tree_on_canvas;
}

TreeOnCanvas* add_or_update_tree(TreeOnCanvas* trees, Tree *tree) {
    if (trees == NULL) {
        return place(tree);
    }

    TreeOnCanvas* curr = trees;

    bool found = false;
    TreeOnCanvas* prev = curr;
    while (curr != NULL && !found) {
        if (strcmp(curr->tree->id, tree->id) == 0) {
            curr->tree = tree;
            found = true;
        }
        prev = curr;
        curr = curr->next;
    }

    if (!found) {
        prev->next = place(tree);
    }

    return trees;
}

void write_trees_on_canvas(char canvas[CANVAS_HEIGHT][CANVAS_WIDTH], TreeOnCanvas* trees) {
    TreeOnCanvas* curr = trees;
    while (curr != NULL) {
        canvas_write_tree(canvas, *curr->tree);
        curr = curr->next;
    }
}

void realize(char canvas[CANVAS_HEIGHT][CANVAS_WIDTH], FutureTree* future_tree)
{
    int now = 0; // time in ms since the start
    TreeOnCanvas* trees = NULL;

    while (future_tree != NULL)
    {
        if (now >= future_tree->when)
        {
            trees = add_or_update_tree(trees, &future_tree->desired);
            canvas_wipe(canvas);
            write_trees_on_canvas(canvas, trees);
            canvas_print(canvas);
            reset_cursor();
            future_tree = future_tree->next;
        }
        else
        {
            usleep(100000); // sleep for 100ms (takes macroseconds)
            now += 100;
        }
    }
}

FutureTree* from(Tree tree, int delay)
{
    FutureTree* first_tree = (FutureTree*)malloc(sizeof(FutureTree));
    first_tree->desired = tree;
    first_tree->when = delay;
    first_tree->next = NULL;
    return first_tree;
}

FutureTree* start_with(Tree tree)
{
    return from(tree, 0);
}
FutureTree* plan_at(FutureTree* curr_tree, int moment, Tree tree) {
    if (curr_tree->next == NULL) {
        curr_tree->next = from(tree, moment);
        return curr_tree->next;
    }

    if (moment < curr_tree->next->when) {
        FutureTree* next_tree = from(tree, moment);
        next_tree->next = curr_tree->next;
        curr_tree->next = next_tree;
        return curr_tree;
    }

    return plan_at(curr_tree->next, moment, tree);
}

FutureTree* after(FutureTree* curr_tree, int delay, Tree tree)
{
    int moment = curr_tree->when + delay;
    return plan_at(curr_tree, moment, tree);
}