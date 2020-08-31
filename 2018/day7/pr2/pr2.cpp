#include <set>
#include <queue>
#include <string>
#include <numeric>
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
    int time_offset = 60, elves = 5;
    if (file_name == "data/test.txt"){
        time_offset = 0;
        elves = 2;
    }
    std::vector<int> times(elves, 0);
    std::vector<char> work(elves, ' ');
    std::priority_queue<char, std::vector<char>, std::greater<char>> to_explore;
    for (auto it=nodes.begin(); it != nodes.end(); it++){
        if (reverse_graph[*it].empty()){
            to_explore.push(*it);
        }

    }
    int time = 0;
    std::set<char> visited;
    while(true){
        for (int i=0; i<elves; i++){
            times[i] = std::max(0, times[i]-1);
            if (times[i] > 0) {
                continue;
            }            
            if (work[i] != ' '){
                visited.insert(work[i]);
                for (char child: graph[work[i]]){
                    if (is_ready(reverse_graph[child], visited)){
                        to_explore.push(child);
                    }
                }
            }
            if(to_explore.empty()) {
                work[i] = ' ';
                times[i] = 0;
                continue;
            }
            char node = to_explore.top();
            to_explore.pop();
            times[i] = time_offset+(node-64);
            work[i] = node;
        }
        if(to_explore.empty() and std::accumulate(times.begin(), times.end(), 0) <= 0){
            break;
        }
        time++;
    }
    std::cout << time << std::endl;
}
