#include <deque>
#include <string>
#include <fstream>
#include <numeric>
#include <iostream>
#include <unordered_map>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

bool DEBUG = false;

typedef std::deque<int> circle;
typedef std::unordered_map<int, int> map_pos;

std::ostream& operator<<(std::ostream& out, circle& hand){
    for (auto& card: hand){
        out << card;
    }
    return out;
}

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file(file_name);
    std::getline(input_file, line);
    input_file.close();
    circle input;
    size_t pad = 1000000;
    //size_t pad = 9;
    //if (file_name == "data/test.txt") pad = 10;
    int max_input = 0;
    for (char digit: line){
        int val = digit-'0';
        input.push_back(val);
        max_input = std::max(max_input, val);
    }
    circle player1(pad+1);
    std::iota(player1.begin(), player1.end(), 1);
    int current_val = input[0];
    for (size_t l=0; l<input.size()-1; l++){
        player1[input[l]] = input[l+1];
    }
    if (pad > input.size()){
        player1.back() = current_val;
        player1[input.back()] = max_input+1;
    }
    else{
        player1[input.back()] = current_val;
    }
    int moves = 10000000;
    
    for (int i=0; i<moves; i++){
        if (DEBUG){
            std::cout << "-- move " << i+1 << " --" << std::endl;
            std::cout << "cups: " << player1 <<std::endl;
        }
        int next = player1[current_val];
        player1[current_val] = player1[player1[player1[next]]];
        circle buff = {next, player1[next], player1[player1[next]]};
        int destination;
        if (current_val > 1){
            destination = current_val-1;
        }
        else{
            destination = pad;
        }
        while (buff[0] == destination or buff[1] == destination or buff[2] == destination){
            if (destination > 1){
                destination--;
            }
            else{
                destination = pad;
            }
        }
        player1[player1[player1[next]]] = player1[destination];
        player1[destination] = next;
        current_val = player1[current_val];
        if (DEBUG){
            std::cout << "pick up: " << buff << std::endl;
            std::cout << "current val: " << current_val << std::endl;
            std::cout << "destination: " << destination << std::endl;
            std::cout << std::endl;
        }
    }
    current_val = player1[1];
    long long product = current_val * player1[current_val];
    std::cout << product << std::endl; 
}
