
#include "hk.h"
#include "shader.h"
#include "control.h"

void error_callback(int error, const char* description) {
    fprintf(stderr, "error: %s\n", description);
}

int main(int argc, char *argv[]) {
    char *name = "window";
    Window w = { .width = 848, .height = 480, .name = name};
    
    Control *control = control_init(&w);
    
    glfwSetErrorCallback(error_callback);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    w.window = glfwCreateWindow(w.width, w.height, name, NULL, NULL);
    glfwMakeContextCurrent(w.window);
    
    Shader *sha = shader("shader.vs", "shader.fs");
    
    
    
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    float *map = malloc(sizeof(float) * 3505683);
    char buf[21];
    memset(buf, 0, 20);
    FILE *fp = fopen("hkhmap.txt", "r");
    for (int i = 0; i < 3505683; i++) {
        fgets(buf, 20, fp);
        map[i] = atof(buf) / 100;
        memset(buf, 0, 20);
    }
    fclose(fp);
    
    
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3505683, map, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    while (!glfwWindowShouldClose(w.window)) {
        control_time(control);
        control_process_key(control);
        control_process_mouse(control);
        
        glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        shader_use(sha);
        
        GLKMatrix4 projection = control_get_projection(control);
        shader_smat4(sha, "projection", &projection);
        
        GLKMatrix4 view = control_get_view(control);
        shader_smat4(sha, "view", &view);

        GLKMatrix4 model = control_get_model(control);
        shader_smat4(sha, "model", &model);
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, 1168561);
        
        glfwSwapBuffers(w.window);
        glfwPollEvents();
        
    }
    
    shader_destroy(sha);
    control_destroy(control);
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    glfwTerminate();
    free(map);
    return 0;
}
