#include "control.h"

Control *control_init(Window *window) {
    Control *control = malloc(sizeof(Control));
    
    control->window = window;
    
    control->camera_position.x = 0;
    control->camera_position.y = 0;
    control->camera_position.z = 0;
    
    control->camera_front.x = 0;
    control->camera_front.y = 0;
    control->camera_front.z = 0;
    
    control->camera_up.x = 0;
    control->camera_up.y = 1;
    control->camera_up.z = 0;
    
    control->camera_speed = 2.5;
    
    control->yaw = -90.0f;
    control->pitch = 0.0f;
    
    control->cursor_last_position.x = control->window->width / 2;
    control->cursor_last_position.y = control->window->height / 2;
    
    control->fov = 45.0f;
    
    control->delta_time = 0.0f;
    control->last_frame = 0.0f;
    
    return control;
}

void control_destroy(Control *control) {
    free(control);
}

void control_process_key(Control *control) {
    if (glfwGetKey(control->window->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(control->window->window, 1);
    
    if (glfwGetKey(control->window->window, GLFW_KEY_W) == GLFW_PRESS)
        control->camera_position = GLKVector3Add(control->camera_position, GLKVector3MultiplyScalar(control->camera_front, control->camera_speed * control->delta_time));
    
    if (glfwGetKey(control->window->window, GLFW_KEY_S) == GLFW_PRESS)
        control->camera_position = GLKVector3Subtract(control->camera_position, GLKVector3MultiplyScalar(control->camera_front, control->camera_speed * control->delta_time));
    
    if (glfwGetKey(control->window->window, GLFW_KEY_A) == GLFW_PRESS)
        control->camera_position = GLKVector3Subtract(control->camera_position, GLKVector3MultiplyScalar(GLKVector3Normalize(GLKVector3CrossProduct(control->camera_front, control->camera_up)), control->camera_speed * control->delta_time));
    
    if (glfwGetKey(control->window->window, GLFW_KEY_D) == GLFW_PRESS)
        control->camera_position = GLKVector3Add(control->camera_position, GLKVector3MultiplyScalar(GLKVector3Normalize(GLKVector3CrossProduct(control->camera_front, control->camera_up)), control->camera_speed * control->delta_time));

}

void control_process_mouse(Control *control) {
    double xpos, ypos;
    glfwGetCursorPos(control->window->window, &xpos, &ypos);
    float xoffset = xpos - control->cursor_last_position.x;
    float yoffset = control->cursor_last_position.y - ypos;
    control->cursor_last_position.x = xpos;
    control->cursor_last_position.y = ypos;
    
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    if (glfwGetMouseButton(control->window->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        control->yaw -= xoffset;
        control->pitch -= yoffset;
    }
    
    if (control->pitch > 89.0f)
        control->pitch = 89.0f;
    if (control->pitch < -89.0f)
        control->pitch = -89.0f;
    
    GLKVector3 front;
    front.x = cos(GLKMathDegreesToRadians(control->yaw)) * cos(GLKMathDegreesToRadians(control->pitch));
    front.y = sin(GLKMathDegreesToRadians(control->pitch));
    front.z = sin(GLKMathDegreesToRadians(control->yaw)) * cos(GLKMathDegreesToRadians(control->pitch));
    control->camera_front = GLKVector3Normalize(front);
}

GLKMatrix4 control_get_projection(Control *control) {
    return GLKMatrix4MakePerspective(GLKMathDegreesToRadians(control->fov), (float)control->window->width / (float)control->window->height, 0.1f, 100.0f);
}

GLKMatrix4 control_get_view(Control *control) {
    return GLKMatrix4MakeLookAt(control->camera_position.x, control->camera_position.y, control->camera_position.z, control->camera_position.x + control->camera_front.x, control->camera_position.y + control->camera_front.y, control->camera_position.z + control->camera_front.z, control->camera_up.x, control->camera_up.y, control->camera_up.z);
}

GLKMatrix4 control_get_model(Control *control) {
    return GLKMatrix4Identity;
}

void control_time(Control *control) {
    float current_frame = glfwGetTime();
    control->delta_time = current_frame - control->last_frame;
    control->last_frame = current_frame;
}
