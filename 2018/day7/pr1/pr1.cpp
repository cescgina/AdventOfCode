#include <set>
#include <queue>
#include <string>
#include <fstream>
#include <iostream>
#include <functional>
#include <unordered_map>
#include <unistd.h>

bool is_ready(const std::set<char>& possible, const std::set<char>& explored){
    for (auto it = possible.begin(); it != possible.end(); it++){
        if (explored.find(*it) == explored.end()){
            return false;
        }
    }
    return true;
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::unordered_map<char, std::set<char>> graph;
    std::unordered_map<char, std::set<char>> reverse_graph;
    std::set<char> nodes;
    for (std::string line; std::getline(input_file, line);){
        char src = line[5];
        char dest = line[36];
        graph[src].insert(dest);
        reverse_graph[dest].insert(src);
        nodes.insert(src);
        nodes.insert(dest);
    }
    std::priority_queue<char, std::vector<char>, std::greater<char>> to_explore;
    for (auto it=nodes.begin(); it != nodes.end(); it++){
        if (reverse_graph[*it].empty()){
            to_explore.push(*it);
        }

    }
    std::set<char> visited;
    while(!to_explore.empty()){
        char node = to_explore.top();
        to_explore.pop();
        std::cout << node;
        visited.insert(node);
        for (char child: graph[node]){
            if (is_ready(reverse_graph[child], visited)){
                to_explore.push(child);
            }
        }
    }
    std::cout << std::endl;
}
