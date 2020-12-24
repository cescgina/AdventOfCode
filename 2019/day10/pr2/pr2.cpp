#include <set>
#include <map>
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

int calculate_distance(std::pair<int, int>& ast1, std::pair<int, int>& ast2){
    return std::abs(ast2.first-ast1.first)+std::abs(ast2.second-ast1.second);

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
    std::pair<int, int> station;
    for (auto& asteroid: asteroids){
        std::set<double> angles;
        for (auto& asteroid2: asteroids){
            if (asteroid == asteroid2) continue;
            double angle = calculate_angle(asteroid, asteroid2);
            if (angles.find(angle) == angles.end()) angles.insert(angle);
        }
        if (angles.size() > max_asteroids){
            max_asteroids = angles.size();
            station = asteroid;
        }
    }
    std::map<double, std::set<std::tuple<int, int, int>>> asteroids_rm;
    for (auto& asteroid2: asteroids){
        if (station == asteroid2) continue;
        double angle = calculate_angle(station, asteroid2)+M_PI_2;
        if (angle < 0){
            angle += 2*M_PI;
        }
        if (asteroids_rm.find(angle) == asteroids_rm.end()) {
            asteroids_rm[angle] = {std::make_tuple(calculate_distance(station, asteroid2), asteroid2.first, asteroid2.second)};
        }
        else{
            asteroids_rm[angle].insert(std::make_tuple(calculate_distance(station, asteroid2), asteroid2.first, asteroid2.second));
        }
    }
    int removed_asteroids = 0, x = 0, y = 0;
    while (true){
        for (auto& elements: asteroids_rm){
            if (not elements.second.empty()){
                x = std::get<1>(*elements.second.begin());
                y = std::get<2>(*elements.second.begin());
                elements.second.erase(elements.second.begin());
                removed_asteroids++;
                if (removed_asteroids == 200){
                    std::cout << "Coordinates are " << x << "," << y << " value " << x*100+y << std::endl;
                    return 0;
                }
            }
        }

    }

}
