#include <set>
#include <map>
#include <queue>
#include <vector>
#include <fstream>
#include <utility>
#include <iostream>
#include <algorithm>
#include <unistd.h>

typedef std::set<std::pair<int, int>> nodes;
typedef std::tuple<int, nodes, int, int, int> info;

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    nodes all_nodes;
    std::ifstream input_file;
    input_file.open(file_name);
    for (std::string line; std::getline(input_file, line);){
        auto pos = line.find("/");
        all_nodes.insert(std::make_pair(std::stoi(line.substr(0, pos)), std::stoi(line.substr(pos+1))));
    }
    input_file.close();

    size_t strength = 0, max_strength = 0, max_len = 0, len = 0;
    int x = 0, y = 0;
    auto cmp = [](info left, info right) {return std::get<0>(left) < std::get<0>(right);};
    std::priority_queue<info, std::vector<info>, decltype(cmp)> paths(cmp);
    nodes visited;
    paths.push(std::make_tuple(len, visited, x, y, strength));
    info element;
    while (not paths.empty()){
        element = paths.top();
        paths.pop();
        len = std::get<0>(element);
        visited = std::get<1>(element);
        x = std::get<2>(element);
        y = std::get<3>(element);
        strength = std::get<4>(element);
        bool can_extend = false;
        for (auto it=all_nodes.begin(); it != all_nodes.end(); it++){
            if (visited.find(*it) != visited.end()) continue;
            if (y == it->first or y == it->second){
                nodes new_visited(visited);
                new_visited.insert(*it);
                can_extend = true;
                int new_x, new_y;
                if (y == it->first){
                    new_y = it->second;
                    new_x = it->first;
                }
                else{
                    new_x = it->second;
                    new_y = it->first;
                }
                paths.push(std::make_tuple(len+1, new_visited, new_x, new_y, strength+it->first+it->second));
            }
        }
        if (!can_extend){
            if (len > max_len){
                max_len = len;
                max_strength = strength;
            }
            else if (len == max_len){
                max_strength = std::max(max_strength, strength);
            }
        }
    }
    std::cout << max_strength << std::endl;
}
