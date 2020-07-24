#ifndef SHADER_H
#define SHADER_H

#include <OpenGL/gl3.h>
#include <GLKit/GLKMath.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned int ID;
} Shader;
Shader *shader(const char *vertexPath, const char *fragmentPath);
void shader_destroy(Shader *self);
void shader_use(Shader *self);
void shader_sint(Shader *self, const char *name, int value);
void shader_sfloat(Shader *self, const char *name, float value);
void shader_smat4(Shader *self, const char *name, GLKMatrix4 *value);

#endif
