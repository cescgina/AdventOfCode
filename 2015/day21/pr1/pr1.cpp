#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

typedef std::map<std::string,std::vector<int>> items;

template <typename T>
void print_vector(std::vector<T>& vec){
    for (auto it=vec.begin(); it != vec.end(); it++){
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

void parse_shop(items& armor_possible, items& weapon_possible, items& rings_possible){
    std::ifstream shop;
    std::string line;
    std::vector<std::string> parsed_line;
    shop.open("data/shop.txt");
    // read weapons header
    std::getline(shop, line);
    std::getline(shop, line);
    while (!line.empty()){
        std::vector<int> parameters;
        boost::split(parsed_line, line, boost::is_any_of(" "), boost::token_compress_on);
        std::transform(parsed_line.begin()+1, parsed_line.end(), std::back_inserter(parameters), [](std::string c) -> int {return std::stoi(c);});
        weapon_possible.insert({parsed_line[0], parameters});
        std::getline(shop, line);
    }
    // read armor header
    std::getline(shop, line);
    std::getline(shop, line);
    while (!line.empty()){
        std::vector<int> parameters;
        boost::split(parsed_line, line, boost::is_any_of(" "), boost::token_compress_on);
        std::transform(parsed_line.begin()+1, parsed_line.end(), std::back_inserter(parameters), [](std::string c) -> int {return std::stoi(c);});
        armor_possible.insert({parsed_line[0], parameters});
        std::getline(shop, line);
    }
    // read rings header
    std::getline(shop, line);
    std::getline(shop, line);
    while (!line.empty()){
        std::vector<int> parameters;
        auto pos = line.find("+");
        // name of rings may contain one space
        std::string name = line.substr(pos+3);
        boost::split(parsed_line, name, boost::is_any_of(" "), boost::token_compress_on);
        std::transform(parsed_line.begin()+1, parsed_line.end(), std::back_inserter(parameters), [](std::string c) -> int {return std::stoi(c);});
        rings_possible.insert({line.substr(0, pos+2), parameters});
        std::getline(shop, line);
    }
    shop.close();
}

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    int boss_hp = 0, boss_dmg = 0, boss_armor = 0;
    int hp = 100, dmg = 0, armor = 0;
    int cost = 0, optimal_cost = 10000, turns_player = 0, turns_boss = 0;
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::getline(input_file, line);
    auto pos = line.find(":");
    boss_hp = std::stoi(line.substr(pos+2));
    std::getline(input_file, line);
    pos = line.find(":");
    boss_dmg = std::stoi(line.substr(pos+2));
    std::getline(input_file, line);
    pos = line.find(":");
    boss_armor = std::stoi(line.substr(pos+2));
    input_file.close();
    items armors, weapons, rings;
    armors["No armor"] = {0, 0, 0};
    rings["No ring 1"] = {0, 0, 0};
    rings["No ring 2"] = {0, 0, 0};
    parse_shop(armors, weapons, rings);
    for (auto w_it=weapons.begin(); w_it != weapons.end(); w_it++){
        for (auto a_it=armors.begin(); a_it != armors.end(); a_it++){
            for (auto r_it=rings.begin(); r_it != rings.end(); r_it++){
                for (auto r_it_2=rings.begin(); r_it_2 != rings.end(); r_it_2++){
                    if (r_it->first == r_it_2->first) continue;
                    dmg = (w_it->second[1]+r_it->second[1]+r_it_2->second[1]);
                    armor = (a_it->second[2]+r_it->second[2]+r_it_2->second[2]);
                    cost = (w_it->second[0]+a_it->second[0]+r_it->second[0]+r_it_2->second[0]);
                    // can't win
                    if (dmg <= boss_armor) {
                        continue;
                    }
                    if (boss_dmg <= armor){
                        optimal_cost = std::min(cost, optimal_cost);
                        continue;
                    }
                    turns_player = boss_hp/(dmg-boss_armor);
                    if (boss_hp % (dmg-boss_armor) != 0) turns_player++;
                    turns_boss = hp/(boss_dmg-armor);
                    if (hp % (boss_dmg-armor) != 0) turns_boss++;
                    if (turns_player <= turns_boss){
                        optimal_cost = std::min(cost, optimal_cost);
                    }
                }
            }
        }
    }
    std::cout << "Cheapest combination costs " << optimal_cost << std::endl;
}
