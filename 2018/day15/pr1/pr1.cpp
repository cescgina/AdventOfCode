#include <set>
#include <map>
#include <list>
#include <vector>
#include <string>
#include <memory>
#include <numeric>
#include <utility>
#include <fstream>
#include <iostream>
#include <unistd.h>

typedef std::vector<char> row_map;
typedef std::vector<row_map> grid;

struct Unit{
    int x;
    int y;
    char type;
    int attack;
    int hp;
    int id;
    bool operator<(const Unit &other) const{
        if (x == other.x){
            return y < other.y;
        }
        else{
            return x < other.x;
        }
    }
};

typedef std::set<Unit> units;
typedef std::map<std::pair<int,int>, std::shared_ptr<Unit>> location_map;
typedef std::list<std::shared_ptr<Unit>> unit_per_type;

bool finish_combat(const units& all_units){
    int elfs = 0, goblins = 0;
    for (auto el: all_units){
        if (el.type == 'E'){
            elfs++;
        }
        else{
            goblins++;
        }
    }
    return (elfs == 0 or goblins == 0);
}

void remove_ptr(unit_per_type& list_to_remove, const int id_to_remove){
    list_to_remove.remove_if([id_to_remove](std::shared_ptr<Unit> x){ return x->id == id_to_remove; });
}

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    grid cavern;
    location_map units_map;
    unit_per_type goblins_list;
    unit_per_type elf_list;
    units unit_set;
    int i = 0, id = 0;
    for(std::string line; std::getline(input_file, line);){
        row_map row;
        for (int j=0; j<line.size(); j++){
            if (line[j] == 'E'){
                // found elf
                Unit elf {i, j, 'E', 3, 200, id};
                unit_set.insert(elf);
                units_map[std::make_pair(i, j)] = std::make_shared<Unit>(elf);
                elf_list.push_back(std::make_shared<Unit>(elf));
                id++;
            }
            else if(line[j] == 'G'){
                // found goblin
                Unit goblin {i, j, 'G', 3, 200, id};
                unit_set.insert(goblin);
                units_map[std::make_pair(i, j)] = std::make_shared<Unit>(goblin);
                goblins_list.push_back(std::make_shared<Unit>(goblin));
                id++;
            }
            row.push_back(line[j]);
        }
        cavern.push_back(row);
        i++;
    }
    input_file.close();
    int rounds = 0;
    while (true){
        unit_per_type round_order;
        // make sure we iterate in order
        for (auto el: unit_set){
            round_order.push_back(std::make_shared<Unit>(el));
        }
        for (auto ptr_unit: round_order){
            if (ptr_unit->hp < 1) {
                // skip if unit was killed during the round
                continue;
            }
            std::pair<int, int> coords;
            if (ptr_unit->type == 'E'){
                coords = find_move(goblins_list, ptr_unit);
            }
            else{
                coords = find_move(elf_list, ptr_unit);
            }
            if (coords.first != -1){
                auto old_coords = std::make_pair(ptr_unit->x, ptr_unit->y);
                ptr_unit->x = coords.first;
                ptr_unit->y = coords.second;
                units_map[coords] = units_map[old_coords];
                units_map.erase(units_map.find(old_coords));
            }
            if (ptr_unit->type == 'E'){
                attack(units_map, ptr_unit);
            }
            else{
                attack(units_map, ptr_unit);
            }
        }
        // clean dead units
        for (auto it = unit_set.begin(); it != unit_set.end(); it++) {
            if (it->hp < 1){
                auto coords = std::make_pair(it->x, it->y);
                char type = it->type;
                int id = it->id;
                if (type == 'E'){
                    remove_ptr(elf_list, id);
                } 
                else{
                    remove_ptr(goblins_list, id);
                }
                unit_set.erase(it);
                units_map.erase(units_map.find(coords));
            }
        }
        if (finish_combat(unit_set)){
            int remaining_hp = 0;
            for (auto unit: unit_set){
                remaining_hp += unit.hp;
            }
            std::cout << "Finished combats with " << rounds*remaining_hp << std::endl;
            return 0;
        }
        rounds++;
    }
}
