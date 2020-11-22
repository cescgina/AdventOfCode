#include <stack>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unistd.h>
#include <boost/functional/hash.hpp>

typedef std::unordered_map<std::pair<int, int>, char, boost::hash<std::pair<int, int>>> map_rooms;
typedef std::unordered_map<std::pair<int, int>, int, boost::hash<std::pair<int, int>>> distances;
typedef std::stack<std::tuple<size_t, int, int, int, int>> branches_queue;

int max_doors(distances& doors_map){
    int n_doors = 0;
    for (auto& el: doors_map){
        n_doors = std::max(n_doors, el.second);
    }
    return n_doors;
}

bool find_branches(int x, int y, branches_queue& branches, size_t index, int parens, std::string& regex, int doors){
    int orig_parens = parens;
    std::vector<std::pair<size_t, int>> indices = {std::make_pair(index, parens)};
    while(index < regex.size()-1){
        char input = regex[index];
        if (input == '(') parens++;
        else if (input == ')') {
            parens--;
            if (parens < orig_parens){
                break;
            }
        }
        else if (input == '|' and (parens == orig_parens)){
            if (regex[index+1] == ')') return true;
            indices.push_back(std::make_pair(index+1, parens));
        }
        index++;
    }
    for (auto l: indices){
        branches.push(std::make_tuple(l.first, x, y, l.second, doors));
    }
    return false;
}

int dist(int x, int y){
    return std::abs(x)+std::abs(y);
}

int parse_regex_nonrecursive(std::string& regex, map_rooms& rooms){
    bool ignore = false;
    branches_queue branches;
    branches.push(std::make_tuple(1, 0, 0, 0, 0));
    distances dists;
    int x, y, orig_parens, parens;
    size_t index;
    int doors = 0;
    while (not branches.empty()){
        index = std::get<0>(branches.top());
        x = std::get<1>(branches.top());
        y = std::get<2>(branches.top());
        parens = std::get<3>(branches.top());
        doors = std::get<4>(branches.top());
        branches.pop();
        orig_parens = parens;
        while(index < regex.size()-1){
            char input = regex[index];
            if (ignore){
                if (input == '(') parens++;
                else if (input == ')') {
                    parens--;
                    if (parens <= orig_parens){
                        ignore = false;
                    }
                }
                index++;
                continue;
            }
            else{
                if (input == '('){
                    parens++;
                    bool loops = find_branches(x, y, branches, index+1, parens, regex, doors);
                    if (loops) {
                        ignore = true;
                    }
                    else{
                        break;
                    }
                    index++;
                }
                else if (input == ')'){
                    parens--;
                    if (parens < 0){
                        std::cout << "There is some problem parsing parentesis in index " << index << " original parens " << orig_parens << " " << parens << std::endl;
                    }
                    else if (parens <= orig_parens){
                        ignore = false;
                    }
                    index++;
                }
                else if (input == '|'){
                    ignore = true;
                    index++;
                }
                else if (input == 'N'){
                    y--;
                    rooms[std::make_pair(y, x)] = '-';
                    y--;
                    rooms[std::make_pair(y, x)] = '.';
                    index++;
                    doors++;
                }
                else if (input == 'W'){
                    x--;
                    rooms[std::make_pair(y, x)] = '|';
                    x--;
                    rooms[std::make_pair(y, x)] = '.';
                    index++;
                    doors++;
                }
                else if (input == 'E'){
                    x++;
                    rooms[std::make_pair(y, x)] = '|';
                    x++;
                    rooms[std::make_pair(y, x)] = '.';
                    index++;
                    doors++;
                }
                else if (input == 'S'){
                    y++;
                    rooms[std::make_pair(y, x)] = '-';
                    y++;
                    rooms[std::make_pair(y, x)] = '.';
                    index++;
                    doors++;
                }
                else{
                    std::cout << "Something wierd found in regex " << input << std::endl;
                }
            }
        }
        auto coords = std::make_pair(y, x);
        if (dists.find(coords) != dists.end()){
            dists[coords] = std::min(dists[coords], doors);
        }
        else {
            dists[coords] = doors;
        }
    }
    return max_doors(dists);
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
    /*
     * For this code to work, the branches that contain cycles need to be
     * removed, I used the following vim command
     * :%s/(\w*|)//g
    */
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
    //print_map(rooms);
    std::cout << "Number of doors is " << doors << std::endl;
}
