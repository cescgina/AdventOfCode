#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

void print_password(const std::vector<char>& vec){
    for (char x: vec){
        std::cout << x;
    }
    std::cout << std::endl; 
}
int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::vector<char> password;
    std::vector<std::string> parsed_line;
    std::vector<std::string> instructions;
    std::getline(input_file, line);
    std::string X, Y;
    int pos_x, pos_y;
    char temp;
    for (char x: line){
        password.push_back(x);
    }
    for (std::string line; std::getline(input_file, line);){
        instructions.push_back(line);
    }
    input_file.close();
    std::vector<int> rotations = {1, 1, 6, 2, 7, 3, 8, 4};
    for (auto it=instructions.rbegin(); it != instructions.rend(); it++){
        line = *it;
        boost::split(parsed_line, line, boost::is_any_of(" "), boost::token_compress_on);
        if (parsed_line[0] == "swap"){
            if (parsed_line[1] == "position"){
                pos_x = std::stoi(parsed_line[2]); 
                pos_y = std::stoi(parsed_line[5]); 
                temp = password[pos_x];
                password[pos_x] = password[pos_y];
                password[pos_y] = temp;
            }
            else if (parsed_line[1] == "letter"){
                X = parsed_line[2];
                Y = parsed_line[5];
                auto X_it = std::find(password.begin(), password.end(), X[0]);
                auto Y_it = std::find(password.begin(), password.end(), Y[0]);
                temp = *X_it; 
                *X_it = *Y_it;
                *Y_it = temp;
            }
        }
        else if (parsed_line[0] == "rotate"){
            if (parsed_line[1] == "based"){
                X = parsed_line[6];
                auto X_it = std::find(password.begin(), password.end(), X[0]);
                pos_x = X_it-password.begin();
                pos_x = rotations[pos_x];
                std::rotate(password.begin(), password.begin()+pos_x, password.end());
            }
            else if (parsed_line[1] == "right"){
                pos_x = std::stoi(parsed_line[2]); 
                pos_x %= password.size();
                std::rotate(password.begin(), password.begin()+pos_x, password.end());
            }
            else if (parsed_line[1] == "left"){
                pos_x = std::stoi(parsed_line[2]); 
                pos_x %= password.size();
                std::rotate(password.rbegin(), password.rbegin()+pos_x, password.rend());
            }
        }
        else if (parsed_line[0] == "reverse"){
            pos_x = std::stoi(parsed_line[2]); 
            pos_y = std::stoi(parsed_line[4]); 
            std::reverse(password.begin()+pos_x, password.begin()+pos_y+1);
        }
        else if (parsed_line[0] == "move"){
            pos_x = std::stoi(parsed_line[5]); 
            pos_y = std::stoi(parsed_line[2]); 
            temp = password[pos_x];
            password.erase(password.begin()+pos_x);
            password.insert(password.begin()+pos_y, temp);
        }
    }
    print_password(password);
}
