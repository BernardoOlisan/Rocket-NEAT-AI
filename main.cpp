// g++ main.cpp glfw3dll.lib -l opengl32
#include "GL/glfw3.h"
#include "json/json.hpp"
#include <iostream>
#include <fstream>
#include <set>
#include <cmath>

#include "model.h"

using json = nlohmann::json;

RocketNN Rocket[20];
RocketNN Half_well_performed[10];
RocketNN Offspring_speciation[5];
RocketNN Old_Rockets[20];
float old_best_score;
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
    char mode;
    std::cout << "Point position Manual(M)/Random(R): ";
    std::cin >> mode;
    if (mode == 'M') {
        std::cout << "Choose the point position, between -20 and 61:" << std::endl;
        std::cin >> random_point;
    } else {
        random_point = random_number();
    }

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

    int cycles = 0;
    int initial_weights = 0;
    int same_structure = 0;
    while (!glfwWindowShouldClose(window)) {
        glfwMakeContextCurrent(window);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(.5,.7,.8,.5);

        atmosphere();
        keys();

        for (int i = 0; i < species; i++) {
            Rocket[i].rocket(colors[i][0], colors[i][1], colors[i][2]);

            // Mutate Random Weights, of the initial connections
            // if (initial_weights == 0) {
            //     for (int j = 0; j < Rocket[i].encoding_scheme_connections.size(); j++) {
            //         Rocket[i].encoding_scheme_connections[j][3] = float(random_weight())/10;
            //         // std::cout << Rocket[i].encoding_scheme_connections[j][3] << std::endl;
            //     }
            //     initial_weights = 1;
            // }

            //disable and enable
            for (int j = 0; j < Rocket[i].encoding_scheme_connections.size(); j++) {
                if (Rocket[i].encoding_scheme_connections[j][3] > 0 && Rocket[i].encoding_scheme_connections[j][4] != 0) {
                    Rocket[i].encoding_scheme_connections[j][4] = 1;
                } else {
                    Rocket[i].encoding_scheme_connections[j][4] = 0;
                }
            }
            Rocket[i].weighted_sum();
        }


        // EVOLVE process
        if (players_finished >= 20) {
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

            // CHECK THE OLD SCORE VS THE NEW SCORE
            float new_best_score = 0;
            for (int i = 0; i < 10; i++) {
                new_best_score += Half_well_performed[i].score;
            }


            //Chack if score is 10, Save the model and stop it when manage to get perfect results
            if (new_best_score >= 9.5) {
                /*
                Create a json file with the Encoding Schemes Nodes & Connections.
                Also save the point position
                */
                json jsonfile;
                jsonfile["Connections"] = Half_well_performed[0].encoding_scheme_connections;
                jsonfile["Nodes"] = Half_well_performed[0].encoding_scheme_nodes;
                jsonfile["Point Position"] = random_point;

                std::ofstream file("tryout/EncodingSchemes.json");
                file << jsonfile;
                file.close();
                std::cout << "-------------------FOUND A 9.5 SCORE STRUCTURE, SAVING IT TO A FILE---------------" << std::endl;
                break;
            }


            if (old_best_score > new_best_score) {
                same_structure += 1;
                if (same_structure == 20) { // UPDATING TO RANDOM WEIGHTS IF THE STRUCTURE IS REPEATED 20 TIMES
                    std::cout << "----SAME STRUCTURE WAS REPEATED 20 TIMES---- changing weights forzozamente" << std::endl;
                    for (int i = 0; i < species; i++) {
                        for (int j = 0; j < Rocket[i].encoding_scheme_connections.size(); j++) {
                            Rocket[i].encoding_scheme_connections[j][3] = float(random_weight())/10;
                            // std::cout << Rocket[i].encoding_scheme_connections[j][3] << std::endl;
                        }
                    }
                    same_structure = 0;
                }
                std::cout << "NO IS NOT HIGHER, USE THE OLD ROCKETS AGAIN. Old: " << old_best_score << " New: " << new_best_score << std::endl;
                for (int i = 0; i < species; i++) {
                    Rocket[i] = Old_Rockets[i];
                }

                // Halfwell perfomr again with the old rockets to mutate again and see if this time we manage to get better score
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

            } else {
                std::cout << "YES IS HIGHER, CONTINUE. Old: " << old_best_score << " New: " << new_best_score <<  std::endl;
            }

            // UPDATING THE OLD SCORE
            float old_best_score_sum = 0;
            for (int i = 0; i < 10; i++) {
                old_best_score_sum += Half_well_performed[i].score; 
            }
            //old_best_score = Half_well_performed[0].score;
            old_best_score = old_best_score_sum;
            
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
                std::vector<std::vector<std::vector<float>>> offspring_encoding_scheme_nodes = {
                    { // Input layer
                        {1, 0}, 
                        {2, 0},
                        {3, 0},
                        {4, 0},
                        {5, 0},
                        {6, 0},
                        {7, 0},
                        {8, 0}, 
                    }, 
                    { // Output layer
                        {9, 0},
                        {10, 0}
                    },
                    // hidden neurons
                    {} 
                };
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
                std::set<int> repeats;
                std::cout << "Parent 1" << std::endl;
                for (int j = 0; j < Half_well_performed[0+i].encoding_scheme_connections.size(); j++) {
                    offspring_encoding_scheme_connections.push_back(Half_well_performed[0+i].encoding_scheme_connections[j]);
                    std::cout << Half_well_performed[0+i].encoding_scheme_connections[j][5] << std::endl;
                    repeats.insert(offspring_encoding_scheme_connections[j][5]);
                }
                // Parent 2, 0-9, 1-8, 2-7, 3-6, 4-5
                std::cout << "Parent 2" << std::endl;
                for (int j = 0; j < Half_well_performed[9-i].encoding_scheme_connections.size(); j++) {
                    std::cout << Half_well_performed[9-i].encoding_scheme_connections[j][5] << std::endl;
            
                    if (repeats.find(Half_well_performed[9-i].encoding_scheme_connections[j][5]) == repeats.end()) {
                        offspring_encoding_scheme_connections.push_back(Half_well_performed[9-i].encoding_scheme_connections[j]);
                    }
                    repeats.insert(offspring_encoding_scheme_connections[j][5]);
                } 

                // updating the scheme nodes
                int no_hidden = 0;
                for (int j = 0; j < offspring_encoding_scheme_connections.size(); j++) {
                    for (int k = 0; k < offspring_encoding_scheme_connections.size(); k++) {
                        // Cheking if there is hidden neurons so we delete {0, -10, 0}
                        if (offspring_encoding_scheme_connections[k][0] >= 11) {
                            no_hidden = 1;
                        }
                        if (offspring_encoding_scheme_connections[k][1] >= 11) {
                            no_hidden = 1;
                        }
                    }

                    if (offspring_encoding_scheme_connections[j][0] >= 11) {
                        offspring_encoding_scheme_nodes[2].push_back(
                            {offspring_encoding_scheme_connections[j][0], 0.5, 0}
                        );
                    }
                    if (offspring_encoding_scheme_connections[j][1] >= 11) {
                        offspring_encoding_scheme_nodes[2].push_back(
                            {offspring_encoding_scheme_connections[j][1], 0.5, 0}
                        );
                    }
                    if (no_hidden == 0) {
                        offspring_encoding_scheme_nodes[2].push_back(
                            {0, -10, 0}
                        );
                    }
                }
                // delete repeated scheme hidden nodes and sort them
                if (offspring_encoding_scheme_nodes[2].size() > 1) {
                    offspring_encoding_scheme_nodes[2].erase(offspring_encoding_scheme_nodes[2].begin());
                }
                std::sort(offspring_encoding_scheme_nodes[2].begin(), offspring_encoding_scheme_nodes[2].end());
                offspring_encoding_scheme_nodes[2].erase(std::unique(offspring_encoding_scheme_nodes[2].begin(), offspring_encoding_scheme_nodes[2].end()), offspring_encoding_scheme_nodes[2].end());

                std::cout << "Offspring num " << 0+i << "-" << 9-i << std::endl;
                for (int j = 0; j < offspring_encoding_scheme_connections.size(); j++) {
                    std::cout << offspring_encoding_scheme_connections[j][5] << std::endl;
                }

                // adding each offspring crossover to the real array 5 of offspring and adding the new scheme nodes
                Offspring_speciation[i].encoding_scheme_connections = offspring_encoding_scheme_connections;
                Offspring_speciation[i].encoding_scheme_nodes = offspring_encoding_scheme_nodes;
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
            int n = 0;
            for (int i = 0; i < 20; i++) {
                //1234, 1234, 1234, 1234, 1234 = 20
                Rocket[i].encoding_scheme_connections = Offspring_speciation[n].encoding_scheme_connections;  
                if (n == 4) {
                    n = 0;
                } else {
                    n += 1;
                }
            }

            // Save the old rockets(before mutation rockets)
            std::cout << "SAVING THE ROCKETS AFTER MUTATION" << std::endl;
            for (int i = 0; i < species; i++) {
                Old_Rockets[i] = Rocket[i];
            }

            std::cout << "RANDOM MUTATION FOR THE 20 AND START AGAIN" << std::endl;
            // MAKE THE RANDOM INITIAL MUTATE AGAIN FOR THE 20 SPECIES, ADDING NEURONS AND MORE d)
            for (int i = 0; i < species; i++) {
                Rocket[i].rocket_mutation();

                // Deleting the disable connections
                for (int j = 0; j < Rocket[i].encoding_scheme_connections.size(); j++) {
                    if (Rocket[i].encoding_scheme_connections[j][4] == 0) {
                        Rocket[i].encoding_scheme_connections.erase(Rocket[i].encoding_scheme_connections.begin()+j);
                    }
                }
            }


            // Reset parameters
            //random_point = random_number();
            players_finished = 0;
            for (int i = 0; i < species; i++) {
                Rocket[i].finish = false;
                if (Rocket[i].encoding_scheme_connections.size() > 60){
                    Rocket[i].speed = 0.0006;
                    Rocket[i].x_speed = 0.006;
                } else {
                    Rocket[i].speed = 0.0004;
                    Rocket[i].x_speed = 0.004;
                }
                Rocket[i].score = 0.0;
            }
            cycles += 1;
            std::cout << "----------------------------------Life cycle " << cycles << " finished------------------------------" << std::endl;
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
