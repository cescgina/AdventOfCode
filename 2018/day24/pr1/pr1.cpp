#include <set>
#include <map>
#include <regex>
#include <string>
#include <vector>
#include <string>
#include <memory>
#include <numeric>
#include <utility>
#include <fstream>
#include <iostream>
#include <boost/regex.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <unistd.h>

bool DEBUG = false;

struct Group{
    int initiative;
    int units;
    std::string attack_type;
    int attack;
    int hp;
    int id;
    char type;
    std::set<std::string> immune;
    std::set<std::string> weak;
    int effective = units*attack;
    bool operator<(const Group &other) const{
        if (effective == other.effective){
            return initiative > other.initiative;
        }
        else{
            return effective > other.effective;
        }
    }
};


std::ostream& operator<<(std::ostream& out, const Group& obj){
    if (obj.type == 'F'){
        out << "Infection: ";
    }
    else{
        out << "Immune system ";
    }
    out << "Attack: " << obj.attack << " of type: " << obj.attack_type << " hp: " << obj.hp << " id: " << obj.id;
    out << " initiative: " << obj.initiative << " units: " << obj.units << " effective power: " << obj.effective;
    if (not obj.immune.empty()){
        out << " Immune to: ";
        for (auto& c: obj.immune){
            out << c << " ";
        }
    }
    if (not obj.weak.empty()){
        out << " weak to: ";
        for (auto& c: obj.weak){
            out << c << " ";
        }
    }
    return out;
}

struct Compare
{
    bool operator() (const std::shared_ptr<Group>& a, const std::shared_ptr<Group>& b) const
    {
        return *a < *b;
    }
};

struct Compare_initiative
{
    bool operator() (const std::shared_ptr<Group>& a, const std::shared_ptr<Group>& b) const
    {
        return a->initiative > b->initiative;
    }
};

typedef std::set<std::shared_ptr<Group>, Compare> units;
typedef std::set<std::shared_ptr<Group>, Compare_initiative> units_initiative;
typedef std::map<std::shared_ptr<Group>, std::shared_ptr<Group>> matching;

bool finish_combat(const units& all_units){
    int infection = 0, immune = 0;
    for (auto& el: all_units){
        if (el->type == 'F'){
            infection++;
        }
        else{
            immune++;
        }
    }
    return (infection == 0 or immune == 0);
}

void attack_enemy(const std::shared_ptr<Group>& ptr, matching& attack_matching){
    std::shared_ptr<Group> enemy = attack_matching[ptr];
    int multiplier = 1;
    if (enemy->weak.find(ptr->attack_type) != enemy->weak.end()){
        multiplier = 2;
    }
    else if (enemy->immune.find(ptr->attack_type) != enemy->immune.end()){
        multiplier = 0;
    }
    int dmg = ptr->effective*multiplier;
    int units_killed = dmg/enemy->hp;
    enemy->units -= units_killed;
    enemy->effective = enemy->units*enemy->attack;
}

void find_move(const std::shared_ptr<Group>& ptr, units& all_groups, matching& attack_matching, units& selected_defenders){
    std::shared_ptr<Group> enemy = nullptr;
    int max_dmg = 0;
    for (const auto& el: all_groups){
        if (el->type == ptr->type or selected_defenders.find(el) != selected_defenders.end()){
            continue;
        }
        int multiplier = 1;
        if (el->weak.find(ptr->attack_type) != el->weak.end()){
            multiplier = 2;
        }
        else if (el->immune.find(ptr->attack_type) != el->immune.end()){
            multiplier = 0;
        }
        int dmg = ptr->effective*multiplier;
        if (dmg > max_dmg){
            max_dmg = dmg;
            enemy = el;
        }
    }
    if (enemy){
        selected_defenders.insert(enemy);
        attack_matching[ptr] = enemy;
    }
}

