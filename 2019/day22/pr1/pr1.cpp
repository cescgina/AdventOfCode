#include <stack>
#include <queue>
#include <vector>
#include <string>
#include <fstream>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

bool DEBUG = false;
typedef std::vector<std::string> instrution;
typedef std::vector<int> cards;
typedef std::vector<std::pair<int, int>> instrutions;

void deal_with_increment(cards& current, int offset){
    cards new_cards(current);
    int cards_changed = 0;
    int positon = 0;
    int number_cards = current.size();
    while (cards_changed < number_cards){
        current[positon] = new_cards[cards_changed];
        cards_changed++;
        positon = (positon+offset) % number_cards;
    }
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    int number_cards = 10;
    if (file_name == "data/input.txt") number_cards = 10007;
    instrutions all_instructions;
    std::ifstream input_file;
    input_file.open(file_name);
    for (std::string line; std::getline(input_file, line);){
        instrution to_parse;
        boost::split(to_parse, line, boost::is_any_of(" "), boost::token_compress_on);
        if (to_parse[0] == "cut"){
            all_instructions.push_back(std::make_pair(2, std::stoi(to_parse[1])));
        }
        else{
            if (to_parse[1] == "into"){
                all_instructions.push_back(std::make_pair(0, 0));
            }
            else{
                all_instructions.push_back(std::make_pair(1, std::stoi(to_parse[3])));
            }
        }
    }
    cards current(number_cards);
    std::iota(current.begin(), current.end(), 0);
    for (auto& instruction_info: all_instructions){
        switch (instruction_info.first) {
            case 0:
                // reverse
                std::reverse(current.begin(), current.end());
                break;
            case 1:
                // deal with increment
                deal_with_increment(current, instruction_info.second);
                break;
            case 2:
                // cut
                if (instruction_info.second > 0){
                    std::rotate(current.begin(),current.begin()+instruction_info.second, current.end());
                }
                else{
                    std::rotate(current.begin(),current.end()+instruction_info.second, current.end());
                }
                break;
        }
    }
    if (file_name == "data/input.txt"){
        for (size_t i = 0; i<current.size(); i++){
            if (current[i] == 2019) {
                std::cout << i << std::endl;
                break;
            }
        }
    }
    else{
        for (int n: current){
            std::cout << n << ' ';
        }
        std::cout << std::endl;
    }
}
