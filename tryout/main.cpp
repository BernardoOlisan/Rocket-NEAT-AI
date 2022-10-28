// g++ main.cpp glfw3dll.lib -l opengl32
#include "../GL/glfw3.h"
#include "../json/json.hpp"
#include <iostream>
#include <fstream>
#include <set>
#include <cmath>

#include "model.h"

using json = nlohmann::json;

RocketNN Rocket;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    
    if (key == GLFW_KEY_A && action == GLFW_REPEAT || key == GLFW_KEY_A && action == GLFW_PRESS) {
        Rocket.angle -= 5.0; 
        a = GL_TRIANGLE_FAN;
    } else {
        a = GL_LINE_STRIP;
    }

    if (key == GLFW_KEY_D && action == GLFW_REPEAT || key == GLFW_KEY_D && action == GLFW_PRESS) {
        Rocket.angle += 5.0;
        d = GL_TRIANGLE_FAN;
    } else {
        d = GL_LINE_STRIP;
    }
}

int main(void) {  
    // Loading Rocket Structure
    json RocketStructure;
    std::ifstream f("EncodingSchemes.json");
    f >> RocketStructure;

    point_position = RocketStructure["Point Position"];
    Rocket.encoding_scheme_nodes = RocketStructure["Nodes"];
    Rocket.encoding_scheme_connections = RocketStructure["Connections"];

    GLFWwindow* window;
    GLFWwindow* nn_window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(800, 800, "Rocket NEAT AI", NULL, NULL);
    nn_window = glfwCreateWindow(400, 400, "Neural Network Structure", NULL, window);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);

    while (!glfwWindowShouldClose(window)) {
        glfwMakeContextCurrent(window);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(.5,.7,.8,.5);

        atmosphere();
        keys();

        Rocket.rocket(255, 0, 0);
        Rocket.weighted_sum();

        glfwSwapBuffers(window);
        glfwPollEvents();


        glfwMakeContextCurrent(nn_window);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0,0,0,0);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
        glEnable( GL_BLEND );

        Rocket.draw_nn();

        glfwSwapBuffers(nn_window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
