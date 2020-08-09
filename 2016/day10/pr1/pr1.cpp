#include <map>
#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>


typedef std::map<std::string, std::vector<int>> stack;

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::vector<std::string>parsed_line;
    stack bots, output;
    std::list<std::vector<std::string>> instructions;
    std::string dest_1, dest_2;
    int max_val, min_val, goal_1 = 17, goal_2 = 61;
    if (file_name == "data/test.txt"){
        goal_1 = 2;
        goal_2 = 5;
    }
    for (std::string line; std::getline(input_file, line);){
        boost::split(parsed_line, line, boost::is_any_of(" "), boost::token_compress_on);
        if (parsed_line[0] == "value"){
            if (bots.find(parsed_line[5]) == bots.end()){
                bots[parsed_line[5]] = {std::stoi(parsed_line[1])};
            }
            else{
                bots[parsed_line[5]].push_back(std::stoi(parsed_line[1]));
            }
        }
        else{
            instructions.push_back(parsed_line);
        }
    }
    while (instructions.size() > 0){
        for (auto it=instructions.begin(); it != instructions.end(); it++){
            parsed_line = *it;
            if (bots.find(parsed_line[1]) != bots.end() and bots[parsed_line[1]].size() == 2){
                dest_1 = parsed_line[6];
                dest_2 = parsed_line[11];
                auto res = std::minmax_element(bots[parsed_line[1]].begin(), bots[parsed_line[1]].end());
                min_val = *res.first;
                max_val = *res.second;
                if (min_val == goal_1 and max_val == goal_2){
                    std::cout << "Bot " << parsed_line[1] << " is the responsible" << std::endl;
                    return 0;
                }
                if (parsed_line[5] == "bot"){
                    if (bots.find(dest_1) == bots.end()){
                        bots[dest_1] = {min_val};
                    }
                    else{
                        bots[dest_1].push_back(min_val);
                    }
                }
                else {
                    if (output.find(dest_1) == bots.end()){
                        output[dest_1] = {min_val};
                    }
                    else{
                        output[dest_1].push_back(min_val);
                    }
                }
                if (parsed_line[10] == "bot"){
                    if (bots.find(dest_2) == bots.end()){
                        bots[dest_2] = {max_val};
                    }
                    else{
                        bots[dest_2].push_back(max_val);
                    }
                }
                else{
                    if (output.find(dest_2) == bots.end()){
                        bots[dest_2] = {max_val};
                    }
                    else{
                        output[dest_2].push_back(max_val);
                    }
                }
                parsed_line[0] = "-";
                bots[parsed_line[1]].clear();
                *it = parsed_line;
            }
        }
        instructions.remove_if([](std::vector<std::string> n){ return n[0]=="-"; });
    }
    for (auto it=output.begin(); it != output.end(); it++){
        std::cout << it->first << " " << it->second[0] << std::endl;
    }
}
