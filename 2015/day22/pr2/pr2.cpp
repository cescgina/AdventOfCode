#include <map>
#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>

typedef std::map<std::string,std::vector<int>> items;

int apply_spell(std::string spell, std::list<std::vector<int>> effects_arr, int hp_pl, int mana_pl, int armor_pl, int hp_boss, int dmg_boss, int minim_mana, items & spells_info, int spent_mana, std::vector<std::string> & list_spells);
bool DEBUG = true;


template <typename T>
void print_vector(std::vector<T>& vec){
    for (auto it=vec.begin(); it != vec.end(); it++){
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

int boss_turn(std::list<std::vector<int>> effects_arr, int hp_pl, int mana_pl, int armor_pl, int hp_boss, int dmg_boss, int minim_mana, int spent_mana, std::vector<std::string> & list_spells, items & spells_info){
    if (DEBUG){
        std::cout << "Boss turn " << spent_mana << std::endl;
    }
    // apply effects
    int armor_increase = 0;
    for (auto it=effects_arr.begin(); it != effects_arr.end(); it++){
        std::vector<int> arr = *it;
        if (arr[0] == 2){
            armor_increase = 7;
        }
        else if (arr[0] == 3){
            hp_boss -= 3;
            if (hp_boss < 1){
                if (DEBUG){
                    std::cout << "Boss killed with " << spent_mana << " mana!" << std::endl;
                }
                return spent_mana;
            }
        }
        else if (arr[0] == 4){
            mana_pl += 101;
        }
        arr[1]--;
        *it = arr;
    }
    // delete old effects
    effects_arr.remove_if([](std::vector<int> x){ return x[1] == 0; });
    hp_pl -= std::max(1, dmg_boss-(armor_increase+armor_pl));
    if (hp_pl < 1 ){
        return 1000000000;
    }
    if (mana_pl < 53){
        // can't use any spell, player loses
        return 1000000000;
    }
    for (auto it=list_spells.begin(); it != list_spells.end(); it++){
        minim_mana = std::min(minim_mana, apply_spell(*it, effects_arr, hp_pl, mana_pl, armor_pl, hp_boss, dmg_boss, minim_mana, spells_info, spent_mana, list_spells));
    }
    return minim_mana;
}

int apply_spell(std::string spell, std::list<std::vector<int>> effects_arr, int hp_pl, int mana_pl, int armor_pl, int hp_boss, int dmg_boss, int minim_mana, items & spells_info, int spent_mana, std::vector<std::string> & list_spells){
    hp_pl -= 1;
    if (hp_pl < 1 ){
        return 1000000000;
    }
    if (DEBUG){
        std::cout << "Applying spell " << spell << " " << hp_pl << " " << hp_boss << " " << spent_mana << std::endl;
    }
    if (mana_pl < spells_info[spell][0]){
        // can't use this spell
        if (DEBUG){
            std::cout << "Not enough mana!" << std::endl;
        }
        return 1000000000;
    }
    // apply effects
    for (auto it=effects_arr.begin(); it != effects_arr.end(); it++){
        std::vector<int> arr = *it;
        if (arr[1] == 0){
            continue;
        }
        if (arr[0] == 2){
            armor_pl += 0;
        }
        else if (arr[0] == 3){
            hp_boss -= 3;
            if (hp_boss < 1){
                if (DEBUG){
                    std::cout << "Boss killed with " << spent_mana << " mana!" << std::endl;
                }
                return spent_mana;
            }
        }
        else if (arr[0] == 4){
            mana_pl += 101;
        }
        arr[1]--;
        *it = arr;
    }
    // delete old effects
    effects_arr.remove_if([](std::vector<int> x){ return x[1] == 0; });
    for (auto it=effects_arr.begin(); it != effects_arr.end(); it++){
        std::vector<int> arr = *it;
        if (list_spells[arr[0]] == spell){
            // can't apply spell
            return 1000000000;
        }
    }
    if (spell == "Magic Missile"){
       spent_mana += 53; 
       mana_pl -= 53;
       hp_boss -= 4;
    }
    else if (spell == "Drain"){
       spent_mana += 73; 
       mana_pl -= 73;
       hp_boss -= 2;
       hp_pl += 2;
    }
    else if (spell == "Shield"){
       spent_mana += 113; 
       mana_pl -= 113;
       effects_arr.push_back(std::vector<int> {2, 6});
    }
    else if (spell == "Poison"){
       spent_mana += 173; 
       mana_pl -= 173;
       effects_arr.push_back(std::vector<int> {3, 6});
    }
    else if (spell == "Recharge"){
       spent_mana += 229; 
       mana_pl -= 229;
       effects_arr.push_back(std::vector<int> {4, 5});
    }
    if (spent_mana > minim_mana){
        // prune early
        return 1000000000;
    }
    if (hp_boss < 1){
        if (DEBUG){
            std::cout << "Boss killed with " << spent_mana << " mana!" << std::endl;
        }
        return spent_mana;
    }
    return std::min(minim_mana, boss_turn(effects_arr, hp_pl, mana_pl, armor_pl, hp_boss, dmg_boss, minim_mana, spent_mana, list_spells, spells_info));
}

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    int boss_hp = 0, boss_dmg = 0;
    int hp = 50, mana = 500, armor = 0;
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::getline(input_file, line);
    auto pos = line.find(":");
    boss_hp = std::stoi(line.substr(pos+2));
    std::getline(input_file, line);
    pos = line.find(":");
    boss_dmg = std::stoi(line.substr(pos+2));
    input_file.close();
    items spells;
    // spell: (mana cost, damage, health, armor, effect duration, mana gain)
    spells["Magic Missile"] = std::vector<int>({53, 4, 0, 0, 0, 0});
    spells["Drain"] = std::vector<int>({73, 2, 2, 0, 0, 0});
    spells["Shield"] = std::vector<int>({113, 0, 0, 7, 6, 0});
    spells["Poison"] = std::vector<int>({173, 3, 0, 0, 6, 0});
    spells["Recharge"] = std::vector<int>({229, 0, 0, 0, 5, 101});
    std::vector<std::string> spell_list({ "Magic Missile", "Drain", "Shield", "Poison", "Recharge" });
    std::list<std::vector<int>> effects;
    int min_mana = 100000000, mana_spent_branch = 0;
    for (auto it=spell_list.begin(); it != spell_list.end(); it++){
        if (DEBUG){
            std::cout << "Player: " << hp << " " << mana << " Boss: " << boss_hp << std::endl;
        }
        mana_spent_branch = apply_spell(*it, effects, hp, mana, armor, boss_hp, boss_dmg, min_mana, spells, 0, spell_list);
        min_mana = std::min(min_mana, mana_spent_branch);
        if (DEBUG){
            std::cout << "Minimum for this branch: " << mana_spent_branch << std::endl;
            std::cout << "///////////////////////////////////////////" << std::endl;
        }
    }
    std::cout << "Minimum mana needed is " << min_mana << std::endl;
}
