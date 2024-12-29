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
    scene_play(xmas_scene, canvas, 10);

    return 0;
}

void blink(SceneEvent* starting, Tree dark, Tree lit, int num_blinks, int avg_duration, int avg_delay);
SceneEvent* grow_tree(SceneEvent* curr_tree, Tree tree, int duration);
Tree new_tree_with_pixel(Tree new_tree, Pixel pixel);

SceneEvent* build_scene() {
    Pixel green_leaf = (Pixel){'.', GREEN};

    Tree tree0_dark = {"tree0", 23, 5, 4, green_leaf};
    Tree tree1_dark = {"tree1", 7, 7, 7, green_leaf};
    Tree tree2_dark = {"tree2", 22, 12, 6, green_leaf};
    Tree tree3_dark = {"tree3", 4, 14, 8, green_leaf};
    Tree tree4_dark = {"tree4", 14, 17, 10, green_leaf};
    Tree tree5_dark = {"tree5", 58, 6, 4, green_leaf};
    Tree tree6_dark = {"tree6", 50, 20, 20, green_leaf};
    Tree tree0_lit = {"tree0", 23, 4, 4, (Pixel){'+', BLUE}};
    Tree tree1_lit = new_tree_with_pixel(tree1_dark, (Pixel){'*', MAGENTA});
    Tree tree2_lit = new_tree_with_pixel(tree2_dark, (Pixel){'@', CYAN});
    Tree tree3_lit = new_tree_with_pixel(tree3_dark, (Pixel){'*', BRIGHT_WHITE});
    Tree tree4_lit = new_tree_with_pixel(tree4_dark, (Pixel){'^', BRIGHT_RED});
    Tree tree5_lit = new_tree_with_pixel(tree5_dark, (Pixel){'$', BRIGHT_YELLOW});
    Tree tree6_lit = new_tree_with_pixel(tree6_dark, (Pixel){'*', BRIGHT_WHITE});

    SceneEvent* xmas_scene = start_with(tree0_dark);

    // Setup
    grow_tree(xmas_scene, tree1_dark, 6000);
    grow_tree(xmas_scene, tree5_dark, 9000);
    grow_tree(xmas_scene, tree2_dark, 8000);
    grow_tree(xmas_scene, tree3_dark, 5000);
    SceneEvent* tree6_adult = grow_tree(xmas_scene, tree6_dark, 10000);
    grow_tree(xmas_scene, tree4_dark, 6000);

    blink(tree6_adult, tree0_dark, tree0_lit, 1000, 200, 1000);
    blink(tree6_adult, tree1_dark, tree1_lit, 1000, 500, 2000);
    blink(tree6_adult, tree2_dark, tree2_lit, 1000, 600, 2000);
    blink(tree6_adult, tree3_dark, tree3_lit, 1000, 700, 2000);
    blink(tree6_adult, tree4_dark, tree4_lit, 1000, 800, 2000);
    blink(tree6_adult, tree5_dark, tree5_lit, 1000, 1000, 4000);
    blink(tree6_adult, tree6_dark, tree6_lit, 1000, 400, 3000);

    return xmas_scene;
}

// ==============================
// = Xmas scene building functions
// ===

// Makes a copy of Tree using a different fill pixel.
Tree new_tree_with_pixel(Tree new_tree, Pixel pixel) {
    new_tree.fill = pixel;
    return new_tree;
}

// Adds Events to the Scene that causes the tree to grow.
SceneEvent* grow_tree(SceneEvent* starting, Tree tree, int duration) {
    int frames = duration / 100;
    float height_increment = (float)tree.height / frames;

    SceneEvent* curr = starting;
    Tree growing_tree = tree;
    float growing_height = 1.0;
    for (int i = 0; i < frames; i++) {
        growing_height += height_increment;
        growing_tree.height = growing_height;
        curr = after(curr, growing_tree, 100);
    }

    return curr;
}

int rand_around(int avg, int factor);
// Adds Events to the Scene that causes the tree to blink on and off.
void blink(SceneEvent* starting, Tree dark, Tree lit, int num_blinks, int avg_duration, int avg_delay) {
    SceneEvent* curr = starting;
    for (int i = 0; i < num_blinks; i++) {
        int duration = rand_around(avg_duration, 2);
        int delay = rand_around(avg_delay, 2);

        curr = after(curr, lit, delay);
        curr = after(curr, dark, duration);
    }
}

// Generates a random number around `avg` with a standard deviation of `avg / factor`.
int rand_around(int avg, int factor) {
    return avg + rand() % (2 * avg / factor) - avg / factor;
}
