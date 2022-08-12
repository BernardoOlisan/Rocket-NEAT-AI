// g++ main.cpp objects.cpp glfw3dll.lib -l opengl32
#include "GL/glfw3.h"
#include <iostream>
#include <cmath>


float speed = 0.0003f;
float x_speed = 0.003f;
float angle = 0.0f;

float rocket_top = -0.9f;
float rocket_bottom = -0.6f;
float rocket_x = 0.05f;

extern char a;
extern char d;


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {glfwSetWindowShouldClose(window, GLFW_TRUE);}

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


void atmosphere();
void rocket(float rocket_top, float rocket_bottom, float rocket_x);
void keys();


int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(800, 800, "Rocket NEAT AI", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);


    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(.5,.7,.8,.5);

        atmosphere();

        if (rocket_top >= 1.0f) {rocket_top = -0.9f; rocket_bottom = -0.6f;} // Repeat when is touches atmosphere

        float rotation = angle*1/360;
        float top_movement = speed - rotation * speed;
        float side_movement = rotation * x_speed;

        if (std::fabs(rotation) > 0.25f) {
            rocket(rocket_top -= top_movement, rocket_bottom -= top_movement, rocket_x += side_movement);
        } else {
            rocket(rocket_top += top_movement, rocket_bottom += top_movement, rocket_x += side_movement);
        }

        keys();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
