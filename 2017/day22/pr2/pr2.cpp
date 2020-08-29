#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <iostream>
#include <algorithm>
#include <unistd.h>

void print_state(std::map<std::pair<int, int>, bool>& state ){
    for (int i=-4; i<5; i++){
        if (i > -1) std::cout << " ";
        std::cout << i << " ";
        for (int j=-4; j<5; j++){
            if (state[std::make_pair(i, j)]) std::cout << '#';
            else std::cout << '.';
        }
        std::cout << std::endl;
    }
}

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::vector<std::vector<int>> initial;
    for(std::string line; std::getline(input_file, line);){
        std::vector<int> row;
        std::transform(line.begin(), line.end(), std::back_inserter(row), [](unsigned char c) -> int { return 2*(c == '#'); });
        initial.push_back(row);
    }
    input_file.close();
    int n = initial.size();
    int middle = n/2;
    std::map<std::pair<int, int>, int> grid;
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            grid[std::make_pair(i-middle, j-middle)] = initial[i][j];
        }
    }
    std::map<char, char> left_turn, right_turn, reverse;
    left_turn['n'] = 'w';
    left_turn['w'] = 's';
    left_turn['s'] = 'e';
    left_turn['e'] = 'n';
    right_turn['n'] = 'e';
    right_turn['e'] = 's';
    right_turn['s'] = 'w';
    right_turn['w'] = 'n';
    reverse['n'] = 's';
    reverse['e'] = 'w';
    reverse['s'] = 'n';
    reverse['w'] = 'e';
    char direction = 'n';
    std::map<char, std::pair<int, int>> move;
    move['n'] = std::make_pair(-1, 0);
    move['w'] = std::make_pair(0, -1);
    move['s'] = std::make_pair(1, 0);
    move['e'] = std::make_pair(0, 1);
    int x = 0, y = 0, infections = 0;
    for (int turns = 0; turns < 10000000; turns++){
        auto coords = std::make_pair(x, y);
        if (grid[coords] == 0){
            direction = left_turn[direction];
        }
        else if (grid[coords] == 1){
            infections++;
        }
        else if (grid[coords] == 2){
            direction = right_turn[direction];
        }
        else if (grid[coords] == 3){
            direction = reverse[direction];
        }
        grid[coords]++;
        grid[coords] %= 4;
        auto change = move[direction];
        x += change.first;
        y += change.second;
    }
    std::cout << infections << std::endl;
}
