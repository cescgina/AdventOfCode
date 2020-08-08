#include <map>
#include <set>
#include <utility>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

typedef std::map<int,std::vector<int>> directions;
typedef std::map<std::string,std::vector<int>> turnings;

template<typename T>
void print_set(std::set<T> & arg){
    for (auto it=arg.begin(); it != arg.end(); it++){
        std::cout << "(" << it->first << ", " << it->second << ") ";
    }
    std::cout << std::endl;
}

int main(int argc, char** argv){
    std::string file_name, turn;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    int offset, x = 0, y = 0, direction = 0;
    std::vector<std::string> parsed_line;
    std::set<std::pair<int,int>> history;
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
            for (int j=1; j<=offset; j++){
                x += changes[direction][0]*1;
                y += changes[direction][1]*1;
                if (history.find(std::make_pair(x, y)) != history.end()){
                    std::cout << "First repeated position at " << x << ", " << y << std::endl;
                    int distance = std::abs(x)+std::abs(y);
                    std::cout << "HQ is " << distance << " blocks away" << std::endl;
                    return 0;
                }
                else{
                    history.insert(std::make_pair(x, y));
                }
            }
        }
    }
}
