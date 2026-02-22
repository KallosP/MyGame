#include "config.h"

// glfw = graphics library framework built on top of OpenGL: creates windows with OpenGL context, sets up callbacks, and handles timing/monitors for you

// IMPORTANT NOTE: if a crash occurs and terminal output has red x in circle, but no errors in output, you might be looking at a seg fault

int main(){

    GLFWwindow* window;

    if (!glfwInit()) {
        std::cout << "GLFW couldn't start" << std::endl;
        return -1;
    }

    window = glfwCreateWindow(640, 480, "My Window", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        glfwTerminate();
        return -1;
    }

    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
} 