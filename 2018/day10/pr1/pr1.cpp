#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <iostream>
#include <unordered_map>
#include <unistd.h>

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::unordered_map<int, std::pair<int, int>> positions, velocities;
    int index = 0;
    for (std::string line; std::getline(input_file, line);){
        auto pos_comma = line.find(",");
        int x = std::stoi(line.substr(10, pos_comma-10));
        auto pos_close = line.find(">", pos_comma+1);
        int y = std::stoi(line.substr(pos_comma+2, pos_close-pos_comma-2));
        pos_comma = line.find(",", pos_close+10);
        int vx = std::stoi(line.substr(pos_close+12, pos_comma-pos_close-12));
        pos_close = line.find(">", pos_comma+1);
        int vy = std::stoi(line.substr(pos_comma+2, pos_close-pos_comma-2));
        positions[index] = std::make_pair(x, y);
        velocities[index] = std::make_pair(vx, vy);
        index++;
    }
    int time = 0, min_dist = 1000000000, min_x = 1000000000, min_y = 1000000000, max_x = 0, max_y = 0;
    std::map<std::pair<int, int>, int> message;
    std::tuple<int, int, int, int> limits;
    while (true){
        min_x = 1000000000, min_y = 1000000000, max_x = 0, max_y = 0;
        for (auto it=positions.begin(); it != positions.end(); it++){
            auto pos = it->second;
            auto speed = velocities[it->first];
            it->second = std::make_pair(pos.first+speed.first, pos.second+speed.second);
            min_x = std::min(min_x, it->second.first);
            max_x = std::max(max_x, it->second.first);
            min_y = std::min(min_y, it->second.second);
            max_y = std::max(max_y, it->second.second);
        }
        time++;
        int dist = (max_x-min_x)+(max_y-min_y);
        if (dist < min_dist){
            min_dist = dist;
            std::map<std::pair<int, int>, int> grid;
            for (auto it=positions.begin(); it != positions.end(); it++){
                grid[it->second] = it->first;
            }
            message = grid;
            limits = std::make_tuple(min_x, min_y, max_x, max_y);
        }
        else if (dist > min_dist+10){
            break;
        }
    }
    min_x = std::get<0>(limits);
    min_y = std::get<1>(limits);
    max_x = std::get<2>(limits);
    max_y = std::get<3>(limits);
    for (int j=min_y; j<=max_y; j++){
        for (int i=min_x; i<=max_x; i++){
            if (message.find(std::make_pair(i, j)) == message.end()){
                std::cout << " ";
            }
            else{
                std::cout << '#';
            }
        }
        std::cout << std::endl;
    }
}
