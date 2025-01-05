#include "canvas.h"
#include "scene.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// Idiomatic way to get the size of an array.
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

// build_scene() constructs the Christmas tree scene.
SceneEvent* build_scene();

int main()
{
    int seed = time(NULL);
    srand(seed);

    Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH];
    SceneEvent* xmas_scene = build_scene();

    printf("Merry Christmas!\n\n");
    scene_play(xmas_scene, canvas, 30);

    return 0;
}

void blink(SceneEvent* starting, Tree dark, Tree lit, int num_blinks, int avg_duration, int avg_delay);
SceneEvent* grow_tree(SceneEvent* curr_tree, Tree tree, int duration);
Tree new_tree_with_pixel(Tree new_tree, Pixel pixel);

SceneEvent* build_scene() {
    int num_trees = 7 + rand() % 100;
    Pixel green_leaf = (Pixel){'.', GREEN};
    char lights[6] = {'+', '@', '*', '^', '$', '='};
    int light_colors[6] = {WHITE, BLUE, CYAN, MAGENTA, BRIGHT_RED, BRIGHT_YELLOW};

    Tree trees_dark[num_trees];
    Tree trees_lit[num_trees];

    int biggest_y = 1;
    int max_y_increment = 2 + CANVAS_HEIGHT / num_trees;
    for (int i = 0; i < num_trees; i++) {
        char *id = malloc(10);
        sprintf(id, "tree%d", i);

        int height = 4 + rand() % 8;
        int x = rand() % (CANVAS_WIDTH);
        int y = biggest_y = biggest_y + (rand() % max_y_increment);
        char light = lights[rand() % ARRAY_SIZE(light_colors)];
        int light_color = light_colors[rand() % (ARRAY_SIZE(light_colors) + 1)];

        trees_dark[i] = (Tree){.id = id, .x = x, .y = y, .height = height, .fill = green_leaf};
        trees_lit[i] = new_tree_with_pixel(trees_dark[i], (Pixel){light, light_color});
    }

    SceneEvent* xmas_scene = start_with(trees_dark[0]);

    // Setup
    SceneEvent* last_tree_grown;
    for (int i = 1; i < num_trees; i++) {
        int grow_duration = 5000 + rand() % 5000;
        last_tree_grown = grow_tree(xmas_scene, trees_dark[i], grow_duration);
    }

    for (int i = 0; i < num_trees; i++) {
        int blink_duration = 200 + rand() % 8000;
        int blink_delay = 1000 + rand() % 3000;

        blink(last_tree_grown, trees_dark[i], trees_lit[i], 1000, blink_duration, blink_delay);
    }

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
