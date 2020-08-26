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
    std::getline(input_file, line);
    std::string X, Y;
    int pos_x, pos_y;
    char temp;
    for (char x: line){
        password.push_back(x);
    }
    std::getline(input_file, line);
    input_file.close();
    boost::split(parsed_line, line, boost::is_any_of(","), boost::token_compress_on);
    for (auto cmd: parsed_line){
        if (cmd[0] == 'x'){
            auto pos = cmd.find("/");
            pos_x = std::stoi(cmd.substr(1, pos-1)); 
            pos_y = std::stoi(cmd.substr(pos+1)); 
            temp = password[pos_x];
            password[pos_x] = password[pos_y];
            password[pos_y] = temp;
        }
        else if (cmd[0] == 'p'){
            auto pos = cmd.find("/");
            X = cmd.substr(1, pos-1); 
            Y = cmd.substr(pos+1); 
            auto X_it = std::find(password.begin(), password.end(), X[0]);
            auto Y_it = std::find(password.begin(), password.end(), Y[0]);
            temp = *X_it; 
            *X_it = *Y_it;
            *Y_it = temp;
        }
        else if (cmd[0] == 's'){
            pos_x = std::stoi(cmd.substr(1)); 
            pos_x %= password.size();
            std::rotate(password.rbegin(), password.rbegin()+pos_x, password.rend());
        }
    }
    print_password(password);
}
