#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

typedef std::vector<std::string> to_parse;

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    int correct = 0;
    std::ifstream input_file(file_name);
    for (std::string line; std::getline(input_file, line);){
        to_parse entry;
        boost::split(entry, line, boost::is_any_of(" -:"), boost::token_compress_on);
        std::string pass = entry[3];
        int min = std::stoi(entry[0])-1;
        int max = std::stoi(entry[1])-1;
        char letter = entry[2][0];
        correct += ((pass[min] == letter) ^ (pass[max] == letter));
    }
    input_file.close();
    std::cout << "There are " << correct << " correct passwords "<< std::endl;
}
