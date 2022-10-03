#include "GL/glfw3.h"
#include <cmath>

void active_neuron(float x, float y) {
    glBegin(GL_LINE_LOOP);
        for(int f = 0; f < 20; f++)
        {
            float theta = 2.0 * 3.1415926 * float(f) / float(20);

            float x = 0.06 * std::cos(theta);
            float y = 0.06 * std::sin(theta);

            glVertex2f(x - x, y + y);
        }
    glEnd();
}

void inactive_neuron(float x, float y) {
    glBegin(GL_POLYGON);
        for(int f = 0; f < 20; f++)
        {
            float theta = 2.0 * 3.1415926 * float(f) / float(20);

            float x = 0.06 * std::cos(theta);
            float y = 0.06 * std::sin(theta);

            glVertex2f(x - x, y + y);
        }
    glEnd();
}