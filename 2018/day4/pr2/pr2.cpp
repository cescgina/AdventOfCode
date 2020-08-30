#include <set>
#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <unordered_map>
#include <unistd.h>

typedef std::unordered_map<int, std::vector<int>> guard;

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::unordered_map<int, std::vector<std::string>> guards_input;
    int id;
    std::set<std::string> buffer;
    for (std::string line; std::getline(input_file, line);){
        buffer.insert(line);
    }
    for (std::string line: buffer){
        std::string action = line.substr(19, 5), id_parsing = line.substr(26, 7);
        if (action == "Guard"){
            auto pos = id_parsing.find(" ");
            id = std::stoi(id_parsing.substr(0, pos));
        }
        else{
            guards_input[id].push_back(line);
        }
    }
    int max_minuts = 0, minute_max = 0, answer = 0;
    for (auto it=guards_input.begin(); it != guards_input.end(); it++){
        auto lines = it->second;
        int id = it->first;
        std::vector<int> days(60, 0);
        std::vector<int> curr_day(60, 0);
        std::string prev_day = "";
        for (std::string line: lines){
            std::string action = line.substr(19, 5), day = line.substr(6, 5);
            int minute = std::stoi(line.substr(15, 2));
            if (day != prev_day){
                for (size_t i=0; i<60; i++){
                    days[i] += curr_day[i];
                }
                prev_day = day;
                curr_day = std::vector<int>(60, 0);    
            }
            for (int j=minute; j<60; j++){
                curr_day[j] = action=="falls";
            }
        }
        for (size_t i=0; i<60; i++){
            days[i] += curr_day[i];
        }
        auto pos_max = std::max_element(days.begin(), days.end());
        int total_minuts = *pos_max;
        if (total_minuts > max_minuts){
            max_minuts = total_minuts;
            minute_max = std::distance(days.begin(), pos_max);
            answer = minute_max*id;
        }
    }
    std::cout << answer << std::endl;
}
