#include <set>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

bool DEBUG = false;
int calculate_energy(const std::vector<std::vector<int>>& coords, const std::vector<std::vector<int>>& speed){
    int energy = 0;
    for (size_t i = 0; i < coords.size(); i++){
        int pot = 0, kin = 0;
        for (size_t k = 0; k < coords[i].size(); k++){
            pot += std::abs(coords[i][k]);
            kin += std::abs(speed[i][k]);
        }
        energy = pot*kin;
    }
    return energy;
}

void print_state(const std::vector<std::vector<int>>& coords, const std::vector<std::vector<int>>& speed){
    for (size_t i = 0; i < coords.size(); i++){
        std::cout << "pos=<x=" << coords[i][0]<<", y=" << coords[i][1] << ", z=" << coords[i][2] << ">, vel=<x=";
        std::cout << speed[i][0]<<", y=" << speed[i][1] << ", z=" << speed[i][2] << ">" << std::endl;
    }
    std::cout << std::endl;
}
int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::vector<std::vector<int>> coordinates;
    std::vector<std::vector<int>> velocities;
    for (std::string line; std::getline(input_file, line);){
        line = line.substr(1, line.size()-2);
        std::vector<std::string> parsed;
        boost::split(parsed, line, boost::is_any_of(", "), boost::token_compress_on);
        std::vector<int> moon = {0, 0, 0};
        for (size_t i=0; i<parsed.size(); i++){
            auto pos = parsed[i].find("=");
            moon[i] = std::stoi(parsed[i].substr(pos+1));
        }
        coordinates.push_back(moon);
        velocities.push_back({0, 0, 0});
    }
    int max_steps = 1000;
    for (int steps = 0; steps < max_steps; steps++){
        for (size_t i = 0; i < coordinates.size(); i++){
            for (size_t j = i+1; j < coordinates.size(); j++){
                for (size_t k = 0; k < coordinates[i].size(); k++){
                    if (coordinates[i][k] < coordinates[j][k]){
                        velocities[i][k]++;
                        velocities[j][k]--;
                    }
                    else if (coordinates[i][k] > coordinates[j][k]){
                        velocities[j][k]++;
                        velocities[i][k]--;
                    }
                }
            }
        }
        for (size_t i = 0; i < coordinates.size(); i++){
            for (size_t k = 0; k < coordinates[i].size(); k++){
                coordinates[i][k] += velocities[i][k];
            }
        }
        if (DEBUG){
            std::cout << "After " << steps+1 << " steps: "<< std::endl;
            print_state(coordinates, velocities);
        }
    }
    std::cout << "Total energy after " << max_steps << " steps is " << calculate_energy(coordinates, velocities) << std::endl;
}
