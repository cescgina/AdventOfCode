#include <set>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>

double calculate_angle(std::pair<int, int>& ast1, std::pair<int, int>& ast2){
    double dy = (double)ast2.second-(double)ast1.second;
    double dx = (double)ast2.first-(double)ast1.first;
    return std::atan2(dy, dx);
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::vector<std::pair<int, int>> asteroids;
    int i = 0;
    for (std::string line; std::getline(input_file, line);){
        for (int j=0; j<(int)line.size(); j++){
            if (line[j] == '#') asteroids.push_back(std::make_pair(j,i));
        }
        i++;
    }
    size_t max_asteroids = 0;
    for (auto& asteroid: asteroids){
        std::set<double> angles;
        for (auto& asteroid2: asteroids){
            if (asteroid == asteroid2) continue;
            double angle = calculate_angle(asteroid, asteroid2);
            if (angles.find(angle) == angles.end()) angles.insert(angle);
        }
        max_asteroids = std::max(max_asteroids, angles.size());
    }
    std::cout << "Maximum number of asteroids " << max_asteroids << std::endl;
}
