#include "../GL/glfw3.h"
#include <cstdlib>
#include <cmath>
#include <time.h>
#include <vector>
#include <string>

#include "../modules.h"

char a = GL_LINE_STRIP;
char d = GL_LINE_STRIP;

float point_position;

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
            glVertex3f(point_position/41-0.5, std::cos(point_position/41-0.5)-0.2, 0); 
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

    // Neat Encoding Scheme "Genotype"
    // Nodes
    std::vector<std::vector<std::vector<float>>> encoding_scheme_nodes;

    // Connections
    std::vector<std::vector<float>> encoding_scheme_connections;


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
        if (rocket_top >= std::cos(point_position/41-0.5)-0.2 && 
            rocket_x-0.03 <= point_position/41-0.5 && 
            rocket_x+0.03 >= point_position/41-0.5) {
            std::cout << "Good!" << std::endl;
            rocket_top = -0.85; 
            rocket_bottom = -0.95; 
            rocket_x = 0.00; 
            angle = 0.0;
            speed = 0.0004;
            x_speed = 0.004;
        } else if (rocket_top >= std::cos(rocket_x) - 0.2 || 
                   rocket_x <= -1.0 || rocket_x >= 1.0 || 
                   rocket_top <= -1.0) {
            std::cout << "Bad" << std::endl;
            rocket_top = -0.85; 
            rocket_bottom = -0.95;  
            rocket_x = 0.00; 
            angle = 0.0;
            speed = 0.0004;
            x_speed = 0.004;
        }

        // Values for input neurons
        encoding_scheme_nodes[0][0][1] = 1-((std::cos(rocket_x) - 0.2) - rocket_top);
        encoding_scheme_nodes[0][1][1] = rocket_x;
        encoding_scheme_nodes[0][2][1] = 0.15 - rocket_top;
        encoding_scheme_nodes[0][3][1] = 0 - rocket_x;

        encoding_scheme_nodes[0][4][1] = rotation;

        encoding_scheme_nodes[0][5][1] = point_position/41-0.5;
        encoding_scheme_nodes[0][6][1] = std::cos(point_position/41-0.5) - 0.2;
        
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
                neuron(0.8, (float(i) * float(0.8) / 5) - 0.5);
            glEnd();
            glColor4f(1, 1, 1, encoding_scheme_nodes[0][i][1]);
            glBegin(GL_POLYGON);
                neuron(0.8, (float(i) * float(0.8) / 5) - 0.5);
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
                                    glVertex2f(-encoding_scheme_nodes[2][j][1]-0.06, 
                                                encoding_scheme_connections[f][2]); 
                                glEnd();
                            glPopMatrix();       
                        }
                    }
                }
            }
        }


        // Hidden nodes (neurons)
        for (int i = 0; i < encoding_scheme_nodes[2].size(); i++) {
            glColor3f(1, 1, 1);
            glBegin(GL_LINE_LOOP);
                neuron(encoding_scheme_nodes[2][i][1], (float(i) * float(0.8) / 5) - 
                      (float(encoding_scheme_nodes[2].size())*0.65/10));
            glEnd();

            // Hidden nodes (synapsis, INPUT in output)
            for (int f = 0; f < encoding_scheme_connections.size(); f++) {
                if (encoding_scheme_connections[f][0] == encoding_scheme_nodes[2][i][0]) {
                    for (int j = 0; j < encoding_scheme_nodes[1].size(); j++) {
                        if (encoding_scheme_connections[f][1] == encoding_scheme_nodes[1][j][0]) {
                            glPushMatrix();
                                glBegin(GL_LINES);
                                    glVertex2f(-0.24 - (encoding_scheme_nodes[2][i][1]-0.3), 
                                              (float(i) * float(0.8) / 5) - 
                                              (float(encoding_scheme_nodes[2].size())*0.65/10));
                                    glVertex2f(0.6, encoding_scheme_connections[f][2]); 
                                glEnd();
                            glPopMatrix();
                        }
                    }

                    // Hidden nodes (synapsis, INPUT in hidden)
                    for (int j = 0; j < encoding_scheme_nodes[2].size(); j++) {
                        if (encoding_scheme_connections[f][1] == encoding_scheme_nodes[2][j][0] && 
                            encoding_scheme_connections[f][0] != encoding_scheme_connections[f][1]) {
                            glPushMatrix();
                                glBegin(GL_LINES);
                                    glVertex2f(0.0-encoding_scheme_nodes[2][i][1]+0.06, 
                                              (float(i) * float(0.8) / 5) - 
                                              (float(encoding_scheme_nodes[2].size())*0.65/10));
                                    glVertex2f(0.0-encoding_scheme_nodes[2][j][1]-0.06, 
                                               encoding_scheme_connections[f][2]); 
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
                        encoding_scheme_connections[f][2] = (float(j) * float(0.8) / 5) - 
                                                            (float(encoding_scheme_nodes[2].size())*0.65/10);
                    }
                }
            }
            
            // Change Hidden neurons position if Hidden to Hidden Connections
            int HiddentoHiddenConnections = 0;
            for (int f = 0; f < encoding_scheme_connections.size(); f++) {
                if (encoding_scheme_connections[f][0] >= encoding_scheme_nodes[2][0][0] && 
                    encoding_scheme_connections[f][1] >= encoding_scheme_nodes[2][0][0]) {
                    for (int j = 0; j < encoding_scheme_nodes[2].size(); j++) {
                        if (encoding_scheme_connections[f][1] == encoding_scheme_nodes[2][j][0] && 
                            encoding_scheme_connections[f][0] != encoding_scheme_connections[f][1]) {
                            encoding_scheme_nodes[2][j][1] = (float(HiddentoHiddenConnections) / 5 * -1)+0.30;
                            
                            int CountHiddenMultipleConnections = 0;
                            for (int k = 0; k < encoding_scheme_connections.size(); k++) {
                                if (encoding_scheme_connections[f][1] == encoding_scheme_connections[k][1]) {
                                    CountHiddenMultipleConnections += 1;
                                }
                            }

                            if (CountHiddenMultipleConnections >= 2) {
                                encoding_scheme_nodes[2][j][1] = encoding_scheme_nodes[2][j][1] -    
                                                                 (float(HiddentoHiddenConnections) / 10 * -1)+0.1;
                            }

                            HiddentoHiddenConnections += 1;
                        }
                    } 
                }
            }
        }


        // Output layer (neurons)
        for (int i = 0; i < encoding_scheme_nodes[1].size(); i++) {
            glColor3f(1, 1, 1);
            glBegin(GL_POLYGON);
                neuron((-0.66), float(i) * float(0.8) / 5);
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
                            encoding_scheme_nodes[2][j][2] += (encoding_scheme_connections[i][3] * 
                                                               encoding_scheme_nodes[0][k][1]);
                        }
                    }
                }
            }
        }

        // Hiddens to Hiddens
        for (int i = 0; i < encoding_scheme_connections.size(); i++) {
            if (encoding_scheme_connections[i][0] >= encoding_scheme_nodes[2][0][0] && 
                encoding_scheme_connections[i][1] >= encoding_scheme_nodes[2][0][0]) {
                for (int j = 0; j < encoding_scheme_nodes[2].size(); j++) {
                    if (encoding_scheme_connections[i][1] == encoding_scheme_nodes[2][j][0]) {
                        for (int k = 0; k < encoding_scheme_nodes[2].size(); k++) {
                            if (encoding_scheme_connections[i][0] == encoding_scheme_nodes[2][k][0]) {
                                encoding_scheme_nodes[2][j][2] += (encoding_scheme_connections[i][3] * 
                                                                   encoding_scheme_nodes[2][k][2]);
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
                        encoding_scheme_nodes[1][0][1] += (encoding_scheme_connections[i][3] * 
                                                           encoding_scheme_nodes[2][j][2]);
                    }
                }

                // Input to Output, Left (output 9)
                for (int j = 0; j < encoding_scheme_nodes[0].size(); j++) {
                    if (encoding_scheme_connections[i][0] == encoding_scheme_nodes[0][j][0]) {
                        encoding_scheme_nodes[1][0][1] += (encoding_scheme_connections[i][3] * 
                                                           encoding_scheme_nodes[0][j][1]);
                    }
                }
            }
            // Hiddens to Outputs, Right (output 10)
            if (encoding_scheme_connections[i][1] == encoding_scheme_nodes[1][1][0]) {
                for (int j = 0; j < encoding_scheme_nodes[2].size(); j++) {
                    if (encoding_scheme_connections[i][0] == encoding_scheme_nodes[2][j][0]) {
                        encoding_scheme_nodes[1][1][1] += (encoding_scheme_connections[i][3] * 
                                                           encoding_scheme_nodes[2][j][2]);
                    }
                }
                // Input to Output, Right (output 10)
                for (int j = 0; j < encoding_scheme_nodes[0].size(); j++) {
                    if (encoding_scheme_connections[i][0] == encoding_scheme_nodes[0][j][0]) {
                        encoding_scheme_nodes[1][1][1] += (encoding_scheme_connections[i][3] * 
                                                           encoding_scheme_nodes[0][j][1]);
                    }
                }
            }
        }
        
        // Rocket decisions
        if (encoding_scheme_nodes[1][0][1] > encoding_scheme_nodes[1][1][1] && 
            encoding_scheme_nodes[1][0][1] > 0) {
            angle -= 0.02;
            a = GL_TRIANGLE_FAN;
            d = GL_LINE_STRIP;

        } else if (encoding_scheme_nodes[1][0][1] < encoding_scheme_nodes[1][1][1] && 
            encoding_scheme_nodes[1][1][1] > 0) {
            angle += 0.02;
            d = GL_TRIANGLE_FAN;
            a = GL_LINE_STRIP;
        } 

        for (int i = 0; i < encoding_scheme_nodes[2].size(); i++) {
            encoding_scheme_nodes[2][i][2] = 0;
        }
        for (int i = 0; i < encoding_scheme_nodes[1].size(); i++) {
            encoding_scheme_nodes[1][i][1] = 0;
        }
    }
};