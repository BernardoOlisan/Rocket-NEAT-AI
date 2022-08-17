#include "GL/glfw3.h"
#include <cstdlib>
#include <cmath>
#include <time.h>

extern float angle;

char a = GL_LINE_STRIP;
char d = GL_LINE_STRIP;

float random_number() {
    srand(time(0));
    return std::rand() % 41 + (-1);
}

float random_num = random_number();

void atmosphere() {
    glPushMatrix();

        glLineWidth(5);
        glBegin(GL_LINE_STRIP);
            for (float i=0;i<41;i++)
            {
                glVertex2f(i/20-1, std::cos(i/20-1)-0.2);
            };
        glEnd();

        glPointSize(15);
        glColor3ub(255, 0, 0);
        glBegin(GL_POINTS);  
            glVertex3f(random_num/41-0.5, std::cos(random_num/41-0.5)-0.2, 0); 
        glEnd();

        glColor3f(1,1,1);
    glPopMatrix();
}

void keys() {
    glPushMatrix();

        glLineWidth(2);
        glBegin(a);
            glVertex2f(-0.9, -0.9);
            glVertex2f(-0.9, -0.8);
            glVertex2f(-0.8, -0.8);
            glVertex2f(-0.8, -0.9);
            glVertex2f(-0.9, -0.9);
        glEnd();

        glBegin(d);
            glVertex2f(-0.8, -0.9);
            glVertex2f(-0.8, -0.8);
            glVertex2f(-0.7, -0.8);
            glVertex2f(-0.7, -0.9);
            glVertex2f(-0.8, -0.9);
        glEnd();
         
    glPopMatrix();
}


class RocketNN {
public:
    int input = 0;
    
    void rocket(
            float rocket_top, 
            float rocket_bottom, 
            float rocket_x, 
            int red, 
            int green, 
            int blue
        ) {
        glPushMatrix();

            glColor3ub(red, green, blue);
            glTranslatef(0+rocket_x, 0+rocket_top, 0);
                glRotatef(angle, 0, 0, -1.0f);
            glTranslatef(0, 0-rocket_top, 0);
            glBegin(GL_TRIANGLE_FAN);
                glVertex2f(0.02f, rocket_top);
                glVertex2f(0.02f, rocket_bottom);
                glVertex2f(-0.02f, rocket_bottom);
                glVertex2f(-0.02f, rocket_top);
            glEnd();

            glColor3f(0,0,0);
            glLineWidth(1.2);
            glBegin(GL_LINE_STRIP);
                glVertex2f(0.02f, rocket_top);
                glVertex2f(-0.02f, rocket_top);
                glVertex2f(-0.02f, rocket_bottom);
                glVertex2f(0.02f, rocket_bottom);
                glVertex2f(0.02f, rocket_top);
            glEnd();

            glColor3f(1,1,1);
        glPopMatrix();
    }
};

// 1. Simple mode for non-ai player with colission and everything, wall colission
// 2. Have AI NEAT mode, each rocket is going to be a nn, so change every
//thing so that the class can have the speed and controls of the rocket and
//in the main function you can just pass the class and the rocket creation,
//change the color or set it randomly so that multiple (20) rockets can evolve 
// 3. clean code 