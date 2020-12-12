#include <map>
#include <vector>
#include <numeric>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

typedef std::map<std::pair<int, int>, std::vector<int>> positions;
typedef std::vector<std::string> to_parse;

void parse_wire(const to_parse& wire, positions& grid, const int id, int& x, int& y){
    char direction;
    int length = 0, dx = 0, dy = 0, step = 0;
    for (auto& move: wire){
        direction = move[0];
        length = std::stoi(move.substr(1));
        switch(direction){
            case 'R':
                dx = 1;
                dy = 0;
                break;
            case 'L':
                dx = -1;
                dy = 0;
                break;
            case 'U':
                dx = 0;
                dy = -1;
                break;
            case 'D':
                dx = 0;
                dy = 1;
                break;
        }
        for (int i = 1; i<=length; i++){
            step++;
            x += dx;
            y += dy;
            auto coords = std::make_pair(y, x);
            if (grid.find(coords) != grid.end()){
                if (grid[coords][id] == 0){
                    // if its not a wire wrapping into itself
                    grid[coords][id] = step;
                }
            }
            else{
                grid[coords] = {0,0,0};
                grid[coords][id] = step;
            }
        }
    }
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    positions grid;
    int id = 1, x = 0, y= 0;
    std::string line; 
    std::getline(input_file, line);
    to_parse wire;
    boost::split(wire, line, boost::is_any_of(", "), boost::token_compress_on);
    parse_wire(wire, grid, id, x, y);
    x = 0;
    y = 0;
    id = 2;
    wire.clear();
    std::getline(input_file, line);
    boost::split(wire, line, boost::is_any_of(", "), boost::token_compress_on);
    parse_wire(wire, grid, id, x, y);
    int min_distance = 1000000000;
    for (auto& element: grid){
        if (element.second[1]*element.second[2] > 0){
            int distance = std::accumulate(element.second.begin(), element.second.end(), 0);
            min_distance = std::min(min_distance, distance);
        }
    }
    std::cout << "Minimum distance is " << min_distance << std::endl;
}
