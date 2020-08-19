#include <map>
#include <set>
#include <queue>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <iostream>
#include <unistd.h>

typedef std::pair<int, int> coords;
typedef std::map<coords, int> grid;
int PENALTY = 100000;

bool is_open(const coords& pos, const int& fav_num){
    long long x = pos.first, y = pos.second; 
    long long val = std::pow(x+y, 2)+3*x+fav_num+y;
    int n_bits = __builtin_popcountll(val);
    return !(n_bits % 2);
}

void process_position(grid& grid_pos, const coords pos, const int& num, std::queue<coords>& new_moves){
    if (pos.first > 0){
        coords next = coords(pos);
        next.first--;
        if(is_open(next, num)){
            if (grid_pos.find(next) == grid_pos.end()) grid_pos[next] = PENALTY;
            grid_pos[next] = std::min(grid_pos[next], grid_pos[pos]+1);
            new_moves.push(next);
        }
    }
    if (pos.second > 0){
        coords next = coords(pos);
        next.second--;
        if(is_open(next, num)){
            if (grid_pos.find(next) == grid_pos.end()) grid_pos[next] = PENALTY;
            grid_pos[next] = std::min(grid_pos[next], grid_pos[pos]+1);
            new_moves.push(next);
        }
    }
    coords next = coords(pos);
    next.second++;
    if(is_open(next, num)){
        if (grid_pos.find(next) == grid_pos.end()) grid_pos[next] = PENALTY;
        grid_pos[next] = std::min(grid_pos[next], grid_pos[pos]+1);
        new_moves.push(next);
    }
    next.second--;
    next.first++;
    if(is_open(next, num)){
        if (grid_pos.find(next) == grid_pos.end()) grid_pos[next] = PENALTY;
        grid_pos[next] = std::min(grid_pos[next], grid_pos[pos]+1);
        new_moves.push(next);
    }
}

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::vector<std::string> parsed_line;
    std::getline(input_file, line);
    std::set<coords> visited;
    int number = std::stoi(line), final_x, final_y;
    std::getline(input_file, line);
    auto pos = line.find(",");
    final_x = std::stoi(line.substr(0, pos));
    final_y = std::stoi(line.substr(pos+1));
    coords initial = std::make_pair(1, 1);
    coords current = std::make_pair(final_x, final_y);
    std::queue<coords> pending;
    pending.push(current);
    grid distances;
    distances[current] = 0;
    while (current != initial){
       current = pending.front();
       pending.pop();
       if (visited.find(current) != visited.end()) continue;
       else visited.insert(current);
       process_position(distances, current, number, pending);
    }
    std::cout << "Minimum number of steps is " << distances[initial] << std::endl;
}
