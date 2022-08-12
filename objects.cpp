#include "GL/glfw3.h"
#include <cmath>


extern float angle;
char w = GL_LINE_STRIP;
char a = GL_LINE_STRIP;
char d = GL_LINE_STRIP;

void atmosphere() 
{
    glPushMatrix();
        glLineWidth(5);
        glBegin(GL_LINE_STRIP);
            for (float i=0;i<41;i++)
            {
                glVertex2f(i/20-1, std::cos(1*i/20-1)-0.2);
            };
        glEnd();
    glPopMatrix();
}

void rocket(float rocket_top, float rocket_bottom, float rocket_x) 
{
    glPushMatrix();
        glTranslatef(0+rocket_x, 0+rocket_top, 0);
            glPointSize(10);
            glBegin(GL_POINTS);  glVertex3f(0, 0, 0); glEnd();
        glRotatef(angle, 0, 0, -1.0f);
        glTranslatef(0, 0-rocket_top, 0);

        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(0.05f, rocket_top);
            glVertex2f(0.05f, rocket_bottom);
            glVertex2f(-0.05f, rocket_bottom);
            glVertex2f(-0.05f, rocket_top);
        glEnd();
    glPopMatrix();
}

void keys()
{
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