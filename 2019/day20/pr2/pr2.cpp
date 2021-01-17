#include <stack>
#include <queue>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <unistd.h>
#include <boost/functional/hash.hpp>

bool DEBUG = false;

typedef std::set<int> seen_paths;
typedef std::pair<int, int> info;
typedef std::tuple<int, int, int> pos_priority;
typedef std::unordered_map<std::pair<int, int>, int, boost::hash<std::pair<int, int>>> map_paths;
typedef std::unordered_set<std::pair<int, int>, boost::hash<std::pair<int, int>>> seen_pos;
typedef std::unordered_map<int, std::vector<std::pair<int, int>>> distances;
typedef std::unordered_map<int, std::string> lookup_portals;
typedef std::unordered_map<int, int> portal_jump;
typedef std::unordered_map<std::string, std::vector<int>> revers_lookup_portals;

bool is_portal_char(int ascii){
    return (ascii > 64 and ascii < 91);
}
bool is_portal(int y, int x, map_paths& mapp){
    int ascii = mapp[std::make_pair(y, x)];
    return ascii < 32;
}

bool is_portal_open(int y, int x, map_paths& mapp, int level, lookup_portals& dict){
    int ascii = mapp[std::make_pair(y, x)];
    if (level == 0){
        if (ascii < 32){
            return dict[ascii] != "AA" and dict[ascii] != "ZZ";
        }
        else{
            return ascii == 46;
        }
    }
    else{
        if (ascii < 32){
            return dict[ascii] != "AA" and dict[ascii] != "ZZ";
        }
        else{
            return ascii == 46;
        }
    }
}

bool is_open(int y, int x, map_paths& mapp){
    int ascii = mapp[std::make_pair(y, x)];
    return ascii == 46;
}

