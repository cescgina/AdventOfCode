#include <set>
#include <cmath>
#include <vector>
#include <string>
#include <numeric>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>


bool DEBUG = false;

int64_t gcd(int64_t a, int64_t b)
{
    return (b == 0) ? a : gcd(b, a % b);
}

int64_t lcm(int64_t a, int64_t b)
{
    return (a == 0 || b == 0) ? 0: ((a / gcd(a, b))*b);
}

int calculate_energy(const std::vector<std::vector<int>>& coords, const std::vector<std::vector<int>>& speed){
    int energy = 0;
    for (size_t i = 0; i < coords.size(); i++){
        int pot = 0, kin = 0;
        for (size_t k = 0; k < coords[i].size(); k++){
            pot += std::abs(coords[i][k]);
            kin += std::abs(speed[i][k]);
        }
        energy += pot*kin;
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

std::string hash(const std::vector<std::vector<int>>& coords, const std::vector<std::vector<int>>& speed, const size_t& i){
    std::ostringstream out;
    for (size_t ii = 0; ii < coords.size(); ii++){
        out << "pos=<" << coords[ii][i] << ">, vel=<";
        out << speed[ii][i] << ">" << std::endl;
    }
    return out.str();
}

int64_t find_lcm(const std::vector<int64_t>& numbers){
    return lcm(lcm(numbers[0], numbers[1]), numbers[2]);
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
    std::vector<int64_t> steps_dim;
    for (size_t dim = 0; dim < 3; dim++){
        int64_t steps = 0;
        std::set<std::string> visited;
        while (true){
            std::string state = hash(coordinates, velocities, dim);
            if (visited.find(state) != visited.end()){
                std::cout << "Found repetition in dimension " << dim << " after " << steps << " steps" << std::endl;
                steps_dim.push_back(steps);
                break;
            }
            visited.insert(state);
            for (size_t i = 0; i < coordinates.size(); i++){
                for (size_t j = i+1; j < coordinates.size(); j++){
                    if (coordinates[i][dim] < coordinates[j][dim]){
                        velocities[i][dim]++;
                        velocities[j][dim]--;
                    }
                    else if (coordinates[i][dim] > coordinates[j][dim]){
                        velocities[j][dim]++;
                        velocities[i][dim]--;
                    }
                }
            }
            for (size_t i = 0; i < coordinates.size(); i++){
                coordinates[i][dim] += velocities[i][dim];
            }
            if (DEBUG){
                std::cout << "After " << steps+1 << " steps: "<< std::endl;
                print_state(coordinates, velocities);
            }
            steps++;
        }
    }
    std::cout << "Total steps is " << find_lcm(steps_dim) << std::endl;
}
