// g++ main.cpp glfw3dll.lib -l opengl32
#include "GL/glfw3.h"
#include "json/json.hpp"
#include <iostream>
#include <fstream>
#include <set>
#include <cmath>

#include "model.h"

using json = nlohmann::json;

RocketNN Rockets[20];
RocketNN HalfWellPerformed[10];
RocketNN OffspringSpecies[5];
RocketNN OldRockets[20];
float old_best_score;
int species = 20;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    
    if (key == GLFW_KEY_A && action == GLFW_REPEAT || 
        key == GLFW_KEY_A && action == GLFW_PRESS) {
        Rockets[0].angle -= 5.0; 
        a = GL_TRIANGLE_FAN;
    } else {
        a = GL_LINE_STRIP;
    }

    if (key == GLFW_KEY_D && action == GLFW_REPEAT || 
        key == GLFW_KEY_D && action == GLFW_PRESS) {
        Rockets[0].angle += 5.0;
        d = GL_TRIANGLE_FAN;
    } else {
        d = GL_LINE_STRIP;
    }
}

int main(void) {  
    char mode;
    std::cout << "*Point position Manual(M)/Random(any): ";
    std::cin >> mode;
    if (mode == 'M' || mode == 'm') {
        std::cout << "*Choose the point position, between -20 and 61: " << std::endl;
        std::cin >> point_position;
    } else {
        point_position = random_position();
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
            }
        );
    }

    int generation = 0;
    int repeated_nn = 0;
    while (!glfwWindowShouldClose(window)) {
        glfwMakeContextCurrent(window);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(.5,.7,.8,.5);

        atmosphere();
        keys();

        for (int i = 0; i < species; i++) {
            Rockets[i].rocket(colors[i][0], colors[i][1], colors[i][2]);

            // Check disable-enable
            for (int j = 0; j < Rockets[i].encoding_scheme_connections.size(); j++) {
                if (Rockets[i].encoding_scheme_connections[j][3] > 0 && 
                    Rockets[i].encoding_scheme_connections[j][4] != 0) {
                    Rockets[i].encoding_scheme_connections[j][4] = 1;
                } else {
                    Rockets[i].encoding_scheme_connections[j][4] = 0;
                }
            }

            Rockets[i].weighted_sum();
        }


        // Evolution process
        if (players_finished >= 20) {
            std::cout << std::endl << "INFO: Generation Number (" << generation << ") begins" << std::endl;

            for (int i = 0; i < species; i++) {
                std::cout << i << " score: " << Rockets[i].score << std::endl;
            } 

            // Half well performed rockets
            std::cout << "INFO: Evolution Process" << std::endl;
            std::vector<float> all_scores;
            for (int i = 0; i < species; i++) {
                all_scores.push_back(Rockets[i].score); 
            }

            for (int i = 0; i < 10; i++) {
                int highest_score = std::distance(all_scores.begin(), 
                                        std::find(all_scores.begin(), 
                                                  all_scores.end(), *std::max_element(
                                                  all_scores.begin(), all_scores.end())));
                HalfWellPerformed[i] = Rockets[highest_score];
                all_scores[highest_score] = 0;
            }

            // Better-Worse structure system 
            float new_best_score = 0;
            for (int i = 0; i < 10; i++) {
                new_best_score += HalfWellPerformed[i].score;
            }

            if (new_best_score >= 9.2) {
                json jsonfile;
                jsonfile["Connections"] = HalfWellPerformed[0].encoding_scheme_connections;
                jsonfile["Nodes"] = HalfWellPerformed[0].encoding_scheme_nodes;
                jsonfile["Point Position"] = point_position;

                std::ofstream file("tryout/EncodingSchemes.json");
                file << jsonfile;
                file.close();
                std::cout << "INFO: Done! found a 9.2 higher structure" << std::endl;
                break;
            }

            if (old_best_score > new_best_score) {
                repeated_nn += 1;
                if (repeated_nn == 20) {
                    std::cout << "INFO: Structure repeated more than 20 times, mutating weights" << std::endl;
                    for (int i = 0; i < species; i++) {
                        for (int j = 0; j < Rockets[i].encoding_scheme_connections.size(); j++) {
                            Rockets[i].encoding_scheme_connections[j][3] = float(random_weight())/10;
                        }
                    }
                    repeated_nn = 0;
                }

                std::cout << "INFO: Comparing structure scores " << old_best_score << " | " << new_best_score << std::endl;
                std::cout << "INFO: Using the old rockets..." << std::endl;
                for (int i = 0; i < species; i++) {
                    Rockets[i] = OldRockets[i];
                }

                // Start again
                for (int i = 0; i < species; i++) {
                    std::cout << i << " score: " << Rockets[i].score << std::endl;
                } 

                std::vector<float> all_scores;
                for (int i = 0; i < species; i++) {
                    all_scores.push_back(Rockets[i].score); 
                }

                for (int i = 0; i < 10; i++) {
                    int highest_score = std::distance(all_scores.begin(), 
                                            std::find(all_scores.begin(), 
                                                      all_scores.end(), *std::max_element(
                                                      all_scores.begin(), all_scores.end())));
                    HalfWellPerformed[i] = Rockets[highest_score];
                    all_scores[highest_score] = 0;
                }
            } else {
                std::cout << "INFO: Comparing structure scores " << old_best_score << " | " << new_best_score << std::endl;
                std::cout << "INFO: Using the new rockets..." << std::endl;
            }

            // Update old score
            float old_score_sum = 0;
            for (int i = 0; i < 10; i++) {
                old_score_sum += HalfWellPerformed[i].score; 
            }
            old_best_score = old_score_sum;
            
            std::cout << "INFO: Half well performed rockets" << std::endl;
            for (int i = 0; i < 10; i++) {
                std::cout << HalfWellPerformed[i].score << std::endl;
            }
            
            // Crossover Phase
            std::cout << "INFO: Crossover half well performed rockets" << std::endl;
            for (int i = 0; i < 5; i++) {
                std::vector<std::vector<std::vector<float>>> offspring_encoding_scheme_nodes = {
                    { 
                        {1, 0}, 
                        {2, 0},
                        {3, 0},
                        {4, 0},
                        {5, 0},
                        {6, 0},
                        {7, 0},
                        {8, 0}, 
                    }, 
                    { 
                        {9, 0},
                        {10, 0}
                    },
                    {} 
                };
                std::vector<std::vector<float>> offspring_encoding_scheme_connections = {};

                // Parent 1 
                std::set<int> repeated_IN;
                std::cout << "Parent 1" << std::endl;
                for (int j = 0; j < HalfWellPerformed[0+i].encoding_scheme_connections.size(); j++) {
                    offspring_encoding_scheme_connections.push_back(
                        HalfWellPerformed[0+i].encoding_scheme_connections[j]);
                    std::cout << HalfWellPerformed[0+i].encoding_scheme_connections[j][5] << std::endl;
                    repeated_IN.insert(offspring_encoding_scheme_connections[j][5]);
                }

                // Parent 2
                std::cout << "Parent 2" << std::endl;
                for (int j = 0; j < HalfWellPerformed[9-i].encoding_scheme_connections.size(); j++) {
                    std::cout << HalfWellPerformed[9-i].encoding_scheme_connections[j][5] << std::endl;
            
                    if (repeated_IN.find(HalfWellPerformed[9-i].encoding_scheme_connections[j][5]) == 
                        repeated_IN.end()) {
                        offspring_encoding_scheme_connections.push_back(
                            HalfWellPerformed[9-i].encoding_scheme_connections[j]);
                    }
                    repeated_IN.insert(offspring_encoding_scheme_connections[j][5]);
                } 

                // Creating new Scheme Nodes
                int no_hidden = 0;
                for (int j = 0; j < offspring_encoding_scheme_connections.size(); j++) {
                    for (int k = 0; k < offspring_encoding_scheme_connections.size(); k++) {
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

                // Sort & delete repeated nodes
                if (offspring_encoding_scheme_nodes[2].size() > 1) {
                    offspring_encoding_scheme_nodes[2].erase(offspring_encoding_scheme_nodes[2].begin());
                }

                std::sort(offspring_encoding_scheme_nodes[2].begin(), 
                          offspring_encoding_scheme_nodes[2].end());
                offspring_encoding_scheme_nodes[2].erase(
                    std::unique(
                        offspring_encoding_scheme_nodes[2].begin(), 
                        offspring_encoding_scheme_nodes[2].end()), 
                        offspring_encoding_scheme_nodes[2].end());

                std::cout << "Offspring " << 0+i << "-" << 9-i << std::endl;
                for (int j = 0; j < offspring_encoding_scheme_connections.size(); j++) {
                    std::cout << offspring_encoding_scheme_connections[j][5] << std::endl;
                }

                OffspringSpecies[i].encoding_scheme_connections = offspring_encoding_scheme_connections;
                OffspringSpecies[i].encoding_scheme_nodes = offspring_encoding_scheme_nodes;
            }

            // Building the next 20 species
            int n = 0;
            for (int i = 0; i < 20; i++) {
                Rockets[i].encoding_scheme_connections = OffspringSpecies[n].encoding_scheme_connections;  
                if (n == 4) {
                    n = 0;
                } else {
                    n += 1;
                }
            }

            // Save old species
            for (int i = 0; i < species; i++) {
                OldRockets[i] = Rockets[i];
            }

            // Random Mutation
            std::cout << "INFO: Random Mutation to the new generation..." << std::endl;
            for (int i = 0; i < species; i++) {
                Rockets[i].rocket_mutation();

                // Getting rid of disabled connections
                for (int j = 0; j < Rockets[i].encoding_scheme_connections.size(); j++) {
                    if (Rockets[i].encoding_scheme_connections[j][4] == 0) {
                        Rockets[i].encoding_scheme_connections.erase(
                            Rockets[i].encoding_scheme_connections.begin()+j);
                    }
                }
            }


            // Reset Parameters
            players_finished = 0;
            for (int i = 0; i < species; i++) {
                Rockets[i].finish = false;
                if (Rockets[i].encoding_scheme_connections.size() > 60){
                    Rockets[i].speed = 0.0006;
                    Rockets[i].x_speed = 0.006;
                } else {
                    Rockets[i].speed = 0.0004;
                    Rockets[i].x_speed = 0.004;
                }
                Rockets[i].score = 0.0;
            }

            generation += 1;
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
            all_scores.push_back(Rockets[i].score); 
        }
        int highest_score = std::distance(all_scores.begin(), 
                            std::find(all_scores.begin(), all_scores.end(), *std::max_element(
                                      all_scores.begin(), all_scores.end())));
        Rockets[highest_score].draw_nn();

        glfwSwapBuffers(nn_window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
