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

bool is_caught(int time, int last_layer, graph& layers){
    int i = 0;
    while (i <= last_layer){
        if (layers.find(i) != layers.end() and time % (2*(layers[i]-1)) == 0){
            return true;
        }
        time++;
        i++;
    }
    return false;
}

int main(int argc, char** argv){
    std::string file_name, des_reg, cond_reg;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    graph layers;
    int last_layer = 0;
    for (std::string line; std::getline(input_file, line);){
        std::vector<std::string> parsed_line;
        boost::split(parsed_line, line, boost::is_any_of(" :"), boost::token_compress_on);
        int layer = std::stoi(parsed_line[0]), range = std::stoi(parsed_line[1]);
        layers[layer] = range;
        last_layer = layer;
    }
    int initial = 1;
    while (true){
        if (!is_caught(initial, last_layer, layers)){
            std::cout << initial << std::endl;
            break;
        }
        initial++;
    }
}
