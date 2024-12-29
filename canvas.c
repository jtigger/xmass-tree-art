#include "canvas.h"

// ==============================
// = Canvas functions
// ===  

void canvas_print(Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH]) {
    for (int i = 0; i < CANVAS_HEIGHT; i++) {
        for (int j = 0; j < CANVAS_WIDTH; j++) {
            printf("\033[%dm%c\033[0m", canvas[i][j].color, canvas[i][j].glyph);
        }
        printf("\n");
    }
}

void canvas_wipe(Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH]) {
    for (int i = 0; i < CANVAS_HEIGHT; i++) {
        for (int j = 0; j < CANVAS_WIDTH; j++) {
            canvas[i][j] = (Pixel){' ', 0};
        }
    }
}

void canvas_write(Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH], int rel_row, int rel_col, Pixel pixel) {
    if (rel_row >= 0 && rel_row < CANVAS_HEIGHT && rel_col >= 0 && rel_col < CANVAS_WIDTH) {
        canvas[rel_row][rel_col] = pixel;
    }
}

void canvas_write_tree(Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH], Tree tree) {
    int width = 2 * tree.height - 1;
    int x = tree.x - width / 2;
    int y = tree.y - tree.height + 1;

    int rel_row = 0;

    for (rel_row = 0; rel_row < tree.height; rel_row++) {
        int tree_pixels = 2 * rel_row + 1;
        int rel_col = tree.height - rel_row - 1;
        for (int j = 0; j < tree_pixels; j++, rel_col++) {
            canvas_write(canvas, rel_row + y, rel_col + x, tree.fill);
        }
    }

    int start_of_trunk = (width - 1) / 2;
    int thickness_of_trunk = tree.height / 5;
    int end_of_trunk = start_of_trunk + thickness_of_trunk;

    int rel_col = start_of_trunk - 1;

    canvas_write(canvas, rel_row + y, rel_col + x, (Pixel){'|', YELLOW});
    rel_col++;

    for (int i = start_of_trunk; i < end_of_trunk; i++, rel_col++) {
        canvas_write(canvas, rel_row + y, rel_col + x, (Pixel){' ', YELLOW});
    }
    canvas_write(canvas, rel_row + y, rel_col + x, (Pixel){'|', YELLOW});
    rel_col++;
    rel_row++;

    rel_col = start_of_trunk - 1 - 1;
    canvas_write(canvas, rel_row + y, rel_col + x, (Pixel){'[', RED});
    rel_col++;
    for (int i = start_of_trunk - 1; i < end_of_trunk + 1; i++, rel_col++) {
        canvas_write(canvas, rel_row + y, rel_col + x, (Pixel){'_', RED});
    }
    canvas_write(canvas, rel_row + y, rel_col + x, (Pixel){']', RED});
    rel_col++;
} 

void reset_cursor() {
    printf("\033[%dA", CANVAS_HEIGHT);
}


// ==============================
// = CanvasLayer functions
// ===

CanvasLayer* add_or_update_tree(CanvasLayer* layers, Tree *tree) {
    if (layers == NULL) {
        return new_layer(tree);
    }

    CanvasLayer* curr = layers;

    bool found = false;
    CanvasLayer* prev = curr;
    while (curr != NULL && !found) {
        if (strcmp(curr->tree->id, tree->id) == 0) {
            curr->tree = tree;
            found = true;
        }
        prev = curr;
        curr = curr->next;
    }

    if (!found) {
        prev->next = new_layer(tree);
    }

    return layers;
}


CanvasLayer* new_layer(Tree *tree) {
    CanvasLayer* tree_on_canvas = (CanvasLayer*)malloc(sizeof(CanvasLayer));
    tree_on_canvas->tree = tree;
    tree_on_canvas->next = NULL;
    return tree_on_canvas;
}

void write_layers_on_canvas(Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH], CanvasLayer* layers) {
    CanvasLayer* curr = layers;
    while (curr != NULL) {
        canvas_write_tree(canvas, *curr->tree);
        curr = curr->next;
    }
}
