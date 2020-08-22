#include <map>
#include <set>
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

typedef std::map<std::pair<int, int>, std::pair<int, int>> cluster;
typedef std::set<std::pair<std::pair<int, int>, std::pair<int, int>>> nodes;

int main(int argc, char** argv){
    std::string file_name, line, node;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    cluster grid;
    int x = 0, y = 0, used = 0, avail = 0;
    input_file.open(file_name);
    std::vector<std::string> parsed_line;
    std::getline(input_file, line);
    std::getline(input_file, line);
    for (std::string line; std::getline(input_file, line);){
        boost::split(parsed_line, line, boost::is_any_of(" "), boost::token_compress_on);
        used = std::stoi(parsed_line[2].substr(0, parsed_line[2].size()-1));
        avail = std::stoi(parsed_line[3].substr(0, parsed_line[3].size()-1));
        node = parsed_line[0].substr(16);
        auto pos = node.find("-");
        x = std::stoi(node.substr(0, pos));
        y = std::stoi(node.substr(pos+2));
        grid[std::make_pair(x, y)] = std::make_pair(used, avail);
    }
    input_file.close();
    nodes valid_nodes;
    for (auto it = grid.begin(); it != grid.end(); it++){
        if (it->second.first == 0) continue;
        for (auto it2 = grid.begin(); it2 != grid.end(); it2++){
            if (it == it2) continue;
            if (it->second.first <= it2->second.second){
                valid_nodes.insert(std::make_pair(it->first, it2->first));
            }
        }
    }
    std::cout << valid_nodes.size() << std::endl;
}
