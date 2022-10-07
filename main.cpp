// g++ main.cpp glfw3dll.lib -l opengl32
#include "GL/glfw3.h"
#include <iostream>
#include <cmath>

#include "model.h"

RocketNN Rocket[20];
RocketNN Half_well_performed[10];
RocketNN Offspring_speciation[5];
int species = 20;

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

    //std::vector<int> half_well_performed;
    //std::vector<int> offspring_speciation;
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
            // Mututaion after ciclo finishes with mutation techniques 
            for (int i = 0; i < species; i++) {
                std::cout << i << " score: " << Rocket[i].score << std::endl;
            } 
            // Half well perform a)
            std::vector<float> all_scores_one;
            for (int i = 0; i < species; i++) {
                all_scores_one.push_back(Rocket[i].score); 
            }
            for (int i = 0; i < 10; i++) {
                int highest_score_one = std::distance(all_scores_one.begin(), std::find(all_scores_one.begin(), all_scores_one.end(), *std::max_element(all_scores_one.begin(), all_scores_one.end())));
                Half_well_performed[i] = Rocket[highest_score_one];
                all_scores_one[highest_score_one] = 0;
            }
            
            std::cout << "Half well performed rockets" << std::endl;
            for (int i = 0; i < 10; i++) {
                std::cout << Half_well_performed[i].score << std::endl;
            }
            

            std::cout << "CROSSOVER 10 OF THEM INTO 5 CHILDREN(OFFSPRING)" << std::endl;
            // CROSSOVER the half of them we stay with 5 b)
            /*
            we don't modify the rockets array until we have the next 20, to repeat the process
            . crossover con el primero y el ultimo de half_well_performed y lo agregamos x4 en 
              offspring_speciation y tumbamos el primero y el ultimo para que vayan los siguiente de 
              half_well_performed
            */
            //1-10 = 110
            for (int i = 0; i < 5; i++) {
                std::vector<std::vector<float>> offspring_encoding_scheme_connections = {
                    // { // {Input, Output, Out position neuron, Weight value, enable-disable(1-0), innovation number}
                    //     1, 
                    //     10,
                    //     0, 
                    //     0.1, 
                    //     0, 
                    //     110,
                    // },
                };
                // Parent 1 
                std::cout << "Parent 1" << std::endl;
                for (int j = 0; j < Half_well_performed[0+i].encoding_scheme_connections.size(); j++) {
                    offspring_encoding_scheme_connections.push_back(Half_well_performed[0+i].encoding_scheme_connections[j]);
                    std::cout << Half_well_performed[0+i].encoding_scheme_connections[j][5] << std::endl;
                }
                // Parent 2, 0-9, 1-8, 2-7, 3-6, 4-5
                std::cout << "Parent 2" << std::endl;
                for (int j = 0; j < Half_well_performed[9-i].encoding_scheme_connections.size(); j++) {
                    std::cout << Half_well_performed[9-i].encoding_scheme_connections[j][5] << std::endl;
                    for (int k = 0; k < offspring_encoding_scheme_connections.size(); k++) {
                        if (Half_well_performed[9-i].encoding_scheme_connections[j][5] != offspring_encoding_scheme_connections[k][5]) {
                            offspring_encoding_scheme_connections.push_back(Half_well_performed[9-i].encoding_scheme_connections[j]);
                        }
                    }
                } 
                std::cout << "Offspring num " << 0+i << "-" << 9-i << std::endl;
                for (int j = 0; j < offspring_encoding_scheme_connections.size(); j++) {
                    std::cout << offspring_encoding_scheme_connections[j][5] << std::endl;
                }

                // adding each offspring crossover to the real array 5 of offspring
                Offspring_speciation[i].encoding_scheme_connections = offspring_encoding_scheme_connections;
            }

            std::cout << "---------New 5 Offsprings" << std::endl;
            for (int i = 0; i < 5; i++) {
                std::cout << "Offspring num " << i << std::endl;
                for (int j = 0; j < Offspring_speciation[i].encoding_scheme_connections.size(); j++) {
                    std::cout << Offspring_speciation[i].encoding_scheme_connections[j][5] << std::endl;
                }
            }

            std::cout << "MULTIPLY OFFSPRINGS BY 4" << std::endl;
            // MULTIPLY OFFSPRING (5) BY 4, SO WE GET 20 again to initial mutate the 20 again c)

            for (int i = 0; i < 20; i++) {
                //1234, 1234, 1234, 1234, 1234 = 20
            }
            


            // Reset parameters
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

    glfwTerminate();
    return 0;
}
