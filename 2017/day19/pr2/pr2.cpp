#include <map>
#include <queue>
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <iostream>
#include <unistd.h>

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    std::map<std::pair<int, int>, char> grid;
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::getline(input_file, line);
    int i = 1, y = 0, x = 0;
    for (auto c: line){
        if (c != ' '){
            grid[(std::make_pair(0, y))] = c;
            break;
        }
        y++;
    }
    for(std::string line; std::getline(input_file, line);){
        int j = 0;
        for (auto c: line){
            if (c != ' '){
                grid[(std::make_pair(i, j))] = c;
            }
            j++;
        }
        i++;

    }
    input_file.close();
    std::vector<std::pair<int, int>> moves = {std::make_pair(0, 1), std::make_pair(0, -1), std::make_pair(1, 0), std::make_pair(-1, 0)};
    bool has_move = false;
    int prev_x = 0, prev_y = 0, direction = 3, count = 0;
    while (true){
        // std::cout << x << "," << y << std::endl;
        count++;
        has_move = false;
        auto move = moves[direction];
        std::pair<int, int> coords = std::make_pair(x+move.first, y+move.second);
        // try to keep going in the same directions first
        if (grid.find(coords) != grid.end()){
            has_move = true;
            prev_x = x;
            prev_y = y;
            x += move.first;
            y += move.second;
            continue;
        }
        direction = 0;
        for (auto move: moves){
            std::pair<int, int> coords = std::make_pair(x+move.first, y+move.second);
            if (x+move.first == prev_x and y+move.second == prev_y){
                direction++;
                continue;
            }
            if (grid.find(coords) != grid.end()){
                has_move = true;
                prev_x = x;
                prev_y = y;
                x += move.first;
                y += move.second;
                break;
            }
            direction++;
        }
        if (!has_move) break;
    }
    std::cout << count << std::endl;
}