std::ostream& operator<<(std::ostream& out, seen_paths& keys){
    for (auto& c: keys){
        out << char(c) << " ";
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const std::pair<int, int>& p){
    out << p.first << "," << p.second;
    return out;
}

void find_distances(int y, int x, int key, map_paths& mapp, distances& dist_cache){
    std::priority_queue<pos_priority, std::vector<pos_priority>, std::greater<pos_priority>> moves;
    seen_pos visited;
    moves.push(std::make_tuple(0, y, x));
    while (!moves.empty()){
        auto element = moves.top();
        int steps = std::get<0>(element);
        y = std::get<1>(element);
        x = std::get<2>(element);
        visited.insert(std::make_pair(y, x));
        moves.pop();
        for (auto& coords: {std::make_pair(y-1, x), std::make_pair(y+1, x), std::make_pair(y, x-1), std::make_pair(y, x+1)}){
            if (visited.find(coords) != visited.end()) continue;
            if (is_portal(coords.first, coords.second, mapp)){
                int prev_key = mapp[coords];
                dist_cache[key].push_back(std::make_pair(prev_key, steps+1));
            }
            if (is_open(coords.first, coords.second, mapp)){
                moves.push(std::make_tuple(steps+1, coords.first, coords.second));
            }
        }
    }
}

std::string parse_portal(map_paths& mapp, std::unordered_map<int, std::pair<int,int>>& start_coords, int portal_ind, lookup_portals& portal_dict, revers_lookup_portals& rev_dict, int y, int x, std::vector<std::string>& lines){
    int portal_x = x, portal_y = y;
    int orig_x = x, orig_y = y;
    std::string name;
    name.push_back(lines[y][x]);
    bool has_open_neih = false;
    for (auto neighbor: {std::make_pair(y-1,x), std::make_pair(y+1,x), std::make_pair(y,x-1), std::make_pair(y,x+1)}){
        int neigh_char = lines[neighbor.first][neighbor.second];
        if (neigh_char == 46){
            has_open_neih = true;
            y = neighbor.first;
            x = neighbor.second;
        }
        else if (is_portal_char(neigh_char)){
            portal_y = neighbor.first;
            portal_x = neighbor.second;
            name.push_back(neigh_char);
        }
    }
    if (not has_open_neih){
        return "";
    }
    std::sort(name.begin(), name.end());
    portal_dict[portal_ind] = name;
    if (DEBUG) std::cout << portal_ind << " " << name << " " << y << "," << x << std::endl;
    rev_dict[name].push_back(portal_ind);
    mapp[std::make_pair(portal_y, portal_x)] = 32;
    mapp[std::make_pair(orig_y, orig_x)] = 32;
    mapp[std::make_pair(y, x)] = portal_ind;
    start_coords[portal_ind] = std::make_pair(y, x);
    return name;
}

int shortest_path(distances& dist_cache, int start_index, int end_index, seen_paths& inner_portals, seen_paths& outer_portals, portal_jump& jumps){
    std::priority_queue<pos_priority, std::vector<pos_priority>, std::greater<pos_priority>> moves;
    seen_pos visited;
    moves.push(std::make_tuple(0, 0, start_index));
    while (!moves.empty()){
        auto element = moves.top();
        int steps = std::get<0>(element);
        int level = std::get<1>(element);
        int node = std::get<2>(element);
        moves.pop();
        visited.insert(std::make_pair(node, level));
        for (auto& element: dist_cache[node]){
            if (DEBUG) std::cout << node << " " << element.first << " " << element.second << std::endl;
            if (visited.find(std::make_pair(element.first, level)) != visited.end()) continue;
            if (element.first == end_index and level == 0){
                return steps+element.second;
            }
            if (level == 0){
                if (outer_portals.find(element.first) != outer_portals.end()) continue;
                if (inner_portals.find(element.first) != inner_portals.end()){
                    moves.push(std::make_tuple(steps+element.second+1, level+1, jumps[element.first]));
                }
            }
            else{
                if (element.first == start_index or element.first == end_index) continue;
                if (inner_portals.find(element.first) != inner_portals.end()){
                    moves.push(std::make_tuple(steps+element.second+1, level+1, jumps[element.first]));
                }
                else if (outer_portals.find(element.first) != inner_portals.end()){
                    moves.push(std::make_tuple(steps+element.second+1, level-1, jumps[element.first]));
                }
            }
        }
    }
    return 1000000;
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    int start_index = 0, end_index = 0;
    int portal_index = 9;
    map_paths path;
    std::ifstream input_file;
    input_file.open(file_name);
    seen_paths all_keys;
    std::unordered_map<int, std::pair<int,int>> start_coords;
    std::vector<std::string> lines;
    lookup_portals portal_dict;
    revers_lookup_portals reverse_portal_dict;
    for (std::string line; std::getline(input_file, line);){
        lines.push_back(line);
    }
    input_file.close();
    int max_y = lines.size()-3;
    int max_x = lines[2].size()-1;
    for (int i=1; i<(int)lines.size()-1; i++){
        std::string line = lines[i];
        for (int j=0; j<(int)line.size(); j++){
            int ascii = (int)line[j];
            if (is_portal_char(ascii)){
                std::string portal_name = parse_portal(path, start_coords, portal_index, portal_dict, reverse_portal_dict, i, j, lines);
                if (portal_name == "") continue;
                if (portal_name == "AA"){
                    start_index = portal_index;
                }
                else if (portal_name == "ZZ"){
                    end_index = portal_index;
                }
                portal_index--;
            }
            else{
                path[std::make_pair(i, j)] = ascii;
            }
        }
    }
    seen_paths outer_portals, inner_portals;
    portal_jump jumps;
    for (auto& element: reverse_portal_dict){
        if (element.first == "AA" or element.first == "ZZ") continue;
        int ind_1 = element.second[0];
        int ind_2 = element.second[1];
        auto coords_1 = start_coords[ind_1];
        jumps[ind_1] = ind_2;
        jumps[ind_2] = ind_1;
        if (coords_1.first == 2 or coords_1.first == max_y or coords_1.second == 2 or coords_1.second == max_x){
            outer_portals.insert(ind_1);
            inner_portals.insert(ind_2);
        }
        else{
            outer_portals.insert(ind_2);
            inner_portals.insert(ind_1);
        }
    }

    for (auto& element: start_coords){
        path[element.second] = element.first;
    }
    distances dist_cache;
    if (DEBUG){
        for (int i=1; i<(int)lines.size()-1; i++){
            std::string line = lines[i];
            for (int j=0; j<(int)line.size(); j++){
                int value = path[std::make_pair(i, j)];
                if (value > 30) std::cout << char(value);
                else std::cout << value;
            }
            std::cout << std::endl;
        }
    }

    for (auto element: start_coords){
        int x = element.second.second;
        int y = element.second.first;
        find_distances(y, x, element.first, path, dist_cache);
    }
    if (DEBUG){
        for (auto& element: dist_cache){
            std::cout << element.first << " ";
            for (auto& c: element.second){
                std::cout << c << " ";
            }
            std::cout << std::endl;
        }
    }
    std::cout << "Finding shortest path: ";
    std::cout << shortest_path(dist_cache, start_index, end_index, inner_portals, outer_portals, jumps) << std::endl;
}
