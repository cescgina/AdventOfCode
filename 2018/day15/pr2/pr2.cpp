#include <set>
#include <map>
#include <list>
#include <queue>
#include <chrono>
#include <vector>
#include <string>
#include <memory>
#include <numeric>
#include <utility>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <unistd.h>

bool DEBUG = false;
typedef std::vector<char> row_map;
typedef std::vector<row_map> grid;
typedef std::vector<std::vector<int>> distances_grid;
typedef std::vector<std::vector<std::pair<int, int>>> path_grid;

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

struct Compare
{
    bool operator() (const std::shared_ptr<Unit>& a, const std::shared_ptr<Unit>& b) const
    {
        return *a < *b;
    }
};

int distance(int x1, int y1, int x2, int y2){
    return std::abs(x1-x2)+std::abs(y2-y1);
}

int index(int x, int y, int size_y){
    return x*size_y+y;
}

typedef std::set<std::shared_ptr<Unit>, Compare> units;
typedef std::map<std::pair<int,int>, std::shared_ptr<Unit>> location_map;
typedef std::list<std::shared_ptr<Unit>> unit_per_type;
typedef std::tuple<int, int, int> info;
typedef std::tuple<int, int, int, int, int> info_path;

bool finish_combat(const units& all_units){
    int elfs = 0, goblins = 0;
    for (auto& el: all_units){
        if (el->type == 'E'){
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
    for (const auto& coords: neighbors){
        if (enemies.find(coords) != enemies.end()){
            if (enemies[coords]->type != ptr->type and enemies[coords]->hp < hp_low){
                weakest_enemy = enemies[coords];
                hp_low = weakest_enemy->hp;
            }
        }
    }
    if (weakest_enemy != nullptr){
        weakest_enemy->hp -= ptr->attack;
        if (DEBUG) std::cout << "Hitting unit with id " << weakest_enemy->id << " and type " << weakest_enemy->type << " at pos " << weakest_enemy->x << "," << weakest_enemy->y << " now has hp: " << weakest_enemy->hp << std::endl;
        if (weakest_enemy->hp < 1){
            if (DEBUG) std::cout << "Unit with id " << weakest_enemy->id << " and type " << weakest_enemy->type << " is dead, hp: " << weakest_enemy->hp << std::endl;
            auto coords = std::make_pair(weakest_enemy->x, weakest_enemy->y);
            int id = weakest_enemy->id;
            remove_ptr(enemy_list, id);
            enemies.erase(enemies.find(coords));
            map_grid[weakest_enemy->x][weakest_enemy->y] = '.';
        }
    }
}

info find_closest_path(int src_x, int src_y, int dest_x, int dest_y, const grid& map_grid, int current_min_dist){
    int max_x = map_grid.size();
    int max_y = map_grid[0].size();
    std::priority_queue<info_path, std::vector<info_path>, std::greater<info_path>> pos;
    pos.push(std::make_tuple(0, src_x, src_y, -1, -1));
    std::set<std::pair<int, int>> visited;
    while (!pos.empty()){
        int dist = std::get<0>(pos.top());
        int x = std::get<1>(pos.top());
        int y = std::get<2>(pos.top());
        int dx = std::get<3>(pos.top());
        int dy = std::get<4>(pos.top());
        if (dist > current_min_dist){
            return std::make_tuple(200, -1, -1);
        }
        pos.pop();
        visited.insert(std::make_pair(x, y));
        std::vector<std::pair<int, int>> neighbors {std::make_pair(x-1, y), std::make_pair(x, y-1), std::make_pair(x, y+1), std::make_pair(x+1, y)};
        for (const auto& c: neighbors){
            int xx = c.first;
            int yy = c.second;
            if (xx < 0 or xx >= max_x or yy < 0 or yy >= max_y or map_grid[xx][yy] != '.'){
                continue;
            }
            if (visited.find(c) != visited.end()){
                continue;
            }
            if (xx == dest_x and yy == dest_y){
                if (dx == -1){
                    return std::make_tuple(dist+1, xx, yy);
                }
                else{
                    return std::make_tuple(dist+1, dx, dy);
                }
            }
            if (dx == -1){
                pos.push(std::make_tuple(dist+1, xx, yy, xx, yy));
            }
            else{
                pos.push(std::make_tuple(dist+1, xx, yy, dx, dy));
            }
        }
    }
    return std::make_tuple(200, -1, -1);
}

void calculate_distances(int x, int y, const grid& map_grid, distances_grid& calculated_distances, path_grid& moves_grid){
    int max_x = map_grid.size();
    int max_y = map_grid[0].size();
    std::queue<std::pair<int,int>> pos;
    pos.push(std::make_pair(x, y));
    calculated_distances[x][y] = 0;
    moves_grid[x][y] = std::make_pair(-1, -1);
    std::unordered_set<int> visited;
    while (!pos.empty()){
        int x = pos.front().first;
        int y = pos.front().second;
        visited.insert(index(x, y, max_y));
        pos.pop();
        int dx = 0;
        if (moves_grid[x][y].first == -1){
            dx = -1;
        }
        std::vector<std::pair<int, int>> neighbors {std::make_pair(x-1, y), std::make_pair(x, y-1), std::make_pair(x, y+1), std::make_pair(x+1, y)};
        for (const auto& c: neighbors){
            int xx = c.first;
            int yy = c.second;
            if (xx < 0 or xx >= max_x or yy < 0 or yy >= max_y or map_grid[xx][yy] != '.'){
                continue;
            }
            int index_value = index(xx, yy, max_y);
            if (visited.find(index_value) != visited.end()){
                continue;
            }
            visited.insert(index_value);
            calculated_distances[xx][yy] = calculated_distances[x][y]+1;
            pos.push(std::make_pair(xx, yy));
            if (dx == -1){
                moves_grid[xx][yy] = std::make_pair(xx, yy);
            }
            else{
                moves_grid[xx][yy] = moves_grid[x][y];
            }
        }
    }
}

bool is_in_range(const std::shared_ptr<Unit>& ptr, grid& map_grid){
    std::vector<std::pair<int, int>> neighbors {std::make_pair(ptr->x-1, ptr->y), std::make_pair(ptr->x, ptr->y-1), std::make_pair(ptr->x, ptr->y+1), std::make_pair(ptr->x+1, ptr->y)};
    for (const auto& coords: neighbors){
        if (map_grid[coords.first][coords.second] != ptr->type and map_grid[coords.first][coords.second] != '.' and map_grid[coords.first][coords.second] != '#'){
            return true;
        }
    }
    return false;
}

std::pair<int, int> find_move(unit_per_type& enemy_list, const std::shared_ptr<Unit>& ptr, grid& map_grid){
    int max_x = map_grid.size();
    int max_y = map_grid[0].size();
    std::priority_queue<info, std::vector<info>, std::greater<info>> closest_pos;
    closest_pos.push(std::make_tuple(100, -1, -1));
    std::unordered_set<int> visited;
    if (is_in_range(ptr, map_grid)){
        return std::make_pair(-1, -1);
    }
    distances_grid distances(max_x, std::vector<int>(max_y, 1000));
    path_grid moves;
    for (int ii = 0; ii < max_x; ii++){
        std::vector<std::pair<int, int>> row;
        for (int jj = 0; jj < max_y; jj++){
            row.push_back(std::make_pair(0, 0));
        }
        moves.push_back(row);
    }
    calculate_distances(ptr->x, ptr->y, map_grid, distances, moves);
    for (const auto& enemy_ptr: enemy_list){
        if (distance(ptr->x, ptr->y, enemy_ptr->x, enemy_ptr->y) > std::get<0>(closest_pos.top())){
            continue;
        }
        std::vector<std::pair<int, int>> neighbors {std::make_pair(enemy_ptr->x-1, enemy_ptr->y), std::make_pair(enemy_ptr->x, enemy_ptr->y-1), std::make_pair(enemy_ptr->x, enemy_ptr->y+1), std::make_pair(enemy_ptr->x+1, enemy_ptr->y)};
        for (const auto& coords: neighbors){
            int index_value = index(coords.first, coords.second, max_y);
            if (visited.find(index_value) != visited.end()){
                continue;
            }
            visited.insert(index_value);
            if (map_grid[coords.first][coords.second] != '.'){
                // location is not open
                continue;
            }
            if (distances[coords.first][coords.second] <= std::get<0>(closest_pos.top())){
                auto coords_move = moves[coords.first][coords.second];
                closest_pos.push(std::make_tuple(distances[coords.first][coords.second], coords_move.first, coords_move.second));
            }
        }
    }
    auto found_coords = closest_pos.top();
    return std::make_pair(std::get<1>(found_coords), std::get<2>(found_coords));
}

void print_grid(grid& grid_state){

    std::cout << std::endl;
    for (const auto& row: grid_state){
        for (const auto& point: row){
            std::cout << point;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int simulate_battle(units orig_unit_set, grid cavern, int attack){
    location_map units_map;
    unit_per_type goblins_list;
    unit_per_type elf_list;
    units unit_set_cp;
    for (const auto& el: orig_unit_set){
        unit_set_cp.insert(std::make_shared<Unit>(*el));
    }
    for (const auto& el: unit_set_cp){
        // create pointers from the set pointers, since
        // STL containers create copies
        units_map[std::make_pair(el->x, el->y)] = std::shared_ptr<Unit>(el);
        if (el->type == 'G'){
            goblins_list.push_back(std::shared_ptr<Unit>(el));
        }
        else{
            // elfs attack level
            el->attack = attack;
            elf_list.push_back(std::shared_ptr<Unit>(el));
        }
    }
    int rounds = 1;
    size_t original_elves = elf_list.size();
    while (true){
        if (DEBUG) std::cout << "Starting round " << rounds << " with " << unit_set_cp.size() << " units " << std::endl;
        if (elf_list.size() < original_elves){
            // an elf has died, not enough power
            if (DEBUG) std::cout << "An elf has died" << std::endl;
            return -1;
        }
        units round_order;
        // make sure we iterate in order
        for (const auto& el: unit_set_cp){
            round_order.insert(std::shared_ptr<Unit>(el));
        }
        for (auto ptr_unit: round_order){
            if (ptr_unit->hp < 1) {
                // skip if unit was killed during the round
                continue;
            }
            std::pair<int, int> coords;
            if (DEBUG) std::cout << "Searching move for unit " << ptr_unit->id << " of type " << ptr_unit->type << " at pos "  << ptr_unit->x << "," << ptr_unit->y;
            if (DEBUG) std::cout << std::endl;;
            if (ptr_unit->type == 'E'){
                coords = find_move(goblins_list, ptr_unit, cavern);
                if (goblins_list.size() == 0){
                    rounds--;
                    break;
                }
            }
            else{
                if (elf_list.size() == 0){
                    rounds--;
                    break;
                }
                coords = find_move(elf_list, ptr_unit, cavern);
            }
            if (coords.first != -1){
                if (DEBUG) std::cout << ": moving to " << coords.first << "," << coords.second << std::endl;
                auto old_coords = std::make_pair(ptr_unit->x, ptr_unit->y);
                cavern[ptr_unit->x][ptr_unit->y] = '.';
                ptr_unit->x = coords.first;
                ptr_unit->y = coords.second;
                cavern[ptr_unit->x][ptr_unit->y] = ptr_unit->type;
                units_map[coords] = units_map[old_coords];
                units_map.erase(units_map.find(old_coords));
            }
            else{
                if (DEBUG) std::cout << ": not moving " << std::endl;
            }
            if (DEBUG) std::cout << "Attacking with unit " << ptr_unit->id << " at pos "  << ptr_unit->x << "," << ptr_unit->y << std::endl;
            if (ptr_unit->type == 'E'){
                attack_enemy(units_map, ptr_unit, goblins_list, cavern);
            }
            else{
                attack_enemy(units_map, ptr_unit, elf_list, cavern);
            }
        }
        if (DEBUG) print_grid(cavern);
        // clean dead units
        for (auto it = unit_set_cp.begin(); it != unit_set_cp.end(); it++) {
            if ((*it)->hp < 1){
                if ((*it)->type == 'E'){
                    if (DEBUG) std::cout << "An elf has died" << std::endl;
                    return -1;
                }
                it = unit_set_cp.erase(it);
            }
        }
        if (finish_combat(unit_set_cp)){
            int remaining_hp = 0;
            for (auto& unit: unit_set_cp){
                remaining_hp += unit->hp;
            }
            return rounds*remaining_hp;
        }
        rounds++;
    }
}
int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    grid cavern;
    units unit_set;
    int i = 0, id = 0;
    for(std::string line; std::getline(input_file, line);){
        row_map row;
        for (int j=0; j<(int)line.size(); j++){
            if (line[j] == 'E'){
                // found elf
                Unit elf {i, j, 'E', 3, 200, id};
                unit_set.insert(std::make_shared<Unit>(elf));
                id++;
            }
            else if(line[j] == 'G'){
                // found goblin
                Unit goblin {i, j, 'G', 3, 200, id};
                unit_set.insert(std::make_shared<Unit>(goblin));
                id++;
            }
            row.push_back(line[j]);
        }
        cavern.push_back(row);
        i++;
    }
    input_file.close();
    int attack_level = 4;
    int outcome = 0;
    while (true){
        outcome = simulate_battle(unit_set, cavern, attack_level);
        if (outcome > 0){
            std::cout << "Finished combats of power " << attack_level << " with " << outcome << std::endl;
            return 0;
        }
        attack_level++;
    }
}
