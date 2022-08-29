#include "GL/glfw3.h"
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <time.h>
#include <vector>

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

        glColor3f(1, 1, 1);
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
    float speed = 0.0004;
    float x_speed = 0.004;

    float rocket_top = -0.85;
    float rocket_bottom = -0.95;
    float rocket_x = 0.00;
    float angle = 0.0;

    float score = 0.0;

    // Input neurons (multidimentional vectors)
    // {{neurons value}, {neurons connected to}, {weights values}}
    std::vector<std::vector<float>> wall_north = {{0}, {0}, {0}};
    std::vector<std::vector<float>> wall_east = {{0}, {0}, {0}};
    std::vector<std::vector<float>> wall_south = {{0}, {0}, {0}};
    std::vector<std::vector<float>> wall_west = {{0}, {0}, {0}};
    std::vector<std::vector<float>> rocket_rotation = {{0}, {0}, {0}};
    std::vector<std::vector<float>> point_x = {{0}, {0}, {0}};
    std::vector<std::vector<float>> point_y = {{0}, {0}, {0}};
    std::vector<std::vector<float>> point_distance = {{0}, {0}, {0}};

    // Output neurons
    std::vector<float> left = {0};
    std::vector<float> right = {0};


    void rocket(int red, int green, int blue) {
        // Movements
        float rotation = angle*1/360;
        float top_movement = speed - rotation * speed;
        float side_movement = rotation * x_speed;

        if (std::fabs(rotation) > 0.25) {
            rocket_top -= top_movement;
            rocket_bottom -= top_movement; 
            rocket_x += side_movement;
        } else {
            rocket_top += top_movement;
            rocket_bottom += top_movement; 
            rocket_x += side_movement;
        }
        
        // Collisions
        if (rocket_top >= std::cos(random_num/41-0.5)-0.2 && 
            rocket_x-0.03 <= random_num/41-0.5 && 
            rocket_x+0.03 >= random_num/41-0.5) {
            std::cout << "Good!" << std::endl;
            rocket_top = -0.85; 
            rocket_bottom = -0.95; 
            rocket_x = 0.00; 
            angle = 0.0;
            random_num = random_number(); 
        } else if (rocket_top >= std::cos(rocket_x) - 0.2 || 
                   rocket_x <= -1.0 || rocket_x >= 1.0 || 
                   rocket_top <= -1.0) {
            std::cout << "Bad" << std::endl;
            rocket_top = -0.85; 
            rocket_bottom = -0.95;  
            rocket_x = 0.00; 
            angle = 0.0;
            random_num = random_number();
        }

        // Values for input neurons
        wall_north[0][0] = 1-((std::cos(rocket_x) - 0.2) - rocket_top);
        wall_east[0][0] = rocket_x;
        wall_south[0][0] = 0.15 - rocket_top;
        wall_west[0][0] = 0 - rocket_x;

        rocket_rotation[0][0] = rotation;

        point_x[0][0] = random_num/41-0.5;
        point_y[0][0] = std::cos(random_num/41-0.5) - 0.2;
        
        float x1 = rocket_x;
        float x2 = point_x[0][0];
        float y1 = rocket_top;
        float y2 = point_y[0][0];
        point_distance[0][0] = (1 - std::sqrt(pow(x2-x1, 2) + pow(y2-y1, 2)));

        // Rocket Graphics
        glPushMatrix();
            glColor3ub(red, green, blue);

            glTranslatef(0+rocket_x, 0+rocket_bottom, 0);
                glRotatef(angle, 0, 0, -1.0);
            glTranslatef(0, 0-rocket_bottom, 0);
            glBegin(GL_TRIANGLE_FAN);
                glVertex2f(0.02, rocket_top);
                glVertex2f(0.02, rocket_bottom);
                glVertex2f(-0.02, rocket_bottom);
                glVertex2f(-0.02, rocket_top);
            glEnd();

            glColor3f(0,0,0);
            glLineWidth(1.2);
            glBegin(GL_LINE_STRIP);
                glVertex2f(0.02, rocket_top);
                glVertex2f(-0.02, rocket_top);
                glVertex2f(-0.02, rocket_bottom);
                glVertex2f(0.02, rocket_bottom);
                glVertex2f(0.02, rocket_top);
            glEnd();

            glColor3f(1,1,1);
        glPopMatrix();
    } 


    void draw_nn() {
        // Input layer vector in "reverse" to get the correct order
        std::vector<float> input_layer = {
            point_distance[0][0], 
            point_y[0][0],
            point_x[0][0], 
            rocket_rotation[0][0],
            wall_west[0][0], 
            wall_south[0][0],
            wall_east[0][0], 
            wall_north[0][0]
        };

        // Input layer (neurons)
        for (int i = 0; i < input_layer.size(); i++) {
            glColor3f(1, 1, 1);
            glBegin(GL_LINE_LOOP);
                for(int f = 0; f < 20; f++)
                {
                    float theta = 2.0 * 3.1415926 * float(f) / float(20);

                    float x = 0.06 * std::cos(theta);
                    float y = 0.06 * std::sin(theta);

                    glVertex2f(x - 0.8, y + (float(i) * float(0.8) / 5) - 0.5);
                }
            glEnd();

            glColor4f(1, 1, 1, input_layer[i]);
            glBegin(GL_POLYGON);
                for(int f = 0; f < 20; f++)
                {
                    float theta = 2.0 * 3.1415926 * float(f) / float(20);

                    float x = 0.06 * std::cos(theta);
                    float y = 0.06 * std::sin(theta);

                    glVertex2f(x - 0.8, y + (float(i) * float(0.8) / 5) - 0.5);
                }
            glEnd();
            glColor3f(1, 1, 1);
        } 

        // Output layer (neurons)
        for (int i = 0; i < 2; i++) {
            glBegin(GL_POLYGON);
                for(int f = 0; f < 20; f++)
                {
                    float theta = 2.0 * 3.1415926 * float(f) / float(20);

                    float x = 0.06 * std::cos(theta);
                    float y = 0.06 * std::sin(theta);

                    glVertex2f(x + 0.6, y + float(i) * float(0.8) / 5);
                }
            glEnd();
        }
    }
};

//DONE 1. Pass the needed data to the input layers (environment sensors) all in -1 to 1 "1 means closest except in point_distance", relu in hidden will delete negative numbers

// 2. Connect with vector structure all the input and output layers (at the end there's going to be only one connection that activates sth) 

// 3. Another window to preview the NN

// 4. Make the evolve process 

// 5. clean code