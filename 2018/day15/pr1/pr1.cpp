#include <set>
#include <map>
#include <list>
#include <queue>
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
typedef std::tuple<int, int, int> info;

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

void attack_enemy(location_map& enemies, std::shared_ptr<Unit>& ptr, unit_per_type& enemy_list, grid & map_grid){
    std::shared_ptr<Unit> weakest_enemy = nullptr;
    int hp_low = 300;
    std::vector<std::pair<int, int>> neighbors {std::make_pair(ptr->x-1, ptr->y), std::make_pair(ptr->x, ptr->y-1), std::make_pair(ptr->x, ptr->y+1), std::make_pair(ptr->x+1, ptr->y)};
    for (auto coords: neighbors){
        if (enemies.find(coords) != enemies.end()){
            if (enemies[coords]->type != ptr->type and enemies[coords]->hp < hp_low){
                weakest_enemy = enemies[coords];
                hp_low = weakest_enemy->hp;
            }
        }
    }
    if (weakest_enemy != nullptr){
        weakest_enemy->hp -= ptr->attack;
        if (weakest_enemy->hp < 1){
            auto coords = std::make_pair(weakest_enemy->x, weakest_enemy->y);
            int id = weakest_enemy->id;
            remove_ptr(enemy_list, id);
            enemies.erase(enemies.find(coords));
            map_grid[weakest_enemy->x][weakest_enemy->y] = '.';
        }
    }
}

info find_closest_path(int src_x, int src_y, int dest_x, int dest_y, const grid& map_grid){
    int max_x = map_grid.size();
    int max_y = map_grid[0].size();
    std::priority_queue<info, std::vector<info>, std::greater<info>> pos;
    pos.push(std::make_tuple(0, src_x, src_y));
    std::set<std::pair<int, int>> visited;
    while (!pos.empty()){
        int dist = std::get<0>(pos.top());
        int x = std::get<1>(pos.top());
        int y = std::get<2>(pos.top());
        pos.pop();
        visited.insert(std::make_pair(x, y));
        std::vector<std::pair<int, int>> neighbors {std::make_pair(x-1, y), std::make_pair(x, y-1), std::make_pair(x, y+1), std::make_pair(x+1, y)};
        for (auto c: neighbors){
            int xx = c.first;
            int yy = c.second;
            if (xx < 0 or xx >= max_x or yy < 0 or yy >= max_y or map_grid[xx][yy] != '.'){
                continue;
            }
            if (visited.find(c) != visited.end()){
                continue;
            }
            if (xx == dest_x and yy == dest_y){
                return std::make_tuple(dist+1, x, y);
            }
            pos.push(std::make_tuple(dist+1, xx, yy));
        }
    }
    return std::make_tuple(200, -1, -1);
}

bool is_in_range(std::shared_ptr<Unit>& ptr, grid& map_grid){
    std::vector<std::pair<int, int>> neighbors {std::make_pair(ptr->x-1, ptr->y), std::make_pair(ptr->x, ptr->y-1), std::make_pair(ptr->x, ptr->y+1), std::make_pair(ptr->x+1, ptr->y)};
    for (auto coords: neighbors){
        if (map_grid[coords.first][coords.second] != ptr->type and map_grid[coords.first][coords.second] != '.' and map_grid[coords.first][coords.second] != '#'){
            return true;
        }
    }
    return false;
}

std::pair<int, int> find_move(unit_per_type& enemy_list, std::shared_ptr<Unit>& ptr, grid& map_grid){
    std::priority_queue<info, std::vector<info>, std::greater<info>> closest_pos;
    closest_pos.push(std::make_tuple(100, -1, -1));
    if (is_in_range(ptr, map_grid)){
        return std::make_pair(-1, -1);
    }
    for (auto enemy_ptr: enemy_list){
        std::vector<std::pair<int, int>> neighbors {std::make_pair(enemy_ptr->x-1, enemy_ptr->y), std::make_pair(enemy_ptr->x, enemy_ptr->y-1), std::make_pair(enemy_ptr->x, enemy_ptr->y+1), std::make_pair(enemy_ptr->x+1, enemy_ptr->y)};
        for (auto coords: neighbors){
            if (map_grid[coords.first][coords.second] != '.'){
                // location is not open
                continue;
            }
            info found = find_closest_path(ptr->x, ptr->y, coords.first, coords.second, map_grid);
            if (std::get<0>(found) <= std::get<0>(closest_pos.top())){
                closest_pos.push(found);
            }
        }
    }
    auto found_coords = closest_pos.top();
    return std::make_pair(std::get<1>(found_coords), std::get<2>(found_coords));
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
        for (int j=0; j<(int)line.size(); j++){
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
        std::cout << "Starting round " << rounds << std::endl;
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
            std::cout << "Searching move for unit " << ptr_unit->id << " at pos "  << ptr_unit->x << "," << ptr_unit->y << std::endl;
            if (ptr_unit->type == 'E'){
                coords = find_move(goblins_list, ptr_unit, cavern);
            }
            else{
                coords = find_move(elf_list, ptr_unit, cavern);
            }
            if (coords.first != -1){
                auto old_coords = std::make_pair(ptr_unit->x, ptr_unit->y);
                cavern[ptr_unit->x][ptr_unit->y] = '.';
                ptr_unit->x = coords.first;
                ptr_unit->y = coords.second;
                cavern[ptr_unit->x][ptr_unit->y] = ptr_unit->type;
                units_map[coords] = units_map[old_coords];
                units_map.erase(units_map.find(old_coords));
            }
            std::cout << "Attacking with unit " << ptr_unit->id << " at pos "  << ptr_unit->x << "," << ptr_unit->y << std::endl;
            if (ptr_unit->type == 'E'){
                attack_enemy(units_map, ptr_unit, goblins_list, cavern);
            }
            else{
                attack_enemy(units_map, ptr_unit, elf_list, cavern);
            }
        }
        // clean dead units
        for (auto it = unit_set.begin(); it != unit_set.end(); it++) {
            if (it->hp < 1){
                unit_set.erase(it);
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
