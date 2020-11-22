#include <stack>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <unistd.h>
#include <boost/functional/hash.hpp>

typedef std::unordered_map<std::pair<int, int>, int, boost::hash<std::pair<int, int>>> map_rooms;
typedef std::stack<std::tuple<int, int, int>> branches_queue;

int count_doors(map_rooms& doors_map){
    int n_doors = 0;
    for (auto& el: doors_map){
        n_doors += (el.second >= 1000);
    }
    return n_doors;
}

int parse_regex_nonrecursive(std::string& regex, map_rooms& rooms){
    branches_queue branches;
    int x = 0 , y = 0, dist = 0;
    size_t index = 1;
    while(index < regex.size()-1){
        char input = regex[index];
        if (input == '('){
            branches.push(std::make_tuple(dist, x, y));
        }
        else if (input == ')'){
            dist = std::get<0>(branches.top());
            x = std::get<1>(branches.top());
            y = std::get<2>(branches.top());
            branches.pop();
        }
        else if (input == '|'){
            dist = std::get<0>(branches.top());
            x = std::get<1>(branches.top());
            y = std::get<2>(branches.top());
        }
        else {
            x += (input == 'E') - (input == 'W');
            y += (input == 'S') - (input == 'N');
            dist++;
            auto coords = std::make_pair(y, x);
            if (rooms.find(coords) != rooms.end()){
                rooms[coords] = std::min(rooms[coords], dist);
            }
            else{
                rooms[coords] = dist;
            }
        }
        index++;
    }
    return count_doors(rooms);
}

void print_map(map_rooms& rooms){
    int x_max = 0, y_max = 0, y_min = 0, x_min = 0;
    for (auto& el: rooms){
        int y = el.first.first;
        int x = el.first.second;
        x_max = std::max(x_max, x);
        y_max = std::max(y_max, y);
        x_min = std::min(x_min, x);
        y_min = std::min(y_min, y);
    }
    for (int i = y_min-1; i <= y_max+1; i++){
        for (int j = x_min-1; j <= x_max+1; j++){
            auto coords = std::make_pair(i, j);
            if (rooms.find(coords) != rooms.end()){
                std::cout << rooms[coords];
            }
            else{
                std::cout << '#';
            }
        }
        std::cout << std::endl;

    }
}

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    std::unordered_map<std::string, int> opcodes;
    input_file.open(file_name);
    std::getline(input_file, line);
    input_file.close();
    map_rooms rooms;
    rooms[std::make_pair(0, 0)] = 'X';
    int doors = parse_regex_nonrecursive(line, rooms);
    std::cout << "Number of doors is " << doors << std::endl;
}
