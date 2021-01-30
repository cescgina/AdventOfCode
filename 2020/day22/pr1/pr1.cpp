#include <deque>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

bool DEBUG = false;

typedef std::deque<int> deck;

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    deck player1, player2;
    std::ifstream input_file(file_name);
    std::getline(input_file, line);
    for (std::string line; std::getline(input_file, line);){
        if (line.empty()) break;
        player1.push_back(std::stoi(line));
    }
    std::getline(input_file, line);
    for (std::string line; std::getline(input_file, line);){
        player2.push_back(std::stoi(line));
    }
    input_file.close();
    while (player1.size()*player2.size() > 0){
        int card1 = player1.front(), card2 =player2.front();
        player1.pop_front();
        player2.pop_front();
        if (card1 > card2){
            player1.push_back(card1);
            player1.push_back(card2);
        }
        else{
            player2.push_back(card2);
            player2.push_back(card1);
        }
    }
    int multiplier = player1.size()+player2.size();
    long long score = 0;
    if (player1.empty()){
        while (not player2.empty()){
            score += multiplier*player2.front();
            multiplier--;
            player2.pop_front();
        }
    }
    else{
        while (not player1.empty()){
            score += multiplier*player1.front();
            multiplier--;
            player1.pop_front();
        }
    }
    std::cout << score << std::endl;
}
