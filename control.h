#ifndef CONTROL_H
#define CONTROL_H

#include "hk.h"

typedef struct {
    Window *window;
    GLKVector3 camera_position;
    GLKVector3 camera_front;
    GLKVector3 camera_up;
    float camera_speed;
    float yaw;
    float pitch;
    GLKVector2 cursor_last_position;
    float fov;
    float delta_time;
    float last_frame;
} Control;

Control *control_init(Window *window);
void control_destroy(Control *control);

void control_process_key(Control *control);
void control_process_mouse(Control *control);
GLKMatrix4 control_get_projection(Control *control);
GLKMatrix4 control_get_view(Control *control);
GLKMatrix4 control_get_model(Control *control);
void control_time(Control *control);

#endif