std::pair<std::set<std::string>, std::set<std::string>> parse_weaknesses(const std::string line){
    std::set<std::string> immune;
    std::set<std::string> weak;
    std::vector<std::string> parsed;
    std::string processed_line = line.substr(2, line.size()-3);
    boost::split(parsed, processed_line, boost::is_any_of(";"), boost::token_compress_on);
    boost::algorithm::split_regex(parsed, processed_line, boost::regex("; "));
    for (auto& c: parsed){
        std::vector<std::string> parsed_types;
        boost::split(parsed_types, c, boost::is_any_of(", "), boost::token_compress_on);
        std::string type = parsed_types[0];
        for (size_t i = 2; i<parsed_types.size(); i++){
            if (type == "weak"){
                weak.insert(parsed_types[i]);
            }
            else{
                immune.insert(parsed_types[i]);
            }
        }
    }
    return std::make_pair(immune, weak);
}

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    units unit_set;
    units unit_set_cp;
    units_initiative units_sorted_ini;
    int id = 0;
    std::getline(input_file, line);
    for(std::string line; std::getline(input_file, line);){
        if (line.empty()){
            break;
        }
        std::regex re("([0-9]+) units each with ([0-9]+) hit points(.*?) with an attack that does ([0-9]+) ([a-z]+) damage at initiative ([0-9]+)");
        std::smatch parts;
        std::regex_match(line, parts, re);
        int units = std::stoi(parts[1].str());
        int hp = std::stoi(parts[2].str());
        std::set<std::string> immune;
        std::set<std::string> weak;
        if (not parts[3].str().empty()){
            auto result = parse_weaknesses(parts[3].str());
            immune = result.first;
            weak = result.second;
        }
        int attack = std::stoi(parts[4].str());
        std::string type_attack = parts[5].str();
        int initiative = std::stoi(parts[6].str());
        Group group {initiative, units, type_attack, attack, hp, id, 'I', immune, weak};
        unit_set.insert(std::make_shared<Group>(group));
        id++;
    }
    std::getline(input_file, line);
    for(std::string line; std::getline(input_file, line);){
        if (line.empty()){
            break;
        }
        std::regex re("([0-9]+) units each with ([0-9]+) hit points(.*?) with an attack that does ([0-9]+) ([a-z]+) damage at initiative ([0-9]+)");
        std::smatch parts;
        std::regex_match(line, parts, re);
        int units = std::stoi(parts[1].str());
        int hp = std::stoi(parts[2].str());
        std::set<std::string> immune;
        std::set<std::string> weak;
        if (not parts[3].str().empty()){
            auto result = parse_weaknesses(parts[3].str());
            immune = result.first;
            weak = result.second;
        }
        int attack = std::stoi(parts[4].str());
        std::string type_attack = parts[5].str();
        int initiative = std::stoi(parts[6].str());
        Group group {initiative, units, type_attack, attack, hp, id, 'F', immune, weak};
        unit_set.insert(std::make_shared<Group>(group));
        id++;
    }
    input_file.close();
    for (const auto& el: unit_set){
        // create pointers from the set pointers, since
        // STL containers create copies
        units_sorted_ini.insert(std::shared_ptr<Group>(el));
    }
    int rounds = 1;
    while (true){
        matching group_to_attack;
        units selected_defenders;
        if (DEBUG) std::cout << "Starting round " << rounds << " with " << unit_set.size() << " groups " << std::endl;
        for (const auto& el: unit_set){
            find_move(el, unit_set, group_to_attack, selected_defenders);
        }
        for (const auto& el: units_sorted_ini){
            if (el->units < 1 or group_to_attack.find(el) == group_to_attack.end()) {
                // skip if unit was killed during the round
                continue;
            }
            attack_enemy(el, group_to_attack);
        }
        // clean dead units
        for (auto it = unit_set.begin(); it != unit_set.end(); it++) {
            if ((*it)->units < 1){
                it = unit_set.erase(it);
            }
        }
        for (auto it = units_sorted_ini.begin(); it != units_sorted_ini.end(); it++) {
            if ((*it)->units < 1){
                it = units_sorted_ini.erase(it);
            }
        }
        if (finish_combat(unit_set)){
            int remaining_units = 0;
            for (auto& unit: unit_set){
                remaining_units += unit->units;
            }
            if (DEBUG){
                std::cout << "Finishing combat in " << rounds << " with " << unit_set.size() << " groups " << std::endl;
                for (auto& unit: unit_set){
                    std::cout << *unit << std::endl;
                }
            }
            std::cout << "Finished combats with " << remaining_units << " units " << std::endl;
            return 0;
        }
        unit_set_cp.clear();
        for (const auto& el: unit_set){
            unit_set_cp.insert(std::shared_ptr<Group>(el));
        }
        unit_set.swap(unit_set_cp);
        rounds++;
    }
}
