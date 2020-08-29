#include <set>
#include <vector>
#include <fstream>
#include <utility>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <unistd.h>

typedef std::tuple<int, int, char> state_details;
typedef std::unordered_map<char, std::vector<state_details>> states_info;
typedef std::unordered_map<int, int> tape;

void print_rules(states_info& rules){
    for (auto it=rules.begin(); it != rules.end(); it++){
        std::cout << "State " << it->first<< std::endl;
        auto element = it->second[0];
        std::cout << 0 << ": " << std::get<0>(element) << " " <<  std::get<1>(element) << " " << std::get<2>(element) << std::endl;
        element = it->second[1];
        std::cout << 1 << ": " << std::get<0>(element) << " " <<  std::get<1>(element) << " " << std::get<2>(element) << std::endl;
        std::cout << std::endl;
    }
}
int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    states_info states;
    std::ifstream input_file;
    input_file.open(file_name);
    std::string line;
    std::getline(input_file, line);
    char state = line[line.size()-2];
    std::getline(input_file, line);
    auto pos = line.find("after ");
    auto pos2 = line.find(" steps", pos);
    int diagnostic_steps = std::stoi(line.substr(pos+6,pos2-pos+6-1));
    std::getline(input_file, line);
    while(input_file.is_open()){
        std::getline(input_file, line);
        char curr_state = line[line.size()-2];
        state_details info1;
        std::getline(input_file, line);
        // parse curr value
        std::getline(input_file, line);
        // parse new value
        int new_val = line[line.size()-2]-'0';
        std::getline(input_file, line);
        // parse movement
        int move = 1;
        auto pos = line.find("the ");
        std::string movement = line.substr(pos+4, line.size()-pos-4-1);
        if (movement == "left"){
            move = -1;
        }
        std::getline(input_file, line);
        // new state
        char new_state = line[line.size()-2];
        info1 = std::make_tuple(new_val, move, new_state);
        states[curr_state] = {info1};

        std::getline(input_file, line);
        // parse curr value
        std::getline(input_file, line);
        // parse new value
        new_val = line[line.size()-2]-'0';
        std::getline(input_file, line);
        // parse movement
        move = 1;
        pos = line.find("the ");
        movement= line.substr(pos+4, line.size()-pos-4-1);
        if (movement == "left"){
            move = -1;
        }
        std::getline(input_file, line);
        // new state
        new_state = line[line.size()-2];
        info1 = std::make_tuple(new_val, move, new_state);
        states[curr_state].push_back({info1});
        std::getline(input_file, line);
        if (input_file.eof()){
            break;
        }
    }
    input_file.close();
    int position = 0;
    tape turing_mach = {{0, 0}};
    int step = 0, curr_value = 0;
    while (step < diagnostic_steps){
        curr_value = turing_mach[position];
        auto info = states[state][curr_value];
        state = std::get<2>(info);
        turing_mach[position] = std::get<0>(info);
        position += std::get<1>(info);
        step++;
    }
    std::cout << std::count_if(turing_mach.begin(), turing_mach.end(), [](std::pair<int, int> c){ return c.second == 1; }) << std::endl;
}
