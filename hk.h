#ifndef HK_H
#define HK_H

#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <GLKit/GLKMath.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    GLFWwindow *window;
    unsigned int width;
    unsigned int height;
    char *name;
} Window;

#endif
