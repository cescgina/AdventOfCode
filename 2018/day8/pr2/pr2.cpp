#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

std::pair<int, int> parse_node(const std::vector<int>& tree, int index){
    int nodes = tree[index];
    int meta = tree[index+1];
    int value = 0;
    index += 2;
    std::vector<int> child_values;
    for (int i=0; i<nodes; i++){
        auto res = parse_node(tree, index);
        index = res.first;
        child_values.push_back(res.second);
    }
    for (int i=0; i<meta; i++){
        if (nodes == 0){
            value += tree[index];
        }
        else if (tree[index] > 0 and tree[index] <= nodes){
            value += child_values[tree[index]-1];
        }
        index++;
    }
    return std::make_pair(index, value);
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::vector<std::string> parsed_line;
    std::string line; 
    std::getline(input_file, line);
    boost::split(parsed_line, line, boost::is_any_of(" "));
    std::vector<int> numbers;
    std::transform(parsed_line.begin(), parsed_line.end(), std::back_inserter(numbers), [](const std::string& s){ return std::stoi(s); });
    auto root = parse_node(numbers, 0);
    std::cout << root.second << std::endl;
}
