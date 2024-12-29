
#include "scene.h"
#include "canvas.h"

void scene_play(SceneEvent* scene, Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH], int frame_rate)
{
    int now = 0; // time in ms since the start
    int ms_between_frames = 1000 / frame_rate;
    CanvasLayer* trees = NULL;

    SceneEvent* next_event = scene;
    while (next_event != NULL)
    {
        if (now >= next_event->when)
        {
            while (next_event != NULL && now >= next_event->when) {
                trees = add_or_update_tree(trees, &next_event->desired);
                next_event = next_event->next;
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

SceneEvent* after(SceneEvent* event, Tree tree, int delay)
{
    return plan_at(event, tree, event->when + delay);
}

SceneEvent* new_event(Tree tree, int moment)
{
    SceneEvent* event = (SceneEvent*)malloc(sizeof(SceneEvent));
    event->desired = tree;
    event->when = moment;
    event->next = NULL;
    return event;
}

SceneEvent* plan_at(SceneEvent* event, Tree tree, int moment) {
    if (event->next == NULL) {
        event->next = new_event(tree, moment);
        return event->next;
    }

    // strictly less than; causes subsequent events in the same moment to be scheduled later.
    // preserving the order of events within the same moment.
    if (moment < event->next->when) {
        SceneEvent* next_event = new_event(tree, moment);
        next_event->next = event->next;
        event->next = next_event;
        return next_event;
    }

    return plan_at(event->next, tree, moment);
}

SceneEvent* start_with(Tree tree)
{
    return new_event(tree, 0);
}
