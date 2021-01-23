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
typedef std::map<std::string, int> cache_values;

template<typename T>
bool contains(T& haystack, std::string needle){
    return haystack.find(needle) != haystack.end();
}

int get_bags(graph& bags, const std::string& node, cache_values& cache){
    if (contains(cache, node)) return cache[node];
    if (not contains(bags, node)){
        cache[node] = 0;
        return 0;
    }
    int value = 0;
    for (auto& neigh: bags[node]){
        value += neigh.second*get_bags(bags, neigh.first, cache) + neigh.second;
    }
    cache[node] = value;
    return value;
}

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
                all_bags[origin].push_back(std::make_pair(dest, std::stoi(bags_info[i])));
            }
        }
    }
    std::string node = "shiny gold";
    cache_values cache;
    std::cout << "shiny gold bags contains " << get_bags(all_bags, "shiny gold", cache) << std::endl;
}
