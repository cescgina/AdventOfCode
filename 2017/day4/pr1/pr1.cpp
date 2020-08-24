#include <set>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    int checksum = 0;
    input_file.open(file_name);
    for (std::string line; std::getline(input_file, line);){
        std::vector<std::string> parsed_line;
        std::set<std::string> unique;
        boost::split(parsed_line, line, boost::is_any_of(" \t"), boost::token_compress_on);
        unique.insert(parsed_line.begin(), parsed_line.end());
        checksum += (unique.size() == parsed_line.size());
    }
    input_file.close();
    std::cout << checksum << std::endl;
}
