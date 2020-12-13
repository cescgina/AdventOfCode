#include <map>
#include <set>
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
    std::string source, dest;
    input_file.open(file_name);
    for (std::string line; std::getline(input_file, line);){
        auto pos = line.find(")");
        std::string start = line.substr(0, pos);
        std::string end = line.substr(pos+1);
        if (end == "SAN") dest = start;
        if (end == "YOU") source = start;
        if (orbits_graph.find(start) != orbits_graph.end()){
            orbits_graph[start].push_back(end);
        }
        else{
            orbits_graph[start] = {end};
        }
        if (orbits_graph.find(end) != orbits_graph.end()){
            orbits_graph[end].push_back(start);
        }
        else{
            orbits_graph[end] = {start};
        }
    }
    input_file.close();
    std::set<std::string> visited;
    std::map<std::string, int> distances;
    std::priority_queue<std::pair<int, std::string>> stars;
    for (auto& element: orbits_graph){
        if (element.first == source){
            distances[source] = 0;
            stars.push(std::make_pair(0, source));
        }
        else{
            distances[element.first] = 10000000;
        }
    }
    while(not stars.empty()){
        auto element = stars.top();
        int distance = element.first;
        std::string center = element.second;
        if (center == dest){
            std::cout << "Minimum distance is " << distance << std::endl;
            return 0;
        }
        visited.insert(center);
        stars.pop();
        for (auto& child: orbits_graph[center]){
            if (visited.find(child) != visited.end()) continue;
            if ((distance+1) < distances[child]){
                distances[child] = distance+1;
                stars.push(std::make_pair(distance+1, child));
            }
        }
    }
}
