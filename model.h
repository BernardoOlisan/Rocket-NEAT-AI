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
            {11, 0.5, 0}, // {neuron number, neuron position(0.5 is the starting), value}
            {12, 0.5, 0},
            {13, 0.5, 0},
            {14, 0.5, 0},
            {15, 0.5, 0}
        }
    };

    // Connections
    std::vector<std::vector<float>> encoding_scheme_connections = {
        { // {In, Out, Out position neuron, Weight value, enable-disable(1-0), innovation number}
            1, 
            11,
            0, 
            0.1, 
            std::max(float(0), float(0.1/std::fabs(0.1))), 
            1,
        },
        {
            11, 
            12,
            0, 
            0.1, 
            std::max(float(0), float(0.1/std::fabs(0.1))), 
            1,
        },
        {
            12, 
            13,
            0, 
            0.1, 
            std::max(float(0), float(0.1/std::fabs(0.1))), 
            1,
        },
        {
            13, 
            14,
            0, 
            0.1, 
            std::max(float(0), float(0.1/std::fabs(0.1))), 
            1,
        },
        {
            14, 
            15,
            0, 
            0.1, 
            std::max(float(0), float(0.1/std::fabs(0.1))), 
            1,
        },
        {
            8, 
            15,
            0, 
            0.1, 
            std::max(float(0), float(0.1/std::fabs(0.1))), 
            1,
        },
        {
            7, 
            14,
            0, 
            0.1, 
            std::max(float(0), float(0.1/std::fabs(0.1))), 
            1,
        },
        {
            6, 
            13,
            0, 
            0.1, 
            std::max(float(0), float(0.1/std::fabs(0.1))), 
            1,
        },
        {
            5, 
            12,
            0, 
            0.1, 
            std::max(float(0), float(0.1/std::fabs(0.1))), 
            1,
        },
        {
            4, 
            11,
            0, 
            0.1, 
            std::max(float(0), float(0.1/std::fabs(0.1))), 
            1,
        },
        {
            2, 
            9,
            0, 
            0.1, 
            std::max(float(0), float(0.1/std::fabs(0.1))), 
            1,
        },
        {
            3, 
            10,
            0, 
            0.1, 
            std::max(float(0), float(0.1/std::fabs(0.1))), 
            1,
        },
        {
            15, 
            9,
            0, 
            0.1, 
            std::max(float(0), float(0.1/std::fabs(0.1))), 
            1,
        },
        {
            15, 
            10,
            0, 
            0.1, 
            std::max(float(0), float(0.1/std::fabs(0.1))), 
            1,
        },
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

            // Input layer (synapsis, INPUT in output)
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
                                    glVertex2f(-encoding_scheme_nodes[2][j][1]-0.06, encoding_scheme_connections[f][2]); 
                                glEnd();
                            glPopMatrix();       
                        }
                    }
                }
            }
        }


        // Hidden nodes (neurons)
        int active_neuron = 0;
        for (int i = 0; i < encoding_scheme_nodes[2].size(); i++) {
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

            // Hidden nodes (synapsis, INPUT in output)
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

                    // Hidden nodes (synapsis, INPUT in hidden)
                    for (int j = 0; j < encoding_scheme_nodes[2].size(); j++) {
                        if (encoding_scheme_connections[f][1] == encoding_scheme_nodes[2][j][0] && encoding_scheme_connections[f][0] != encoding_scheme_connections[f][1]) {
                            glPushMatrix();
                                glBegin(GL_LINES);
                                    glVertex2f(0.0-encoding_scheme_nodes[2][i][1]+0.06, (float(i) * float(0.8) / 5) - (float(encoding_scheme_nodes[2].size())*0.65/10));
                                    glVertex2f(0.0-encoding_scheme_nodes[2][j][1]-0.06, encoding_scheme_connections[f][2]); 
                                glEnd();
                            glPopMatrix();
                        } 
                    }
                }
            }

            // Hidden nodes (synapsis, INPUT in hidden "y")
            for (int f = 0; f < encoding_scheme_connections.size(); f++) {
                for (int j = 0; j < encoding_scheme_nodes[2].size(); j++)  {
                    if (encoding_scheme_connections[f][1] == encoding_scheme_nodes[2][j][0]) {
                        encoding_scheme_connections[f][2] = (float(j) * float(0.8) / 5) - (float(encoding_scheme_nodes[2].size())*0.65/10);
                    }
                }
            }
            
            // Change Hidden neurons position if Hidden to Hidden Connections
            int HiddentoHiddenConnections = 0;
            for (int f = 0; f < encoding_scheme_connections.size(); f++) {
                if (encoding_scheme_connections[f][0] >= encoding_scheme_nodes[2][0][0] && encoding_scheme_connections[f][1] >= encoding_scheme_nodes[2][0][0]) {
                    for (int j = 0; j < encoding_scheme_nodes[2].size(); j++) {
                        if (encoding_scheme_connections[f][1] == encoding_scheme_nodes[2][j][0] && encoding_scheme_connections[f][0] != encoding_scheme_connections[f][1]) {
                            encoding_scheme_nodes[2][j][1] = (float(HiddentoHiddenConnections) / 5 * -1)+0.30;
                            
                            int CountHiddenMultipleConnections = 0;
                            for (int k = 0; k < encoding_scheme_connections.size(); k++) {
                                if (encoding_scheme_connections[f][1] == encoding_scheme_connections[k][1]) {
                                    CountHiddenMultipleConnections += 1;
                                }
                            }

                            if (CountHiddenMultipleConnections >= 2) {
                                encoding_scheme_nodes[2][j][1] = encoding_scheme_nodes[2][j][1]-(float(HiddentoHiddenConnections) / 10 * -1)+0.1;
                            }

                            HiddentoHiddenConnections += 1;
                        }
                    } 
                }
            }
        }


        // Output layer (neurons)
        for (int i = 0; i < encoding_scheme_nodes[1].size(); i++) {
            glBegin(GL_POLYGON);
                for (int f = 0; f < 20; f++)
                {
                    float theta = 2.0 * 3.1415926 * float(f) / float(20);

                    float x = 0.06 * std::cos(theta);
                    float y = 0.06 * std::sin(theta);

                    glVertex2f(x + 0.6, y + float(i) * float(0.8) / 5);
                }
            glEnd();

            // Output layer (synapsis, INPUT in output "y")
            for (int f = 0; f < encoding_scheme_connections.size(); f++) {
                for (int j = 0; j < encoding_scheme_nodes[1].size(); j++) {
                    if (encoding_scheme_connections[f][1] == encoding_scheme_nodes[1][j][0]) {
                        encoding_scheme_connections[f][2] = (float(j) * float(0.8) / 5) + 0.01;
                    }
                }
            }
        }
    }


    void weighted_sum() { 
        // Inputs to Hiddens  
        for (int i = 0; i < encoding_scheme_connections.size(); i++) {
            for (int j = 0; j < encoding_scheme_nodes[2].size(); j++) {
                if (encoding_scheme_connections[i][1] == encoding_scheme_nodes[2][j][0]) {
                    for (int k = 0; k < encoding_scheme_nodes[0].size(); k++) {
                        if (encoding_scheme_connections[i][0] == encoding_scheme_nodes[0][k][0]) {
                            encoding_scheme_nodes[2][j][2] += (encoding_scheme_connections[i][3] * encoding_scheme_nodes[0][k][1]);
                        }
                    }
                }
            }
        }

        // Hiddens to Hiddens
        for (int i = 0; i < encoding_scheme_connections.size(); i++) {
            if (encoding_scheme_connections[i][0] >= encoding_scheme_nodes[2][0][0] && encoding_scheme_connections[i][1] >= encoding_scheme_nodes[2][0][0]) {
                for (int j = 0; j < encoding_scheme_nodes[2].size(); j++) {
                    if (encoding_scheme_connections[i][1] == encoding_scheme_nodes[2][j][0]) {
                        for (int k = 0; k < encoding_scheme_nodes[2].size(); k++) {
                            if (encoding_scheme_connections[i][0] == encoding_scheme_nodes[2][k][0]) {
                                encoding_scheme_nodes[2][j][2] += (encoding_scheme_connections[i][3] * encoding_scheme_nodes[2][k][2]);
                            }
                        }
                    }
                }
            }
        }

        // Hiddens to Outputs, left (output 9)
        for (int i = 0; i < encoding_scheme_connections.size(); i++) {
            if (encoding_scheme_connections[i][1] == encoding_scheme_nodes[1][0][0]) {
                for (int j = 0; j < encoding_scheme_nodes[2].size(); j++) {
                    if (encoding_scheme_connections[i][0] == encoding_scheme_nodes[2][j][0]) {
                        encoding_scheme_nodes[1][0][1] += (encoding_scheme_connections[i][3] * encoding_scheme_nodes[2][j][2]);
                    }
                }

                // Input to Output, Left (output 9)
                for (int j = 0; j < encoding_scheme_nodes[0].size(); j++) {
                    if (encoding_scheme_connections[i][0] == encoding_scheme_nodes[0][j][0]) {
                        encoding_scheme_nodes[1][0][1] += (encoding_scheme_connections[i][3] * encoding_scheme_nodes[0][j][1]);
                    }
                }
            }
            // Hiddens to Outputs, Right (output 10)
            if (encoding_scheme_connections[i][1] == encoding_scheme_nodes[1][1][0]) {
                for (int j = 0; j < encoding_scheme_nodes[2].size(); j++) {
                    if (encoding_scheme_connections[i][0] == encoding_scheme_nodes[2][j][0]) {
                        encoding_scheme_nodes[1][1][1] += (encoding_scheme_connections[i][3] * encoding_scheme_nodes[2][j][2]);
                    }
                }
                // Input to Output, Right (output 10)
                for (int j = 0; j < encoding_scheme_nodes[0].size(); j++) {
                    if (encoding_scheme_connections[i][0] == encoding_scheme_nodes[0][j][0]) {
                        encoding_scheme_nodes[1][1][1] += (encoding_scheme_connections[i][3] * encoding_scheme_nodes[0][j][1]);
                    }
                }
            }
        }

        // DELETE THIS, is just for debug cases
        // for (int i = 0; i < encoding_scheme_nodes[0].size(); i++) {
        //     std::cout << "X" << encoding_scheme_nodes[0][i][0] << " = " << encoding_scheme_nodes[0][i][1] << std::endl;
        // }
        // std::cout << "----------Hidden NEURONS-----------" << std::endl;
        // for (int i = 0; i < encoding_scheme_nodes[2].size(); i++) {
        //     std::cout << "Y" << encoding_scheme_nodes[2][i][0] << " = " << encoding_scheme_nodes[2][i][2] << std::endl;       
        // }
        // std::cout << "----------OUTPUT NEURONS-----------" << std::endl;
        // for (int i = 0; i < encoding_scheme_nodes[1].size(); i++) {
        //     std::cout << "Z" << encoding_scheme_nodes[1][i][0] << " = " << encoding_scheme_nodes[1][i][1] << std::endl;
        // }


        // Action rocket stuff
        if (encoding_scheme_nodes[1][0][1] > encoding_scheme_nodes[1][1][1] && encoding_scheme_nodes[1][0][1] > 0) {
            std::cout << "left" << std::endl;
            angle -= 0.01;
            a = GL_TRIANGLE_FAN;
            d = GL_LINE_STRIP;

        } else if (encoding_scheme_nodes[1][0][1] < encoding_scheme_nodes[1][1][1] && encoding_scheme_nodes[1][1][1] > 0) {
            std::cout << "right" << std::endl;
            angle += 0.01;
            d = GL_TRIANGLE_FAN;
            a = GL_LINE_STRIP;
        } 

        // Cleaning the hidden and output nodes data (weighted sum)
        for (int i = 0; i < encoding_scheme_nodes[2].size(); i++) {
            encoding_scheme_nodes[2][i][2] = 0;
        }
        for (int i = 0; i < encoding_scheme_nodes[1].size(); i++) {
            encoding_scheme_nodes[1][i][1] = 0;
        }
    }
};


/*
FIXME:
none
*/

/* 
TODO:
2.5 Depending on the weighted sum, the hidden neurons will activate

3. Check disable and enable in encoding scheme

4. Another window to preview the NN

5. Make the evolve process 

6. clean code

7. optimized it (try to replace "raw loops" with "monadic algorithms loops(std::any_of)")
*/