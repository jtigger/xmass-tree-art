#include <stdio.h>
#include <unistd.h>

#define CANVAS_HEIGHT 20
#define CANVAS_WIDTH 60

// Struct to represent a tree.
typedef struct
{
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

int main()
{
    char canvas[CANVAS_HEIGHT][CANVAS_WIDTH];

    Tree tree1 = {0, 0, 7, '.'};
    Tree tree2 = {11, 2, 6, '.'};
    Tree tree3 = {3, 7, 10, '.'};

    FutureTree place_third_tree = {tree3, 2000, NULL};
    FutureTree place_second_tree = {tree2, 1000, &place_third_tree};
    FutureTree the_beginning = {tree1, 0, &place_second_tree};

    printf("Merry Christmas!\n\n");

    canvas_wipe(canvas);
    realize(canvas, &the_beginning);

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

void realize(char canvas[CANVAS_HEIGHT][CANVAS_WIDTH], FutureTree* future_tree)
{
    int now = 0;

    while (future_tree != NULL)
    {
        if (now >= future_tree->when)
        {
            canvas_write_tree(canvas, future_tree->desired);
            canvas_print(canvas);
            reset_cursor();
            future_tree = future_tree->next;
        }
        else
        {
            usleep(500000);
            now += 500;
        }
    }
}

