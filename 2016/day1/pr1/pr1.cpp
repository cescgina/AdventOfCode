#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

typedef std::map<int,std::vector<int>> directions;
typedef std::map<std::string,std::vector<int>> turnings;

int main(int argc, char** argv){
    std::string file_name, turn;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    int offset, x = 0, y = 0, direction = 0;
    std::vector<std::string> parsed_line;
    directions changes;
    changes[0] = {0, 1};
    changes[1] = {1, 0};
    changes[2] = {0, -1};
    changes[3] = {-1, 0};
    turnings turn_direction;
    turn_direction["L"] = {3,0,1,2};
    turn_direction["R"] = {1,2,3,0};
    for (std::string line; std::getline(input_file, line);){
        boost::split(parsed_line, line, boost::is_any_of(", "), boost::token_compress_on);
        for (size_t i = 0; i < parsed_line.size(); i++){
            turn = parsed_line[i].substr(0, 1);
            offset = std::stoi(parsed_line[i].substr(1));
            direction = turn_direction[turn][direction];
            x += changes[direction][0]*offset;
            y += changes[direction][1]*offset;
        }
    }
    int distance = std::abs(x)+std::abs(y);
    std::cout << "HQ is " << distance << " blocks away" << std::endl;
}
