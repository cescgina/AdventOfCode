#include <deque>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

bool DEBUG = false;

typedef std::deque<int> circle;

std::ostream& operator<<(std::ostream& out, circle& hand){
    for (auto& card: hand){
        out << card;
    }
    return out;
}

int find_dest(circle& play, int current_val){
    while (true){
        if (current_val == 1) current_val = 9;
        else current_val--;
        auto it = std::find(play.begin(), play.end(), current_val);
        if (it != play.end()) return current_val;
    }
    return 0;
}
int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    circle player1;
    std::ifstream input_file(file_name);
    std::getline(input_file, line);
    input_file.close();
    for (char digit: line){
        player1.push_back(digit-'0');
    }
    int moves = 100;
    int current = 0, current_val = player1[0];
    size_t n = player1.size();
    for (int i=0; i<moves; i++){
        circle buff;
        if (DEBUG){
            std::cout << "-- move " << i+1 << " --" << std::endl;
            std::cout << "cups: " << player1 <<std::endl;
        }
        std::rotate(player1.begin(), player1.begin()+current+1, player1.end());
        for (size_t j=0; j<3; j++){
            buff.push_back(player1.front());
            player1.pop_front();
        }
        int destination = find_dest(player1, current_val);
        if (DEBUG){
            std::cout << "pick up: " << buff << std::endl;
            std::cout << "current val: " << current_val << std::endl;
            std::cout << "destination: " << destination << std::endl;
        }
        auto it = std::find(player1.begin(), player1.end(), destination)+1;
        std::rotate(player1.begin(), it, player1.end());
        for (size_t j=0; j<3; j++){
            player1.push_front(buff.back());
            buff.pop_back();
        }
        it = std::find(player1.begin(), player1.end(), current_val);
        int distance = it-player1.begin();
        if (distance > current){
            std::rotate(player1.begin(), player1.begin()+distance-current, player1.end());
        }
        else{
            std::rotate(player1.rbegin(), player1.rbegin()+current-distance, player1.rend());
        }
        current = (current+1) % n;
        current_val = player1[current];
        if (DEBUG){
            std::cout << std::endl;
        }
    }
    auto it = std::find(player1.begin(), player1.end(), 1);
    std::rotate(player1.begin(), it, player1.end());
    player1.pop_front();
    std::cout << player1 << std::endl; 
}
