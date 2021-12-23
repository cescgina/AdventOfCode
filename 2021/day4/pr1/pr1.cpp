#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>

struct Result {
    bool success;
    size_t row, column;
    Result(bool succes, size_t r, size_t c){
        success = succes;
        row = r;
        column = c;
    }
};

class Card {
    std::vector<std::vector<std::string>> numbers;
    public:
    void reset(){
        numbers.clear();
    }
    void add_line(std::string line){
        std::vector<std::string> contents;
        boost::trim(line);
        boost::split(contents, line, boost::is_any_of(" "), boost::token_compress_on);
        numbers.push_back(contents);
    }

    void print_card(){
        std::cout << std::endl;
        for (size_t i=0; i<numbers.size(); i++){
            for (size_t j=0; j<numbers[i].size(); j++){
                std::cout << numbers[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    Result have_number(std::string number){
        for (size_t i=0; i<numbers.size(); i++){
            for (size_t j=0; j<numbers[i].size(); j++){
                if (numbers[i][j] == number){
                    numbers[i][j] = "*";
                    return Result(true, i, j);
                }
            }
        }
        return Result(false, 0, 0);
    }

    bool is_finished(size_t row, size_t col){
        if (std::all_of(numbers[row].begin(), numbers[row].end(), [](std::string& num){return num == "*";})) return true;
        for (size_t i=0; i<numbers.size(); i++){
            if (numbers[i][col] != "*") return false;
        }
        return true;
    }

    size_t get_score(){
        size_t score = 0;
        for (size_t i=0; i<numbers.size(); i++){
            for (size_t j=0; j<numbers[0].size(); j++){
                if (numbers[i][j] != "*") score += std::stoi(numbers[i][j]);
            }
        }
        return score;
     }       
};

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file(file_name);
    std::getline(input_file, line);
    std::vector<std::string> lottery;
    boost::split(lottery, line, boost::is_any_of(","), boost::token_compress_on);
    std::getline(input_file, line);
    std::vector<Card> cards;
    Card current;
    while (std::getline(input_file, line)){
        if (line.empty()){
            cards.push_back(current);
            current.reset();
        }
        else{
            current.add_line(line);
        }
    }
    cards.push_back(current);
    for (auto&& number: lottery){
        for (auto&& card: cards){
            Result res = card.have_number(number);
            if (res.success and card.is_finished(res.row, res.column)){
                size_t score = card.get_score();
                std::cout << "Score is " << score*std::stoi(number) << std::endl;
                return 0;
            }
        }
    }
}
