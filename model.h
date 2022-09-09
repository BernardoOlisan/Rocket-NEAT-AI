#include "GL/glfw3.h"
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <time.h>
#include <vector>
#include <string>

char a = GL_LINE_STRIP;
char d = GL_LINE_STRIP;

float random_number() {
    srand(time(0));
    return std::rand() % 41 + (-1);
}

int random_weight() {
    return std::rand() % 10 + (-1);
}

float random_point = random_number();

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
            glVertex3f(random_point/41-0.5, std::cos(random_point/41-0.5)-0.2, 0); 
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

    // Neat Encoding Scheme "Genotype"
    // Nodes
    std::vector<std::vector<std::vector<float>>> encoding_scheme_nodes = {
        { // Input layer
            {1, 0}, // {wall_north, value} 
            {2, 0}, // {wall_east, value}
            {3, 0}, // {wall_south, value}
            {4, 0}, // {wall_west, value}
            {5, 0}, // {rocket_rotation, value}
            {6, 0}, // {point_x, value}
            {7, 0}, // {point_y, value}
            {8, 0}, // {point_distance, value}
        }, 
        { // Output layer
            {9, 0}, // {left, value}
            {10, 0} // {right, value}
        },
        { // Hidden Nodes
            // {11, 0.3, 0}, // {neuron number, neuron position(0.3 is the starting), value}
            // {12, 0.3, 0},
            // {13, 0.3, 0}
            {11, 0.3, 0},
            {12, 0.3, 0},
            {13, 0.3, 0},
            {14, 0.3, 0},
            {15, 0.3, 0},
            {16, 0.3, 0}
            
        }
    };

    // Connections
    std::vector<std::vector<float>> encoding_scheme_connections = {
        { // {In, Out, Out position neuron, Weight value, enable-disable(1-0), innovation number}
            1, 
            9,
            0, 
            0.1, 
            std::max(float(0), float(0.1/std::fabs(0.1))), 
            1,
        },
        {
            12, 
            14,
            0, 
            0.1, 
            std::max(float(0), float(0.1/std::fabs(0.1))), 
            1,   
        },
        {
            14,
            16,
            0,
            0.1,
            std::max(float(0), float(0.1/std::fabs(0.1))),
            2,
        },
        {
            11,
            11,
            0,
            0.1,
            std::max(float(0), float(0.1/std::fabs(0.1))),
            2,
        }
        // {
        //     11,
        //     10,
        //     0,
        //     0.1,
        //     std::max(float(0), float(0.1/std::fabs(0.1))),
        //     3,
        // }, 
        // {
        //     12,
        //     13,
        //     0,
        //     0.1,
        //     std::max(float(0), float(0.1/std::fabs(0.1))),
        //     3,
        // },
        // {
        //     13,
        //     10,
        //     0,
        //     0.1,
        //     std::max(float(0), float(0.1/std::fabs(0.1))),
        //     3,
        // }  

        // {
        //     1,
        //     10,
        //     0,
        //     0.2,
        //     std::max(float(0), float(0.2/std::fabs(0.2))), 
        //     2,
        // },
        // {
        //     2, 
        //     9,
        //     0,
        //     0.1,
        //     std::max(float(0), float(0.1/std::fabs(0.1))), 
        //     3,
        // },
        // {
        //     2,
        //     10,
        //     0,
        //     0.3,
        //     std::max(float(0), float(0.3/std::fabs(0.3))), 
        //     4
        // },
        // {
        //     3,
        //     9,
        //     0,
        //     0.3,
        //     std::max(float(0), float(0.3/std::fabs(0.3))), 
        //     5
        // },
        // {
        //     3,
        //     10,
        //     0,
        //     0.3,
        //     std::max(float(0), float(0.3/std::fabs(0.3))), 
        //     6
        // },
        // {
        //     4,
        //     9,
        //     0,
        //     0.3,
        //     std::max(float(0), float(0.3/std::fabs(0.3))), 
        //     7
        // },
        // {
        //     4,
        //     10,
        //     0,
        //     0.4,
        //     std::max(float(0), float(0.4/std::fabs(0.4))), 
        //     8
        // },
        // {
        //     5,
        //     9,
        //     0,
        //     0.1,
        //     std::max(float(0), float(0.1/std::fabs(0.1))), 
        //     9
        // },
        // {
        //     5,
        //     10,
        //     0,
        //     0.1,
        //     std::max(float(0), float(0.1/std::fabs(0.1))), 
        //     10
        // },
        // {
        //     6,
        //     9,
        //     0,
        //     0.2,
        //     std::max(float(0), float(0.2/std::fabs(0.2))), 
        //     11
        // },
        // {
        //     6,
        //     10,
        //     0,
        //     0.3,
        //     std::max(float(0), float(0.3/std::fabs(0.3))), 
        //     12
        // },
        // {
        //     7,
        //     9,
        //     0,
        //     0.3,
        //     std::max(float(0), float(0.3/std::fabs(0.3))), 
        //     13
        // },
        // {
        //     7,
        //     10,
        //     0,
        //     0.1,
        //     std::max(float(0), float(0.1/std::fabs(0.1))), 
        //     14
        // },
        // {
        //     8,
        //     9,
        //     0,
        //     -0.3,
        //     std::max(float(0), float(-0.3/std::fabs(-0.3))), 
        //     15
        // },
        // {
        //     8,
        //     10,
        //     0,
        //     0.5,
        //     std::max(float(0), float(0.5/std::fabs(0.5))), 
        //     16
        // },
    };


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
        if (rocket_top >= std::cos(random_point/41-0.5)-0.2 && 
            rocket_x-0.03 <= random_point/41-0.5 && 
            rocket_x+0.03 >= random_point/41-0.5) {
            std::cout << "Good!" << std::endl;
            rocket_top = -0.85; 
            rocket_bottom = -0.95; 
            rocket_x = 0.00; 
            angle = 0.0;
            random_point = random_number(); 
            // modify weights each time it finish track
            for (int i = 0; i < encoding_scheme_connections.size(); i++) {
                encoding_scheme_connections[i][3] = float(random_weight())/10;
            }
        } else if (rocket_top >= std::cos(rocket_x) - 0.2 || 
                   rocket_x <= -1.0 || rocket_x >= 1.0 || 
                   rocket_top <= -1.0) {
            std::cout << "Bad" << std::endl;
            rocket_top = -0.85; 
            rocket_bottom = -0.95;  
            rocket_x = 0.00; 
            angle = 0.0;
            random_point = random_number();
            // modify weights each time it finish track
            for (int i = 0; i < encoding_scheme_connections.size(); i++) {
                encoding_scheme_connections[i][3] = float(random_weight())/10;
            }
        }

        // Values for input neurons
        encoding_scheme_nodes[0][0][1] = 1-((std::cos(rocket_x) - 0.2) - rocket_top);
        encoding_scheme_nodes[0][1][1] = rocket_x;
        encoding_scheme_nodes[0][2][1] = 0.15 - rocket_top;
        encoding_scheme_nodes[0][3][1] = 0 - rocket_x;

        encoding_scheme_nodes[0][4][1] = rotation;

        encoding_scheme_nodes[0][5][1] = random_point/41-0.5;
        encoding_scheme_nodes[0][6][1] = std::cos(random_point/41-0.5) - 0.2;
        
        float x1 = rocket_x;
        float x2 = encoding_scheme_nodes[0][5][1];
        float y1 = rocket_top;
        float y2 = encoding_scheme_nodes[0][6][1];
        encoding_scheme_nodes[0][7][1] = (1 - std::sqrt(pow(x2-x1, 2) + pow(y2-y1, 2)));

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
        // Input layer (neurons)
        for (int i = 0; i < encoding_scheme_nodes[0].size(); i++) {
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

            glColor4f(1, 1, 1, encoding_scheme_nodes[0][i][1]);
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

            // Synapsis "In"
            for (int f = 0; f < encoding_scheme_connections.size(); f++) {
                if (encoding_scheme_connections[f][0] == i+1) {
                    for (int j = 0; j < encoding_scheme_nodes[1].size(); j++) {
                        if (encoding_scheme_connections[f][1] == encoding_scheme_nodes[1][j][0]) {
                            glPushMatrix();
                                glBegin(GL_LINES);
                                    glVertex2f(-0.74, (float(i) * float(0.8) / 5) - 0.5);
                                    glVertex2f(0.6, encoding_scheme_connections[f][2]); 
                                glEnd();
                            glPopMatrix();
                        }
                    }

                    for (int j = 0; j < encoding_scheme_nodes[2].size(); j++) {
                        if (encoding_scheme_connections[f][1] == encoding_scheme_nodes[2][j][0]) {
                            glPushMatrix();
                                glBegin(GL_LINES);
                                    glVertex2f(-0.74, (float(i) * float(0.8) / 5) - 0.5);
                                    glVertex2f(-0.36, encoding_scheme_connections[f][2]); 
                                glEnd();
                            glPopMatrix();       
                        }
                    }
                }
            }
        }

        // Output layer (neurons)
        for (int i = 0; i < encoding_scheme_nodes[1].size(); i++) {
            // Synapsis "Out in output neurons"
            for (int f = 0; f < encoding_scheme_connections.size(); f++) {
                for (int j = 0; j < encoding_scheme_nodes[1].size(); j++) {
                    if (encoding_scheme_connections[f][1] == encoding_scheme_nodes[1][j][0]) {
                        encoding_scheme_connections[f][2] = (float(j) * float(0.8) / 5) + 0.01;
                    }
                }
            }

            glBegin(GL_POLYGON);
                for (int f = 0; f < 20; f++)
                {
                    float theta = 2.0 * 3.1415926 * float(f) / float(20);

                    float x = 0.06 * std::cos(theta);
                    float y = 0.06 * std::sin(theta);

                    glVertex2f(x + 0.6, y + float(i) * float(0.8) / 5);
                }
            glEnd();
        }

        // Hidden nodes (neurons)
        int active_neuron = 0;

        for (int i = 0; i < encoding_scheme_nodes[2].size(); i++) {
            // Synapsis "In for hidden neurons"
            for (int f = 0; f < encoding_scheme_connections.size(); f++) {
                if (encoding_scheme_connections[f][0] == encoding_scheme_nodes[2][i][0]) {
                    for (int j = 0; j < encoding_scheme_nodes[1].size(); j++) {
                        if (encoding_scheme_connections[f][1] == encoding_scheme_nodes[1][j][0]) {
                            glPushMatrix();
                                glBegin(GL_LINES);
                                    glVertex2f(-0.24 - (encoding_scheme_nodes[2][i][1]-0.3), (float(i) * float(0.8) / 5) - (float(encoding_scheme_nodes[2].size())*0.65/10));
                                    glVertex2f(0.6, encoding_scheme_connections[f][2]); 
                                glEnd();
                            glPopMatrix();
                        }
                    }

                    // Synapsis "In for hidden to hidden neurons"
                    for (int j = 0; j < encoding_scheme_nodes[2].size(); j++) {
                        if (encoding_scheme_connections[f][1] == encoding_scheme_nodes[2][j][0]) {
                            glPushMatrix();
                                glBegin(GL_LINES);
                                    glVertex2f(-0.24, (float(i) * float(0.8) / 5) - (float(encoding_scheme_nodes[2].size())*0.65/10));
                                    glVertex2f((encoding_scheme_nodes[2][i][1]-0.31), encoding_scheme_connections[f][2]); 
                                glEnd();
                            glPopMatrix();
                        } 
                    }
                }
            }

            // Synapsis "Out in hidden neurons"
            for (int f = 0; f < encoding_scheme_connections.size(); f++) {
                for (int j = 0; j < encoding_scheme_nodes[2].size(); j++)  {
                    if (encoding_scheme_connections[f][1] == encoding_scheme_nodes[2][j][0]) {
                        encoding_scheme_connections[f][2] = (float(j) * float(0.8) / 5) - (float(encoding_scheme_nodes[2].size())*0.65/10);
                    }
                }
            }
            
            // Check if hidden are connected with hidden to change the position of the out hidden one
            for (int f = 0; f < encoding_scheme_connections.size(); f++) {
                if (encoding_scheme_connections[f][0] >= encoding_scheme_nodes[2][0][0] && encoding_scheme_connections[f][1] >= encoding_scheme_nodes[2][0][0]) {
                    for (int j = 0; j < encoding_scheme_nodes[2].size(); j++) {
                        if (encoding_scheme_connections[f][1] == encoding_scheme_nodes[2][j][0]) {
                            encoding_scheme_nodes[2][j][1] = -0.05;
                        } 
                    }
                }
            }

            glColor3f(1, 1, 1);
            glBegin(GL_LINE_LOOP);
                for(int f = 0; f < 20; f++)
                {
                    float theta = 2.0 * 3.1415926 * float(f) / float(20);

                    float x = 0.06 * std::cos(theta);
                    float y = 0.06 * std::sin(theta);

                    glVertex2f(x - encoding_scheme_nodes[2][i][1], y + (float(i) * float(0.8) / 5) - (float(encoding_scheme_nodes[2].size())*0.65/10));
                }
            glEnd();
            glColor4f(1, 1, 1, active_neuron);
            glBegin(GL_POLYGON);
                for (int f = 0; f < 20; f++)
                {
                    float theta = 2.0 * 3.1415926 * float(f) / float(20);

                    float x = 0.06 * std::cos(theta);
                    float y = 0.06 * std::sin(theta);

                    glVertex2f(x - encoding_scheme_nodes[2][i][1], y + (float(i) * float(0.8) / 5) - (float(encoding_scheme_nodes[2].size())*0.65/10));
                }
            glEnd();
            glColor3f(1, 1, 1);
        }
    }


    void weighted_sum() {
        float output1_neuron9_left = 0;
        int n1 = 0;
        for (int i = 0; i < encoding_scheme_connections.size(); i++) {
            if (encoding_scheme_connections[i][1] == encoding_scheme_nodes[1][0][0]) {
                output1_neuron9_left += (encoding_scheme_connections[i][3] * encoding_scheme_nodes[0][n1][1]);
                n1 += 1;
            }
        }

        float output2_neuron10_right = 0;
        int n2 = 0;
        for (int i = 0; i < encoding_scheme_connections.size(); i++) {
            if (encoding_scheme_connections[i][1] == encoding_scheme_nodes[1][1][0]) {
                output2_neuron10_right += (encoding_scheme_connections[i][3] * encoding_scheme_nodes[0][n2][1]);
                n2 += 1;
            }
        }

        // Adding values to output neurons
        encoding_scheme_nodes[1][0][1] = output1_neuron9_left;
        encoding_scheme_nodes[1][1][1] = output2_neuron10_right;
        // std::cout << "Output Neuron 9 = " << output1_neuron9_left << std::endl;
        // std::cout << "Output Neuron 10 = " << output2_neuron10_right << std::endl;
        if (output1_neuron9_left > output2_neuron10_right && output1_neuron9_left > 0) {
            std::cout << "left" << std::endl;
            angle -= 0.01;
            a = GL_TRIANGLE_FAN;
            d = GL_LINE_STRIP;

        } else if (output1_neuron9_left < output2_neuron10_right && output2_neuron10_right > 0) {
            std::cout << "right" << std::endl;
            angle += 0.01;
            d = GL_TRIANGLE_FAN;
            a = GL_LINE_STRIP;
        } 
        //std::cout << "max = " << std::max(output1_neuron9, output2_neuron10) << std::endl;
    }
};


/*
FIXME:
Problems in 2.1 and 2.2, everything works well, the problem is that if we connect 11 to 12 and then 
12 to 13 it doesn't work.

The other one is that if we connect 12 to 12 it bugs, is not possible (only in hiddens).
*/


/* 
TODO:
2.1 Extend the dynamic vector for hidden neurons, control everything with the arrays
2.2 the neurons nodes position is going to change the depending on how many hidden nodes they will have
2.3 Do the dynamic weighted sum with input neurons

2.4 Clean code

EXTRA.- Check if the weighted sum is correct

2.5 Depending on the weighted sum, the hidden neurons will activate

3. Check disable and enable in encoding scheme

4. Another window to preview the NN

5. Make the evolve process 

6. clean code

7. optimized it (try to replace "raw loops" with "monadic algorithms loops(std::any_of)")
*/