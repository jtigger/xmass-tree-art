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

typedef struct
{
    char glyph;  // character to display
    int color;   // ANSI color code
} Pixel;


// Struct to represent a tree.
typedef struct
{
    char* id;         // unique identifier for the tree
    int x, y;         // position of the left-side of the trunk of the tree on the canvas
    int height;       // height of the leafy part of the tree
    Pixel leaf_char;  // character to use for the leaves
} Tree;

typedef struct FutureTree FutureTree;
struct FutureTree
{
    Tree desired;     // the to-be state of the tree
    int when;         // when (in ms since the start) this state should be applied to the canvas
    FutureTree* next; // the next state to be applied to the canvas
};


// ==============
// = Canvas
// ==============

// Print the canvas to the console.
void canvas_print(Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH]);

// Set the canvas with spaces.
void canvas_wipe(Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH]);

// Write a character to the canvas at the given position, if it is within the canvas bounds.
void canvas_write(Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH], int rel_row, int rel_col, Pixel pixel);

// Write a tree to the canvas at the given height.
void canvas_write_tree(Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH], Tree tree);

// Reposition the cursor to the top left corner of the canvas.
void reset_cursor();

// ==============
// = Realizer
// ==============

// Play the sequence of future trees at the appropriate moments.
void realize(Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH], FutureTree* future_tree);

FutureTree* after(FutureTree* future_tree, int delay, Tree tree);
FutureTree* plan_at(FutureTree* curr_tree, int moment, Tree tree);
FutureTree* start_with(Tree tree);

// ==============
// = Plan authoring "macros"
// ==============

void blink(FutureTree* starting, Tree dark, Tree lit, int num_blinks, int avg_duration, int avg_delay) {
    FutureTree* curr = starting;
    for (int i = 0; i < num_blinks; i++) {
        int duration = avg_duration + rand() % (2 * avg_duration / 2) - avg_duration / 2;
        int delay = avg_delay + rand() % (2 * avg_delay / 2) - avg_delay / 2;

        curr = after(curr, delay, lit);
        curr = after(curr, duration, dark);
    }
}

FutureTree* grow(FutureTree* curr_tree, Tree tree, int duration) {
    int frames = duration / 100;
    float height_increment = (float)tree.height / frames;

    Tree growing_tree = tree;
    float growing_height = 1.0;
    for (int i = 0; i < frames; i++) {
        growing_height += height_increment;
        growing_tree.height = growing_height;
        curr_tree = after(curr_tree, 100, growing_tree);
    }

    return curr_tree;
}

int main()
{
    Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH];

    Tree tree0 = {"tree0", 23, 5, 4, {'.', GREEN}};
    Tree tree1 = {"tree1", 7, 7, 7, {'.', GREEN}};
    Tree tree2 = {"tree2", 22, 12, 6, {'.', GREEN}};
    Tree tree3 = {"tree3", 4, 14, 8, {'.', GREEN}};
    Tree tree4 = {"tree4", 14, 17, 10, {'.', GREEN}};
    Tree tree5 = {"tree5", 58, 6, 4, {'.', GREEN}};
    Tree tree6 = {"tree6", 50, 20, 20, {'.', GREEN}};
    Tree tree0_lit = {"tree0", 23, 5, 4, {'+', BLUE}};
    Tree tree1_lit = {"tree1", 7, 7, 7, {'*', MAGENTA}};
    Tree tree2_lit = {"tree2", 22, 12, 6, {'@', CYAN}};
    Tree tree3_lit = {"tree3", 4, 14, 8, {'*', BRIGHT_WHITE}};
    Tree tree4_lit = {"tree4", 14, 17, 10, {'^', BRIGHT_RED}};
    Tree tree5_lit = {"tree5", 58, 6, 4, {'$', BRIGHT_YELLOW}};
    Tree tree6_lit = {"tree6", 50, 20, 20, {'*', BRIGHT_WHITE}};

    FutureTree* the_beginning = start_with(tree0);
    FutureTree* prev = the_beginning;

    // Setup
    FutureTree* tree1_adult = grow(the_beginning, tree1, 1000);
    FutureTree* tree5_adult = grow(the_beginning, tree5, 9000);
    FutureTree* tree2_adult = grow(the_beginning, tree2, 3000);
    FutureTree* tree3_adult = grow(the_beginning, tree3, 5000);
    FutureTree* tree6_adult = grow(the_beginning, tree6, 10000);
    FutureTree* tree4_adult = grow(the_beginning, tree4, 6000);

    blink(the_beginning, tree0, tree0_lit, 1000, 200, 1000);
    blink(tree1_adult, tree1, tree1_lit, 1000, 500, 2000);
    blink(tree2_adult, tree2, tree2_lit, 1000, 600, 2000);
    blink(tree3_adult, tree3, tree3_lit, 1000, 700, 2000);
    blink(tree4_adult, tree4, tree4_lit, 1000, 800, 2000);
    blink(tree5_adult, tree5, tree5_lit, 1000, 1000, 4000);
    blink(tree6_adult, tree6, tree6_lit, 1000, 400, 3000);

    printf("Merry Christmas!\n\n");

    realize(canvas, the_beginning);

    return 0;
}

