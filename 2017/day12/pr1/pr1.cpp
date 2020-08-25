#include <map>
#include <set>
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

typedef std::map<std::string, std::set<std::string>> graph;

void find_connnected_programs(const graph& network, std::set<std::string>& available, const std::string& node, std::set<std::string>& explored){
    if (explored.find(node) != explored.end()){
        return;
    }
    explored.insert(node);
    available.insert(node);
    for (auto children: network.at(node)){
        find_connnected_programs(network, available, children, explored);
    }
    return;
}

int main(int argc, char** argv){
    std::string file_name, des_reg, cond_reg;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    graph programs;
    for (std::string line; std::getline(input_file, line);){
        std::vector<std::string> parsed_line;
        boost::split(parsed_line, line, boost::is_any_of(" ,"), boost::token_compress_on);
        std::set<std::string> edges;
        for (size_t i=0; i<parsed_line.size()-2; i++){
            edges.insert(parsed_line[i+2]);
        }
        programs[parsed_line[0]] = edges;
    }

    std::set<std::string> from_zero, visited;
    find_connnected_programs(programs, from_zero, "0", visited);
    std::cout << from_zero.size() << std::endl;
}
