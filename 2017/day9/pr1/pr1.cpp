#include <string>
#include <fstream>
#include <utility>
#include <iostream>
#include <unistd.h>

std::pair<int, int> parse_groups(const std::string& msg, int index, int score){
    bool garbage = false;
    int n = msg.size();
    int local_score = 0;
    while (index < n){
        if (garbage){ 
            if (msg[index] == '!'){
                index += 2;
                continue;
            }
            if (msg[index] == '>'){
                garbage = false;
            }
            index++;
            continue;
        }
        if (msg[index] == '{'){
            auto res = parse_groups(msg, index+1, score+1);
            local_score += res.first;
            index = res.second;
            continue;
        }
        else if (msg[index] == '}'){
            return std::make_pair(local_score+score, index+1);
        }
        else if (msg[index] == '<'){
            garbage = true;
        }
        index++;
    }
    return std::make_pair(score+local_score, index);
}

int main(int argc, char** argv){
    std::string file_name, des_reg, cond_reg;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    for (std::string line; std::getline(input_file, line);){
        std::cout << line << " ";
        auto res = parse_groups(line, 1, 1);
        std::cout << res.first << std::endl;
    }
}
