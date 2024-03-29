#include <map>
#include <utility>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>

typedef std::map<std::string,std::vector<int>> turnings;
typedef std::vector<std::vector<std::string>> keypad;

template<typename T>
void print_vector(std::vector<T> & arg){
    for (auto it=arg.begin(); it != arg.end(); it++){
        std::cout << *it;
    }
    std::cout << std::endl;
}

int main(int argc, char** argv){
    std::string file_name, turn;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    int x = 2, y = 0, new_x = 1, new_y = 1;
    keypad numbers = {{"0", "0", "1", "0", "0"}, {"0", "2", "3" ,"4", "0"}, 
                      {"5", "6", "7", "8", "9"}, {"0", "A", "B", "C", "0"},
                      {"0", "0", "D", "0", "0"}};
    std::vector<std::string> code;
    turnings turn_direction;
    turn_direction["L"] = {0,-1};
    turn_direction["R"] = {0,1};
    turn_direction["U"] = {-1,0};
    turn_direction["D"] = {1,0};
    for (std::string line; std::getline(input_file, line);){
        for (size_t i = 0; i < line.size(); i++){
            turn = line[i];
            new_x = std::min(4, std::max(0, x+turn_direction[turn][0]));
            new_y = std::min(4, std::max(0, y+turn_direction[turn][1]));
            if (numbers[new_x][y] != "0"){
                x = new_x;
            }
            if (numbers[x][new_y] != "0"){
                y = new_y;
            }
        }
        code.push_back(numbers[x][y]);
    }
    std::cout << "Code is ";
    print_vector(code);
}
