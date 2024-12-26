#include <stdio.h>
#include <unistd.h>

#define CANVAS_HEIGHT 20
#define CANVAS_WIDTH 60

// Struct to represent a tree.
typedef struct
{
    int x;
    int y;
    int height;
    char leaf_char;
} Tree;


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


int main()
{
    char canvas[CANVAS_HEIGHT][CANVAS_WIDTH];

    Tree tree1 = {0, 0, 7, '.'};
    Tree tree2 = {11, 2, 6, '.'};
    Tree tree3 = {3, 7, 10, '.'};

    printf("Merry Christmas!\n\n");

    canvas_wipe(canvas);
    canvas_write_tree(canvas, tree1);
    canvas_write_tree(canvas, tree2);
    canvas_write_tree(canvas, tree3);
    canvas_print(canvas);
    reset_cursor();
    canvas_print(canvas);

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