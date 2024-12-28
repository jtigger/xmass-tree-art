#ifndef SCENE_H
#define SCENE_H

#include "canvas.h"

typedef struct SceneEvent SceneEvent;
struct SceneEvent
{
    Tree desired;     // the to-be state of the tree
    int when;         // when (in ms since the start) this state should be applied to the canvas
    SceneEvent* next; // the next state to be applied to the canvas
};

// Play the sequence of future trees at the appropriate moments.
SceneEvent* after(SceneEvent* future_tree, int delay, Tree tree);
SceneEvent* plan_at(SceneEvent* curr_tree, int moment, Tree tree);
void realize(Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH], SceneEvent* future_tree, int frame_rate);
SceneEvent* start_with(Tree tree);


#endif