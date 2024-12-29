#ifndef SCENE_H
#define SCENE_H

#include "canvas.h"

// Descriptor of a moment/event within a scene.
typedef struct SceneEvent SceneEvent;
struct SceneEvent
{
    Tree desired;     // what the Tree will become in this event
    int when;         // the moment (ms since the start) this event should occur
    SceneEvent* next; // very next event
};

//==============================================
// SceneEvent constructing functions
//======

// Create a `SceneEvent` wherein `tree` appears.
// This is often how a "scene" is started.
SceneEvent* start_with(Tree tree);

// Create a `SceneEvent` wherein `tree.id` becomes `tree`, `delay`ms after `curr_tree`
SceneEvent* after(SceneEvent* event, Tree tree, int delay);

// Create a `SceneEvent` where `tree.id` becomes `tree`, in `scene` at `moment`ms since the start
SceneEvent* plan_at(SceneEvent* scene, Tree tree, int moment);

//==============================================
// Scene playing functions
//==============================================

// Renders an entire scene on `canvas` at `frame_rate` frames per second.
// - All `SceneEvent`s that occur since the previous frame are rendered together, in order (if the same object is updated in multiple events, the last one wins).
void scene_play(SceneEvent* scene, Pixel canvas[CANVAS_HEIGHT][CANVAS_WIDTH], int frame_rate);
#endif
