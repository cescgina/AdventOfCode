#include <set>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    std::vector<std::string> parsed_line;
    std::set<std::string> programs, nodes; 
    input_file.open(file_name);
    for (std::string line; std::getline(input_file, line);){
        boost::split(parsed_line, line, boost::is_any_of(" \t,"), boost::token_compress_on);
        if (parsed_line.size() > 2){
            for (size_t i=3; i<parsed_line.size(); i++){
                nodes.insert(parsed_line[i]);
            }
        }
        programs.insert(parsed_line[0]);
    }
    input_file.close();
    for (auto program: programs){
        if (nodes.find(program) == nodes.end()){
            std::cout << program << std::endl;
            return 0;
        }
    }
}
