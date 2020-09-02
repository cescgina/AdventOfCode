#include <set>
#include <map>
#include <queue>
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <iostream>
#include <unistd.h>

bool can_turn(int x_coor, int y_coor, int dir, std::map<std::pair<int, int>, char>& road_grid, const std::vector<std::pair<int, int>> pos_moves, int orig_dir){
    auto move = pos_moves[orig_dir];
    move = pos_moves[dir];
    x_coor += move.second;
    y_coor += move.first;
    auto pos = std::make_pair(x_coor, y_coor);
    if (road_grid.find(pos) == road_grid.end()) return false;
    if (orig_dir % 2 != 0){
        return road_grid[pos] != '|';
    }  
    else{
        return road_grid[pos] != '-';
    }
}

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    std::map<std::pair<int, int>, char> grid;
    //y, x, direction, instersection_action
    std::set<std::vector<int>> carts;
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    int i = 0, cart_num = 0;
    for(std::string line; std::getline(input_file, line);){
        int j = 0;
        for (auto c: line){
            if (c == '^'){
                grid[(std::make_pair(j, i))] = '|';
                carts.insert({i, j, 3, 0, cart_num});
                cart_num++;
            }
            else if (c == '<'){
                grid[(std::make_pair(j, i))] = '-';
                carts.insert({i, j, 2, 0, cart_num});
                cart_num++;
            }
            else if (c == 'v'){
                grid[(std::make_pair(j, i))] = '|';
                carts.insert({i, j, 1, 0, cart_num});
                cart_num++;
            }
            else if (c == '>'){
                grid[(std::make_pair(j, i))] = '-';
                carts.insert({i, j, 0, 0, cart_num});
                cart_num++;
            }
            else if (c != ' '){
                grid[(std::make_pair(j, i))] = c;
            }
            j++;
        }
        i++;
    }
    input_file.close();
    std::vector<std::pair<int, int>> moves = {std::make_pair(0, 1), std::make_pair(1, 0), std::make_pair(0, -1), std::make_pair(-1, 0)};
    int x = 0, y = 0, direction = 0, turns = 0;
    int ticks = 1;
    while (true){
        std::set<std::vector<int>> new_carts;
        while (!carts.empty()){
            auto it = carts.begin();
            y = (*it)[0];
            x = (*it)[1];
            direction = (*it)[2];
            turns = (*it)[3];
            cart_num = (*it)[4];
            carts.erase(it);
            char road = grid[std::make_pair(x, y)];
            if (road == '+'){
                if (turns == 0){
                    direction = direction == 0 ? 3: direction-1;
                }
                else if (turns == 2){
                    direction = (direction+1) % 4;
                }
                turns++;
                turns %= 3;
            }
            else if (road == '/' or road == '\\'){
                // try right
                if (can_turn(x, y, (direction+1)%4, grid, moves, direction)){
                    direction = (direction+1) % 4;
                }
                else {
                    direction = direction == 0 ? 3: direction-1;
                }
            }
            y += moves[direction].first;
            x += moves[direction].second;
            for (auto it=carts.begin(); it != carts.end(); it++){
                if ((*it)[0] == y and (*it)[1] == x){
                    std::cout << x << "," << y << std::endl;
                    return 0;
                }
            }
            for (auto it=new_carts.begin(); it != new_carts.end(); it++){
                if ((*it)[0] == y and (*it)[1] == x){
                    std::cout << x << "," << y << std::endl;
                    return 0;
                }
            }
            new_carts.insert({y, x, direction, turns, cart_num});
        }
        std::swap(carts, new_carts);
        ticks++;
    }
}
