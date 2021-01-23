#include <map>
#include <set>
#include <queue>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

typedef std::vector<std::string> to_parse;
typedef std::map<std::string, std::vector<std::pair<std::string, int>>> graph;
int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file(file_name);
    graph all_bags;
    for (std::string line; std::getline(input_file, line);){
        to_parse bags_info;
        boost::split(bags_info, line, boost::is_any_of(" ,"), boost::token_compress_on);
        std::string origin = bags_info[0]+" "+bags_info[1];
        if (bags_info[4] != "no"){
            for (size_t i = 4; i < bags_info.size(); i+=4){
                std::string dest = bags_info[i+1]+" "+bags_info[i+2];
                all_bags[dest].push_back(std::make_pair(origin, std::stoi(bags_info[i])));
            }
        }
    }
    std::string node = "shiny gold";
    std::queue<std::string> to_visit;
    to_visit.push(node);
    std::set<std::string> possible;
    while(!to_visit.empty()){
        node = to_visit.front();
        to_visit.pop();
        for (auto& neigh: all_bags[node]){
            possible.insert(neigh.first);
            to_visit.push(neigh.first);
        }
    }
    std::cout << possible.size() << " bags can contain shiny gold" << std::endl;
}
