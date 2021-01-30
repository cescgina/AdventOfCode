#include <set>
#include <deque>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

bool DEBUG = false;

typedef std::deque<int> deck;

template <typename T>
bool contains(T& haystack, deck needle){
    return haystack.find(needle) != haystack.end();
}

std::ostream& operator<<(std::ostream& out, deck& hand){
    for (auto& card: hand){
        out << card << " ";
    }
    return out;
}
bool play_game(deck& player1, deck& player2){
    std::set<deck> cache;
    while (player1.size()*player2.size() > 0){
        if (contains(cache, player1)) return true;
        cache.insert(player1);
        size_t card1 = player1.front(), card2 = player2.front();
        player1.pop_front();
        player2.pop_front();
        if (player1.size() >= card1 and player2.size() >= card2){
            deck new_play1;
            new_play1.insert(new_play1.begin(), player1.begin(), player1.begin()+card1);
            deck new_play2;
            new_play2.insert(new_play2.begin(), player2.begin(), player2.begin()+card2);
            bool winer = play_game(new_play1, new_play2);
            if (winer){
                player1.push_back(card1);
                player1.push_back(card2);
            }
            else{
                player2.push_back(card2);
                player2.push_back(card1);
            }
        }
        else{
            if (card1 > card2){
                player1.push_back(card1);
                player1.push_back(card2);
            }
            else{
                player2.push_back(card2);
                player2.push_back(card1);
            }
        }
    }
    return player2.empty();
}
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
    bool won = play_game(player1, player2);
    if (DEBUG) {
        std::cout << "Player 1's deck: " << player1 << std::endl;
        std::cout << "Player 2's deck: " << player2 << std::endl;
    }
    int multiplier = player1.size()+player2.size();
    long long score = 0;
    if (not won){
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
