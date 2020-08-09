#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>

long long  parse_position(size_t i, std::string & message, long long score, size_t n){
    if (i == n){
        return score+1;
    }
    while (i < n){
        if (message[i] == '('){
            auto pos = message.find("x", i);
            int chars = std::stoi(message.substr(i+1, pos-i-1));
            auto close_pos = message.find(")", pos);
            int repeats = std::stoi(message.substr(pos+1, close_pos-pos-1));
            score += repeats*parse_position(close_pos+1, message, 0, close_pos+chars+1);
            i = close_pos+chars+1;
        }
        else{
            score++;
            i++;
        }
    }
    return score;
}

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::getline(input_file, line);
    size_t n = line.size();
    std::cout << "Total length of decompressed message is  " << parse_position(0, line, 0, n) << std::endl;
}
