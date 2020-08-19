#include <set>
#include <vector>
#include <queue>
#include <string>
#include <fstream>
#include <iostream>
#include <utility> 
#include <algorithm>
#include <stdexcept>
#include <unistd.h>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

typedef std::vector<std::set<std::string>> map;

void print_building(map& blueprint){
    size_t n_floor = blueprint.size();
    for (auto it=blueprint.rbegin(); it != blueprint.rend(); it++){
        std::set<std::string> contents = *it;
        std::cout << "F" << n_floor;
        for (auto it_f=contents.begin(); it_f != contents.end(); it_f++){
            std::cout << " " << *it_f;
        }
        n_floor--;
        std::cout << std::endl;
    }
}

std::string hash_state(map& state, int floor){
    std::string hash = std::to_string(floor);
    for (size_t i=0; i<state.size(); i++){
        for (auto it=state[i].begin(); it!=state[i].end(); it++){
            hash += *it;
        }
        hash += "|";
    }
    return hash;
}

bool is_valid(std::set<std::string> contents, std::set<std::string> contents_src, const std::vector<std::string>& elevator){
    contents.insert(elevator[0]);
    if (elevator.size() == 2) contents.insert(elevator[1]);
    std::string element, type;
    size_t n;
    bool have_generator = std::any_of(contents.begin(), contents.end(), [](std::string x){ return x.substr(x.size()-1) == "G";});
    for (auto it=contents.begin(); it!=contents.end(); it++){
        n = it->size();
        element = it->substr(0, n-1);
        type = it->substr(n-1);
        if (type == "M" and have_generator and (contents.find(element+"G") == contents.end())) return false;
    }
    contents_src.erase(contents_src.find(elevator[0]));
    if (elevator.size() == 2) contents_src.erase(contents_src.find(elevator[1]));
    have_generator = std::any_of(contents_src.begin(), contents_src.end(), [](std::string x){ return x.substr(x.size()-1) == "G";});
    for (auto it=contents_src.begin(); it!=contents_src.end(); it++){
        n = it->size();
        element = it->substr(0, n-1);
        type = it->substr(n-1);
        if (type == "M" and have_generator and (contents_src.find(element+"G") == contents_src.end())) return false;
    }
    return true;
}

bool is_final_pos(const map & locations){
    return (locations[0].size()+locations[1].size()+locations[2].size()) == 0;
}

std::pair<bool,map> apply_move(map locations, std::vector<std::string> elevator, int dest_floor, int source_floor){
    if (!is_valid(locations[dest_floor], locations[source_floor], elevator)){
        return std::make_pair(false, locations);
    }
    locations[dest_floor].insert(elevator[0]);
    locations[source_floor].erase(locations[source_floor].find(elevator[0]));
    if (elevator.size() == 2) {
        locations[dest_floor].insert(elevator[1]);
        locations[source_floor].erase(locations[source_floor].find(elevator[1]));
    }
    return std::make_pair(true, locations);
}

void handle_valid_move(std::set<std::string>& visited, bool valid, map& new_state, int new_floor, int new_depth, int& min_moves, std::queue<map>& queue_states, std::queue<int> & queue_floor, std::queue<int>& queue_moves){
    if (valid){
        if (is_final_pos(new_state)){
            // check if shortest path
            min_moves = std::min(new_depth, min_moves);
            return;
        }
        // check if new state has already been explored
        std::string hash = hash_state(new_state, new_floor);
        if (visited.find(hash) == visited.end()){
            // need to explore further this state, so we add it
            // back to the queue
            visited.insert(hash);
            queue_states.push(new_state);
            queue_moves.push(new_depth);
            queue_floor.push(new_floor);
        }
    }
}

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    map building;
    std::vector<std::string> parsed_line;
    int floor = 0;
    for (std::string line; std::getline(input_file, line);){
        std::set<std::string> contents;
        boost::split(parsed_line, line, boost::is_any_of(" "), boost::token_compress_on);
        size_t i = 3, n = parsed_line.size();
        while (i < n){
            if (parsed_line[i] == "a"){
                contents.insert(boost::to_upper_copy(parsed_line[i+1].substr(0, 2)+parsed_line[i+2].substr(0, 1)));
                i += 3;
            }
            else{
                i++;
            }
        }
        building.push_back(contents);
        floor++;
    }
    building[0].insert("ELG");
    building[0].insert("ELM");
    building[0].insert("DIG");
    building[0].insert("DIM");
    print_building(building);
    floor = 0;
    std::set<std::string> set_states, history;
    int min_len = 100000000, move_num=0;
    std::vector<std::string> elevator_cont;
    std::queue<map> valid_states;
    std::queue<int> nmoves, nfloor;
    valid_states.push(building);
    nmoves.push(0);
    nfloor.push(0);
    while (not valid_states.empty()){
        building = valid_states.front();
        valid_states.pop();
        floor = nfloor.front();
        nfloor.pop();
        move_num = nmoves.front();
        nmoves.pop();
        // try combinations
        for (auto it=building[floor].begin(); it != building[floor].end(); it++){
            elevator_cont = {*it};
            // move_up
            if (floor < 3){
                auto res = apply_move(building, elevator_cont, floor+1, floor);
                handle_valid_move(history, res.first, res.second, floor+1, move_num+1, min_len, valid_states, nfloor, nmoves);
            }
            if (floor > 0){
                auto res = apply_move(building, elevator_cont, floor-1, floor);
                handle_valid_move(history, res.first, res.second, floor-1, move_num+1, min_len, valid_states, nfloor, nmoves);
            }
            elevator_cont.push_back(*it);
            for (auto it2=it; it2 != building[floor].end(); it2++){
                if (*it == *it2) continue;
                elevator_cont[1] = *it2;
                if (floor < 3){
                    auto res = apply_move(building, elevator_cont, floor+1, floor);
                    handle_valid_move(history, res.first, res.second, floor+1, move_num+1, min_len, valid_states, nfloor, nmoves);
                }
                if (floor > 0){
                    auto res = apply_move(building, elevator_cont, floor-1, floor);
                    handle_valid_move(history, res.first, res.second, floor-1, move_num+1, min_len, valid_states, nfloor, nmoves);
                }
            }
        }
    }
    std::cout << "Mininum path is " << min_len << " moves" << std::endl;
}