void canvas_print(Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH])
{
    for (int i = 0; i < CANVAS_HEIGHT; i++)
    {
        for (int j = 0; j < CANVAS_WIDTH; j++)
        {
            printf("\033[%dm%c\033[0m", canvas[i][j].color, canvas[i][j].glyph);
        }
        printf("\n");
    }
}

void reset_cursor()
{
    // ANSI escape sequence: move cursor CANVAS_HEIGHT lines up.
    printf("\033[%dA", CANVAS_HEIGHT);
}

void canvas_wipe(Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH])
{
    for (int i = 0; i < CANVAS_HEIGHT; i++)
    {
        for (int j = 0; j < CANVAS_WIDTH; j++)
        {
            canvas[i][j] = (Pixel){' ', 0};
        }
    }
}

void canvas_write(Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH], int rel_row, int rel_col, Pixel pixel)
{
    if (rel_row >= 0 && rel_row < CANVAS_HEIGHT && rel_col >= 0 && rel_col < CANVAS_WIDTH)
    {
        canvas[rel_row][rel_col] = pixel;
    }
}

void canvas_write_tree(Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH], Tree tree)
{
    int width = 2 * tree.height - 1;
    // tree.x, tree.y is top-left of the trunk (to ensure the trunk is stable as tree grows)
    // x, y is the top-left corner of the painting of the tree
    int x = tree.x - width / 2;
    int y = tree.y - tree.height + 1;

    int rel_row = 0;

    for (rel_row = 0; rel_row < tree.height; rel_row++)
    {
        int tree_pixels = 2 * rel_row + 1;
        int rel_col = tree.height - rel_row - 1;
        for (int j = 0; j < tree_pixels; j++, rel_col++)
        {
            canvas_write(canvas, rel_row + y, rel_col + x, tree.leaf_char);
        }
    }

    int start_of_trunk = (width - 1) / 2;
    int thickness_of_trunk = tree.height / 5;
    int end_of_trunk = start_of_trunk + thickness_of_trunk;

    int rel_col = start_of_trunk - 1;

    canvas_write(canvas, rel_row + y, rel_col + x, (Pixel){'|', YELLOW});
    rel_col++;

    for (int i = start_of_trunk; i < end_of_trunk; i++, rel_col++)
    {
        canvas_write(canvas, rel_row + y, rel_col + x, (Pixel){' ', YELLOW});
    }
    canvas_write(canvas, rel_row + y, rel_col + x, (Pixel){'|', YELLOW});
    rel_col++;
    rel_row++;

    rel_col = start_of_trunk - 1 - 1;
    canvas_write(canvas, rel_row + y, rel_col + x, (Pixel){'[', RED});
    rel_col++;
    for (int i = start_of_trunk - 1; i < end_of_trunk + 1; i++, rel_col++)
    {
        canvas_write(canvas, rel_row + y, rel_col + x, (Pixel){'_', RED});
    }
    canvas_write(canvas, rel_row + y, rel_col + x, (Pixel){']', RED});
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

void write_trees_on_canvas(Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH], TreeOnCanvas* trees) {
    TreeOnCanvas* curr = trees;
    while (curr != NULL) {
        canvas_write_tree(canvas, *curr->tree);
        curr = curr->next;
    }
}

void realize(Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH], FutureTree* future_tree)
{
    int now = 0; // time in ms since the start
    TreeOnCanvas* trees = NULL;

    while (future_tree != NULL)
    {
        if (now >= future_tree->when)
        {
            while (future_tree != NULL && now >= future_tree->when) {
                trees = add_or_update_tree(trees, &future_tree->desired);
                future_tree = future_tree->next;
            }
            canvas_wipe(canvas);
            write_trees_on_canvas(canvas, trees);
            canvas_print(canvas);
            reset_cursor();
        }
        usleep(50000); // sleep for 50ms (takes macroseconds)
        now += 50;
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
        return next_tree;
    }

    return plan_at(curr_tree->next, moment, tree);
}

FutureTree* after(FutureTree* curr_tree, int delay, Tree tree)
{
    int moment = curr_tree->when + delay;
    return plan_at(curr_tree, moment, tree);
}