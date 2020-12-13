#include <map>
#include <queue>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>

typedef std::map<std::string, std::vector<std::string>> orbits;

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    orbits orbits_graph;
    input_file.open(file_name);
    for (std::string line; std::getline(input_file, line);){
        auto pos = line.find(")");
        std::string start = line.substr(0, pos);
        std::string end = line.substr(pos+1);
        if (orbits_graph.find(start) != orbits_graph.end()){
            orbits_graph[start].push_back(end);
        }
        else{
            orbits_graph[start] = {end};
        }
    }
    input_file.close();
    int total_orbits = 0;
    std::queue<std::pair<std::string, int>> stars;
    stars.push(std::make_pair("COM", 0));
    while(not stars.empty()){
        auto element = stars.front();
        int depth = element.second;
        std::string center = element.first;
        stars.pop();
        for (auto& child: orbits_graph[center]){
            total_orbits += depth+1;
            stars.push(std::make_pair(child, depth+1));
        }
    }
    std::cout << "Total orbits " << total_orbits << std::endl;
}
