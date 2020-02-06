#include <map>
#include <string>
#include <fstream>
#include <cstdlib>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include "cppitertools/combinations.hpp"

typedef std::map<std::string, std::map<std::string, int>> properties;
typedef std::vector<std::string> names;

void parse_command(const std::string& direction, properties& food, names& list_names){
    std::string name;
    std::string::size_type n, n_prev;
    n = direction.find(":");
    n_prev = n;
    name = direction.substr(0, n);
    n = direction.find(",", n_prev+1);
    food[name] = {std::make_pair("capacity", std::stoi(direction.substr(n_prev+11, n-n_prev-10)))};
    n_prev = n;
    n = direction.find(",", n_prev+1);
    food[name].insert(std::make_pair("durability", std::stoi(direction.substr(n_prev+12, n-n_prev-11))));
    n_prev = n;
    n = direction.find(",", n_prev+1);
    food[name].insert({std::make_pair("flavor", std::stoi(direction.substr(n_prev+8, n-n_prev-7)))});
    n_prev = n;
    n = direction.find(",", n_prev+1);
    food[name].insert({std::make_pair("texture", std::stoi(direction.substr(n_prev+9, n-n_prev-8)))});
    n_prev = n;
    n = direction.find("s ", n_prev+1);
    food[name].insert({std::make_pair("calories", std::stoi(direction.substr(n_prev+11)))});
    list_names.push_back(name);
}

void print_vector(const std::vector<int>& elements){
    for (auto it=elements.begin(); it!=elements.end(); it++){
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

int calculate_scores(properties& food, names& ingredients){
    int max_score = 0, sum = 0, order = 0, score = 0;
    // generate combinations
    std::vector<int> v(100);
    std::vector<int> comb(food.size());
    std::iota(v.begin(), v.end(), 1);
    for (auto&& i : iter::combinations(v, food.size())) {
        sum = 0;
        order = 0;
        for (auto&& j : i ) {
           sum += j; 
           comb[order] = j;
           order++;
        }
        if (sum != 100 ){
            continue;
        }
        std::sort(comb.begin(), comb.end());
        do {
            int capa_score = 0, dur_score = 0, flavor_score = 0, texture_score = 0, calories = 0;
            order = 0;
            for (auto&& j : comb ) {
                capa_score += j*food[ingredients[order]]["capacity"];
                dur_score += j*food[ingredients[order]]["durability"];
                flavor_score += j*food[ingredients[order]]["flavor"];
                texture_score += j*food[ingredients[order]]["texture"];
                calories += j*food[ingredients[order]]["calories"];
                order++;
            }
            if (calories != 500) continue;
            score = std::max(0, capa_score)*std::max(0, dur_score)*std::max(0, flavor_score)*std::max(0, texture_score);
            if (score > max_score){
                max_score = score;
            }
        } while (std::next_permutation(comb.begin(), comb.end()));
    }
    return max_score;
}

int main(int argc, char** argv){
    std::string file_name, command;
    int opt_score = 0;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    properties food_prop;
    names ingredients;
    std::ifstream input_file;
    input_file.open(file_name);
    while (std::getline(input_file, command)){
        parse_command(command, food_prop, ingredients);
    }
    input_file.close();
    opt_score = calculate_scores(food_prop, ingredients);
    std::cout << "Max score is " << opt_score << std::endl;
}
