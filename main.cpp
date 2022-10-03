// g++ main.cpp glfw3dll.lib -l opengl32
#include "GL/glfw3.h"
#include <iostream>
#include <cmath>

#include "model.h"

RocketNN Rocket[21];
int species = 21;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    
    if (key == GLFW_KEY_A && action == GLFW_REPEAT || key == GLFW_KEY_A && action == GLFW_PRESS) {
        Rocket[0].angle -= 5.0; 
        a = GL_TRIANGLE_FAN;
    } else {
        a = GL_LINE_STRIP;
    }

    if (key == GLFW_KEY_D && action == GLFW_REPEAT || key == GLFW_KEY_D && action == GLFW_PRESS) {
        Rocket[0].angle += 5.0;
        d = GL_TRIANGLE_FAN;
    } else {
        d = GL_LINE_STRIP;
    }
}

int main(void) {
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

    std::vector<std::vector<int>> colors;
    for (int i = 0; i < species; i++) {
        colors.push_back(
            {
                0 + rand() % (( 255 + 1 ) - 0), 
                0 + rand() % (( 255 + 1 ) - 0), 
                0 + rand() % (( 255 + 1 ) - 0)
            });
    }

    while (!glfwWindowShouldClose(window)) {
        glfwMakeContextCurrent(window);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(.5,.7,.8,.5);

        atmosphere();
        keys();

        for (int i = 0; i < species; i++) {
            Rocket[i].rocket(colors[i][0], colors[i][1], colors[i][2]);
            Rocket[i].weighted_sum();
        }
        if (players_finished >= 20) {
            random_point = random_number();
            players_finished = 0;
            for (int i = 0; i < species; i++) {
                Rocket[i].finish = false;
                Rocket[i].speed = 0.0004;
                Rocket[i].x_speed = 0.004;
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();


        glfwMakeContextCurrent(nn_window);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0,0,0,0);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
        glEnable( GL_BLEND );

        std::vector<float> all_scores;
        for (int i = 0; i < species; i++) {
            all_scores.push_back(Rocket[i].score); 
        }
        int highest_score = std::distance(all_scores.begin(), std::find(all_scores.begin(), all_scores.end(), *std::max_element(all_scores.begin(), all_scores.end())));
        Rocket[highest_score].draw_nn();

        glfwSwapBuffers(nn_window);
        glfwPollEvents();
    }

    for (int i = 0; i < species; i++) {
        std::cout << i << " score: " << Rocket[i].score << std::endl;
    }

    glfwTerminate();
    return 0;
}
