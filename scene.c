
#include "scene.h"
#include "canvas.h"

SceneEvent* from(Tree tree, int delay);

void realize(Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH], SceneEvent* future_tree, int frame_rate)
{
    int now = 0; // time in ms since the start
    int ms_between_frames = 1000 / frame_rate;
    CanvasLayer* trees = NULL;

    while (future_tree != NULL)
    {
        if (now >= future_tree->when)
        {
            while (future_tree != NULL && now >= future_tree->when) {
                trees = add_or_update_tree(trees, &future_tree->desired);
                future_tree = future_tree->next;
            }
            canvas_wipe(canvas);
            write_layers_on_canvas(canvas, trees);
            canvas_print(canvas);
            reset_cursor();
        }
        usleep(ms_between_frames * 1000);
        now += ms_between_frames;
    }
}

SceneEvent* after(SceneEvent* curr_tree, int delay, Tree tree)
{
    int moment = curr_tree->when + delay;
    return plan_at(curr_tree, moment, tree);
}

SceneEvent* from(Tree tree, int delay)
{
    SceneEvent* first_tree = (SceneEvent*)malloc(sizeof(SceneEvent));
    first_tree->desired = tree;
    first_tree->when = delay;
    first_tree->next = NULL;
    return first_tree;
}

SceneEvent* plan_at(SceneEvent* curr_tree, int moment, Tree tree) {
    if (curr_tree->next == NULL) {
        curr_tree->next = from(tree, moment);
        return curr_tree->next;
    }

    if (moment < curr_tree->next->when) {
        SceneEvent* next_tree = from(tree, moment);
        next_tree->next = curr_tree->next;
        curr_tree->next = next_tree;
        return next_tree;
    }

    return plan_at(curr_tree->next, moment, tree);
}

SceneEvent* start_with(Tree tree)
{
    return from(tree, 0);
}
