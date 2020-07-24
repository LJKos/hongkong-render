
#include "shader.h"

/*
 * helper functions
 */

void checkCompileErrors(unsigned int shader, const char *type) {
    int success;
    char log[1024];
    if (strcmp(type, "PROGRAM")) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, log);
            printf("error: SHADER_COMPILATION_ERROR of type: %s\n=> %s\n", type, log);
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, log);
            printf("error: SHADER_LINKING_ERROR of type: %s\n=> %s\n", type, log);
        }
    }
}

char *rsf(const char *path) {
    FILE *fp = fopen(path, "r");
    if (fp == NULL)
        return NULL;
    int len = 0;
    char *str = malloc(sizeof(char));
    char c;
    while (1) {
        c = fgetc(fp);
        if (feof(fp))
            break;
        len++;
        str = realloc(str, sizeof(char) * len);
        str[len - 1] = c;
    }
    str = realloc(str, sizeof(char) * (len + 1));
    str[len] = '\0';
    fclose(fp);
    return str;
}

/*
 * helper functions ^
 */

Shader *shader(const char *vertexPath, const char *fragmentPath) {
    Shader *ret = malloc(sizeof(Shader));
    
    const char *vertexSource = rsf(vertexPath);
    const char *fragmentSource = rsf(fragmentPath);
    
    unsigned int vertex, fragment;
    
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSource, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSource, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    
    ret->ID = glCreateProgram();
    glAttachShader(ret->ID, vertex);
    glAttachShader(ret->ID, fragment);
    glLinkProgram(ret->ID);
    checkCompileErrors(ret->ID, "PROGRAM");
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    
    free(vertexSource);
    free(fragmentSource);
    
    return ret;
}

void shader_destroy(Shader *self) {
    free(self);
}

void shader_use(Shader *self) {
    glUseProgram(self->ID);
}

void shader_sint(Shader *self, const char *name, int value) {
    glUniform1i(glGetUniformLocation(self->ID, name), value);
}

void shader_sfloat(Shader *self, const char *name, float value) {
    glUniform1i(glGetUniformLocation(self->ID, name), value);
}

void shader_smat4(Shader *self, const char *name, GLKMatrix4 *value) {
    glUniformMatrix4fv(glGetUniformLocation(self->ID, name), 1, GL_FALSE, &value->m);
}
