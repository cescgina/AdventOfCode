#include <map>
#include <set>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

std::string find_root(const std::set<std::string>& programs, const std::set<std::string>& nodes){
    for (auto program: programs){
        if (nodes.find(program) == nodes.end()){
            return program;
        }
    }
    return "";
}

void find_subtree_weights(const std::string& node, std::map<std::string, int>& weight_orig, std::map<std::string, int>& weight_acumm, const std::map<std::string, std::set<std::string>>& graph){
    int sum = weight_orig[node];
    if (graph.find(node) == graph.end()){
        weight_acumm[node] = weight_orig[node];
        return;
    }
    int values = 0;
    for (auto children: graph.at(node)){
        find_subtree_weights(children, weight_orig, weight_acumm, graph);
        sum += weight_acumm[children];
    }
    for (auto children: graph.at(node)){
        if (values == 0){
            values = weight_acumm[children];
        }
        else if (weight_acumm[children] != values){
            std::cout << weight_orig[children]+(values-weight_acumm[children]) << std::endl;
            sum += values-weight_acumm[children];
            break;
        }
    }
    weight_acumm[node] = sum;
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    std::vector<std::string> parsed_line;
    std::set<std::string> programs, nodes; 
    std::map<std::string, std::set<std::string>> tree;
    std::map<std::string, int> weigths, subtrees, weigths_subtree;
    input_file.open(file_name);
    for (std::string line; std::getline(input_file, line);){
        boost::split(parsed_line, line, boost::is_any_of(" \t,"), boost::token_compress_on);
        weigths[parsed_line[0]] = std::stoi(parsed_line[1].substr(1, parsed_line[1].size()-1));
        if (parsed_line.size() > 2){
            std::set<std::string> local_nodes;
            for (size_t i=3; i<parsed_line.size(); i++){
                local_nodes.insert(parsed_line[i]);
                nodes.insert(parsed_line[i]);
            }
            tree[parsed_line[0]] = local_nodes;
        }
        programs.insert(parsed_line[0]);
    }
    input_file.close();
    std::string root = find_root(programs, nodes);
    find_subtree_weights(root, weigths,weigths_subtree, tree);
}
