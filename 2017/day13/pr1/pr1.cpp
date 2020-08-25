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

typedef std::map<int, int> graph;

int main(int argc, char** argv){
    std::string file_name, des_reg, cond_reg;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    graph layers;
    int last_layer = 0, i = 0, severity = 0;
    for (std::string line; std::getline(input_file, line);){
        std::vector<std::string> parsed_line;
        boost::split(parsed_line, line, boost::is_any_of(" :"), boost::token_compress_on);
        int layer = std::stoi(parsed_line[0]), range = std::stoi(parsed_line[1]);
        layers[layer] = range;
        last_layer = layer;
    }
    while (i <= last_layer){
        if (layers.find(i) != layers.end() and i % (2*(layers[i]-1)) == 0){
            severity += i*layers[i];
        }
        i++;
    }
    std::cout << severity << std::endl;
}
