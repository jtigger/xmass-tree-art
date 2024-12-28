#include "canvas.h"
#include "scene.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// build_scene() constructs the Christmas tree scene.
SceneEvent* build_scene();

int main()
{
    Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH];
    SceneEvent* xmas_scene = build_scene();

    printf("Merry Christmas!\n\n");
    realize(canvas, xmas_scene, 10);

    return 0;
}

void blink(SceneEvent* starting, Tree dark, Tree lit, int num_blinks, int avg_duration, int avg_delay);
SceneEvent* grow(SceneEvent* curr_tree, Tree tree, int duration);
Tree new_tree_with_pixel(Tree new_tree, Pixel pixel);

SceneEvent* build_scene() {
    Pixel green_leaf = (Pixel){'.', GREEN};

    Tree tree0 = {"tree0", 23, 5, 4, green_leaf};
    Tree tree1 = {"tree1", 7, 7, 7, green_leaf};
    Tree tree2 = {"tree2", 22, 12, 6, green_leaf};
    Tree tree3 = {"tree3", 4, 14, 8, green_leaf};
    Tree tree4 = {"tree4", 14, 17, 10, green_leaf};
    Tree tree5 = {"tree5", 58, 6, 4, green_leaf};
    Tree tree6 = {"tree6", 50, 20, 20, green_leaf};
    Tree tree0_lit = new_tree_with_pixel(tree0, (Pixel){'+', BLUE});
    Tree tree1_lit = new_tree_with_pixel(tree1, (Pixel){'*', MAGENTA});
    Tree tree2_lit = new_tree_with_pixel(tree2, (Pixel){'@', CYAN});
    Tree tree3_lit = new_tree_with_pixel(tree3, (Pixel){'*', BRIGHT_WHITE});
    Tree tree4_lit = new_tree_with_pixel(tree4, (Pixel){'^', BRIGHT_RED});
    Tree tree5_lit = new_tree_with_pixel(tree5, (Pixel){'$', BRIGHT_YELLOW});
    Tree tree6_lit = new_tree_with_pixel(tree6, (Pixel){'*', BRIGHT_WHITE});

    SceneEvent* the_beginning = start_with(tree0);

    // Setup
    SceneEvent* tree1_adult = grow(the_beginning, tree1, 1000);
    SceneEvent* tree5_adult = grow(the_beginning, tree5, 9000);
    SceneEvent* tree2_adult = grow(the_beginning, tree2, 3000);
    SceneEvent* tree3_adult = grow(the_beginning, tree3, 5000);
    SceneEvent* tree6_adult = grow(the_beginning, tree6, 10000);
    SceneEvent* tree4_adult = grow(the_beginning, tree4, 6000);

    blink(the_beginning, tree0, tree0_lit, 1000, 200, 1000);
    blink(tree1_adult, tree1, tree1_lit, 1000, 500, 2000);
    blink(tree2_adult, tree2, tree2_lit, 1000, 600, 2000);
    blink(tree3_adult, tree3, tree3_lit, 1000, 700, 2000);
    blink(tree4_adult, tree4, tree4_lit, 1000, 800, 2000);
    blink(tree5_adult, tree5, tree5_lit, 1000, 1000, 4000);
    blink(tree6_adult, tree6, tree6_lit, 1000, 400, 3000);

    return the_beginning;
}

// Adds Events to the Scene that causes the tree to blink on and off.
void blink(SceneEvent* starting, Tree dark, Tree lit, int num_blinks, int avg_duration, int avg_delay) {
    SceneEvent* curr = starting;
    for (int i = 0; i < num_blinks; i++) {
        int duration = avg_duration + rand() % (2 * avg_duration / 2) - avg_duration / 2;
        int delay = avg_delay + rand() % (2 * avg_delay / 2) - avg_delay / 2;

        curr = after(curr, delay, lit);
        curr = after(curr, duration, dark);
    }
}

// Adds Events to the Scene that causes the tree to grow.
SceneEvent* grow(SceneEvent* curr_tree, Tree tree, int duration) {
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

// Makes a copy of Tree with a different pixel.
Tree new_tree_with_pixel(Tree new_tree, Pixel pixel) {
    Tree tree = new_tree;
    tree.leaf_char = pixel;
    return tree;
}
