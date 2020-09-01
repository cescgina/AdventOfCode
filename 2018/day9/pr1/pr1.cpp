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
    input_file.open(file_name);
    std::vector<std::string> parsed_line;
    std::string line; 
    std::getline(input_file, line);
    boost::split(parsed_line, line, boost::is_any_of("; "), boost::token_compress_on);
    int players = std::stoi(parsed_line[0]), last_marble = std::stoi(parsed_line[6]), marble=2, player = 1, curr_pos = 1;
    std::vector<int> marbles = {0, 1};    
    std::vector<int> scores(players, 0);
    while (marble <= last_marble){
        if (marble % 23 == 0){
            curr_pos -= 7;
            if (curr_pos < 0){
                curr_pos = marbles.size()+curr_pos;
            }
            scores[player] += marbles[curr_pos]+marble;
            marbles.erase(marbles.begin()+curr_pos);
        }
        else{
            if (curr_pos == ((int)marbles.size()-2)){
                marbles.push_back(marble);
                curr_pos += 2;
            }
            else{
                curr_pos += 2;
                curr_pos %= marbles.size();
                auto it = marbles.begin()+curr_pos;
                marbles.insert(it, marble);
            }
        }
        player++;
        player %= players;
        marble++;
    }
    std::cout << *std::max_element(scores.begin(), scores.end()) << std::endl;
}
