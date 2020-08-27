#include <map>
#include <queue>
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

long long calculate_distance(std::string& particle, const int& t, const int& acc_factor){
    long long dist = 0;
    boost::erase_all(particle, "p");
    boost::erase_all(particle, "v");
    boost::erase_all(particle, "a");
    boost::erase_all(particle, "<");
    boost::erase_all(particle, ">");
    boost::erase_all(particle, "=");
    std::vector<std::string> parsed;
    boost::split(parsed, particle, boost::is_any_of(", "), boost::token_compress_on);
    dist += std::abs(std::stoi(parsed[0])+std::stoi(parsed[3])*t+acc_factor*std::stoi(parsed[6]));
    dist += std::abs(std::stoi(parsed[1])+std::stoi(parsed[4])*t+acc_factor*std::stoi(parsed[7]));
    dist += std::abs(std::stoi(parsed[2])+std::stoi(parsed[5])*t+acc_factor*std::stoi(parsed[8]));
    return dist;
}

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    std::map<std::pair<int, int>, char> grid;
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::getline(input_file, line);
    int i = 0, min_val = 1000000000, time = 10000, min_part = -1;
    int accel_factor = (time*(time+1))/2;
    for(std::string line; std::getline(input_file, line);){
        long long part_dist = calculate_distance(line, time, accel_factor);
        if (part_dist < min_val){
            min_val = part_dist;
            min_part = i;
        }
        i++;
    }
    input_file.close();
    std::cout << min_part+1 << std::endl;
}
