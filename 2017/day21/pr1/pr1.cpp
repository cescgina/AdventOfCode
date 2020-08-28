#include <map>
#include <set>
#include <queue>
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

typedef std::vector<std::string> pattern;
typedef std::map<pattern, pattern> rulebook;

void print_state(const pattern& picture){
    for (auto row: picture){
        std::cout << row << std::endl;
    }
}

pattern transpose(const pattern& input){
    int n = input.size();
    pattern new_patt(input);
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            new_patt[j][i] = input[i][j];
        }
    }
    return new_patt;
}

pattern flip_h(const pattern& input){
    int n = input.size();
    pattern new_patt(input);
    for (int i = 0; i < n; i++){
        std::reverse(new_patt[i].begin(), new_patt[i].end());
    }
    return new_patt;
}

pattern flip_v(const pattern& input){
    int n = input.size();
    pattern new_patt(input);
    for (int i=0; i<n/2; i++){
        std::swap(new_patt[i], new_patt[n-i-1]);
    }
    return new_patt;
}

pattern enhanced_tile(const pattern& input, rulebook& rules_list){
    if (rules_list.find(input) != rules_list.end()){
        return rules_list[input];
    }
    pattern transformed(input);
    pattern result(input);
    std::queue<pattern> to_explore;
    std::set<pattern> visited;
    to_explore.push(transformed);
    while (!to_explore.empty()){
        transformed = to_explore.front();
        to_explore.pop();
        if (visited.find(transformed) != visited.end()){
            continue;
        }
        visited.insert(transformed);
        result = flip_v(transformed);
        if (rules_list.find(result) != rules_list.end()){
            return rules_list[result];
        }
        to_explore.push(result);
        result = flip_h(transformed);
        if (rules_list.find(result) != rules_list.end()){
            return rules_list[result];
        }
        to_explore.push(result);
        result = transpose(transformed);
        if (rules_list.find(result) != rules_list.end()){
            return rules_list[result];
        }
        to_explore.push(result);
    }
    std::cout << "Could not match pattern" << std::endl;
    return input;
}

pattern transform_squares(const pattern& prev, rulebook& rules_list, int s){
    int prev_size = prev.size();
    int new_size = prev_size+(prev_size/s);
    pattern next_state(new_size, "");
    for (int i=0; i<(prev_size/s); i++){
        int pos = i*s;
        for (int j=0; j<(prev_size/s); j++){
            pattern tile(s, "");
            for (int k=0; k<s; k++){
                tile[k] += prev[pos+k].substr(j*s, s);
            }
            pattern enhanced = enhanced_tile(tile, rules_list);
            for (int k=0; k<=s; k++){
                auto a = enhanced[k];
                next_state[i*(s+1)+k] += a;
            }
        }
    }
    return next_state;
}

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    rulebook rules;
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    for(std::string line; std::getline(input_file, line);){
        auto pos = line.find(" => ");
        std::string original = line.substr(0, pos);
        std::string replace = line.substr(pos+4);
        std::vector<std::string> original_rows;
        std::vector<std::string> replace_rows;
        boost::split(original_rows, original, boost::is_any_of("/"));
        boost::split(replace_rows, replace, boost::is_any_of("/"));
        rules[original_rows] = replace_rows;
    }
    input_file.close();
    pattern state = {".#.", "..#", "###"};
    int size = state.size();
    print_state(state);
    std::cout << std::endl;
    for (int iter=0; iter<5; iter++){
        if (size % 2 == 0){
            state = transform_squares(state, rules, 2);
        }
        else{
            state = transform_squares(state, rules, 3);
        }
        int squares_on = 0;
        for (auto row: state){
            squares_on += std::count(row.begin(), row.end(), '#');
        }
        size = state.size();
        std::cout << iter+1 << " " << squares_on << std::endl;
        print_state(state);
        std::cout << std::endl;
    }
}
