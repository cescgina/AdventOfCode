#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

int parse_node(const std::vector<int>& tree, int index, int& sum){
    int nodes = tree[index];
    int meta = tree[index+1];
    index += 2;
    for (int i=0; i<nodes; i++){
        index = parse_node(tree, index, sum);
    }
    for (int i=0; i<meta; i++){
        sum += tree[index];
        index++;
    }
    return index;
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
    int metadata = 0;
    parse_node(numbers, 0, metadata);
    std::cout << metadata << std::endl;
}
