#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <unistd.h>

typedef std::map<std::string, std::map<std::string, int>> properties;
typedef std::map<std::string, int> contents;
typedef std::vector<std::string> names;

void parse_command(const std::string& direction, properties& food, names& list_names){
    std::string name, property;
    int value;
    std::string::size_type n, n_prev;
    n = direction.find(":");
    n_prev = n;
    name = direction.substr(0, n);
    food[name] = {};
    while (n != std::string::npos){
        n = direction.find(":", n_prev+1);
        property = direction.substr(n_prev+2, n-n_prev-2);
        n_prev = n;
        n = direction.find(",", n_prev+1);
        if (n == std::string::npos){
            value = std::stoi(direction.substr(n_prev+2));
        }
        else{
            value = std::stoi(direction.substr(n_prev+2, n-n_prev-2));
        }
        n_prev = n;
        food[name][property] = value;
    }
    list_names.push_back(name);
}

template <typename T>
void print_vector(const std::vector<T>& elements){
    for (auto it=elements.begin(); it!=elements.end(); it++){
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

int calculate_scores(properties& food, contents& ingredients, names& aunts){
    bool is_aunt=false;
    int aunt_num=0;
    for (auto& name: aunts){
        auto aunt_list = food[name];
        is_aunt = true;
        for (auto it=ingredients.begin(); it != ingredients.end(); it++){
            if (aunt_list.find(it->first) != aunt_list.end()){
                if (it->first == "cats" or it->first == "trees"){
                    if (it->second >= aunt_list[it->first]){
                        is_aunt = false; 
                        break;
                    }
                }
                else if (it->first == "pomeranians" or it->first == "goldfish"){
                    if (it->second <= aunt_list[it->first]){
                        is_aunt = false; 
                        break;
                    }
                }
                else{
                    if (it->second != aunt_list[it->first]){
                        is_aunt = false; 
                        break;
                    }
                }
            }
        }
        if (is_aunt){
            aunt_num = std::stoi(name.substr(4));
            return aunt_num;
        }
    }
    return -1;
}

int main(int argc, char** argv){
    std::string file_name, command;
    getopt(argc, argv, "");
    properties food_prop;
    contents found;
    found["children"]= 3;
    found["cats"]= 7;
    found["samoyeds"]= 2;
    found["pomeranians"]= 3;
    found["akitas"]= 0;
    found["vizslas"]= 0;
    found["goldfish"]= 5;
    found["trees"]= 3;
    found["cars"]= 2;
    found["perfumes"]= 1;
    names aunts;
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    while (std::getline(input_file, command)){
        parse_command(command, food_prop, aunts);
    }
    input_file.close();
    std::cout << "Present is from aunt " << calculate_scores(food_prop, found, aunts) << std::endl;
}
