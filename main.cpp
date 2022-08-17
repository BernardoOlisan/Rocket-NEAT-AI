// g++ main.cpp glfw3dll.lib -l opengl32
#include "GL/glfw3.h"
#include <iostream>
#include <cmath>

#include "model.h"

float speed = 0.0002f;
float x_speed = 0.002f;
float angle = 0.0f;

float rocket_top = -0.95f;
float rocket_bottom = -0.85f;
float rocket_x = 0.05f;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (key == GLFW_KEY_A && action == GLFW_REPEAT || key == GLFW_KEY_A && action == GLFW_PRESS) {
        angle -= 5.0f; 
        a = GL_TRIANGLE_FAN;
    } else {
        a = GL_LINE_STRIP;
    }

    if (key == GLFW_KEY_D && action == GLFW_REPEAT || key == GLFW_KEY_D && action == GLFW_PRESS) {
        angle += 5.0f;
        d = GL_TRIANGLE_FAN;
    } else {
        d = GL_LINE_STRIP;
    }
}


int main(void) {
    GLFWwindow* window;
    RocketNN Rocket;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(800, 800, "Rocket NEAT AI", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(.5,.7,.8,.5);

        atmosphere();
        keys();

        float rotation = angle*1/360;
        float top_movement = speed - rotation * speed;
        float side_movement = rotation * x_speed;
        
        if (std::fabs(rotation) > 0.25f) {
            Rocket.rocket(
                rocket_top -= top_movement, 
                rocket_bottom -= top_movement, 
                rocket_x += side_movement, 
                255,0,0
            );
        } else {
            Rocket.rocket(
                rocket_top += top_movement, 
                rocket_bottom += top_movement, 
                rocket_x += side_movement, 
                255,0,0
            );
        }

        if (rocket_bottom >= std::cos(random_num/41-0.5)-0.2 && 
            rocket_x-0.03 <= random_num/41-0.5 && 
            rocket_x+0.03 >= random_num/41-0.5) {

            std::cout << "Good!" << std::endl;
            rocket_top = -0.95f; 
            rocket_bottom = -0.85f; 
            angle = 0.0f;
            random_num = random_number(); 
            
        } else if (rocket_bottom >= std::cos(rocket_x)-0.2 || 
            rocket_x <= -1.0 || rocket_x >= 1.0 || 
            rocket_bottom <= -1.0) {

            std::cout << "Bad" << std::endl;
            rocket_top = -0.95f; 
            rocket_bottom = -0.85f;  
            rocket_x = 0.05f; 
            angle = 0.0f;
            random_num = random_number();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
