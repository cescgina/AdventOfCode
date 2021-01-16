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

typedef std::unordered_map<std::pair<int, int>, int, boost::hash<std::pair<int, int>>> map_paths;
typedef std::unordered_set<std::pair<int, int>, boost::hash<std::pair<int, int>>> seen_pos;
typedef std::set<int> seen_paths;
typedef std::unordered_set<std::pair<seen_paths, int>, boost::hash<std::pair<seen_paths, int>>> to_prune;
typedef std::unordered_map<std::pair<int, int>, std::pair<int, seen_paths>, boost::hash<std::pair<int, int>>> distances;
typedef std::tuple<int, int, int, seen_paths> info;
typedef std::tuple<int, int, seen_paths> info_short;
typedef std::tuple<std::pair<int, int>, std::pair<int, int>,std::pair<int, int>,std::pair<int, int>, seen_paths> info_long;
typedef std::unordered_map<info_long, int, boost::hash<info_long>> score_keeping;

bool is_key(int y, int x, map_paths& mapp){
    int ascii = mapp[std::make_pair(y, x)];
    return (ascii > 96 and ascii < 123);
}

bool is_door(int y, int x, map_paths& mapp){
    int ascii = mapp[std::make_pair(y, x)];
    return (ascii > 64 and ascii < 91);
}

bool is_open(int y, int x, map_paths& mapp){
    int ascii = mapp[std::make_pair(y, x)];
    return ascii == 46 or is_door(y, x, mapp) or is_key(y, x, mapp);
}

std::ostream& operator<<(std::ostream& out, seen_paths& keys){
    for (auto& c: keys){
        out << char(c) << " ";
    }
    return out;
}

info find_distance(int y, int x, int key, map_paths& mapp){
    std::priority_queue<info, std::vector<info>, std::greater<info>> moves;
    seen_pos visited;
    seen_paths doors;
    moves.push(std::make_tuple(0, y, x, doors));
    while (!moves.empty()){
        auto element = moves.top();
        int steps = std::get<0>(element);
        y = std::get<1>(element);
        x = std::get<2>(element);
        doors = std::get<3>(element);
        visited.insert(std::make_pair(y, x));
        moves.pop();
        for (auto& coords: {std::make_pair(y-1, x), std::make_pair(y+1, x), std::make_pair(y, x-1), std::make_pair(y, x+1)}){
            if (visited.find(coords) != visited.end()) continue;
            if (is_open(coords.first, coords.second, mapp)){
                seen_paths new_doors(doors);
                if (is_door(coords.first, coords.second, mapp)){
                    new_doors.insert(mapp[coords]+32);
                }
                if (mapp[coords] == key){
                    return std::make_tuple(steps+1, coords.first, coords.second, new_doors);
                }
                moves.push(std::make_tuple(steps+1, coords.first, coords.second, new_doors));
            }
        }
    }
    return std::make_tuple(-1, 0, 0, doors);
}

bool is_blocked(seen_paths& keys, seen_paths& required_keys){
    for (int key: required_keys){
        if (keys.find(key) == keys.end()){
            return true;
        }
    }
    return false;
}

void create_multiple_starts(map_paths& mapp, std::unordered_map<int, std::pair<int,int>>& start_coords){
    int start_y = start_coords[0].first;
    int start_x = start_coords[0].second;
    mapp[start_coords[0]] = '#';
    mapp[std::make_pair(start_y, start_x-1)] = '#';
    mapp[std::make_pair(start_y, start_x+1)] = '#';
    mapp[std::make_pair(start_y-1, start_x)] = '#';
    mapp[std::make_pair(start_y+1, start_x)] = '#';
    mapp[std::make_pair(start_y-1, start_x-1)] = '.';
    mapp[std::make_pair(start_y-1, start_x+1)] = '.';
    mapp[std::make_pair(start_y+1, start_x-1)] = '.';
    mapp[std::make_pair(start_y+1, start_x+1)] = '.';
    start_coords[0] = std::make_pair(start_y-1, start_x-1);
    start_coords[1] = std::make_pair(start_y-1, start_x+1);
    start_coords[2] = std::make_pair(start_y+1, start_x-1);
    start_coords[3] = std::make_pair(start_y+1, start_x+1);
}

int score(info_long state, score_keeping& scores, distances& dist_cache, map_paths& mapp, std::unordered_map<int, std::pair<int,int>>& start_coords, seen_paths& all_keys){
    if (scores.find(state) != scores.end()) return scores[state];
    int min_score = 1000000;
    seen_paths keys = std::get<4>(state);
    if (keys == all_keys){
        return 0;
    }
    std::vector<std::pair<int,int>> positions = {std::get<0>(state), std::get<1>(state), std::get<2>(state), std::get<3>(state)};
    for (int robot=0; robot<4; robot++){
        auto coords = positions[robot];
        int prev_key = mapp[coords];
        if (prev_key == 46) prev_key = robot;
        for (int key: all_keys){
            if (keys.find(key) != keys.end()){
                continue;
            }
            auto move_info = dist_cache[std::make_pair(prev_key, key)];
            int dist = move_info.first;
            seen_paths required_keys = move_info.second;
            if (dist == -1 or is_blocked(keys, required_keys)) {
                continue;
            }
            int new_y = start_coords[key].first;
            int new_x = start_coords[key].second;
            seen_paths new_keys(keys);
            new_keys.insert(key);
            auto new_positions(positions);
            new_positions[robot] = std::make_pair(new_y, new_x);
            info_long new_state = std::make_tuple(new_positions[0], new_positions[1], new_positions[2], new_positions[3], new_keys);
            min_score = std::min(score(new_state, scores, dist_cache, mapp, start_coords, all_keys)+dist, min_score);
        }
    }
    scores[state] = min_score;
    return min_score;
}
int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    int x = 0, y = 0;
    int i = 0;
    map_paths path;
    std::ifstream input_file;
    input_file.open(file_name);
    seen_paths all_keys;
    std::unordered_map<int, std::pair<int,int>> start_coords;
    for (std::string line; std::getline(input_file, line);){
        for (int j=0; j<(int)line.size(); j++){
            int ascii = (int)line[j];
            if (ascii > 96 and ascii < 123){
                all_keys.insert(ascii);
                start_coords[ascii] = std::make_pair(i, j);
            }
            if (line[j] == '@'){
                start_coords[0] = std::make_pair(i, j);
                path[std::make_pair(i, j)] = (int)'.';
            }
            else{
                path[std::make_pair(i, j)] = ascii;
            }
        }
        i++;
    }
    create_multiple_starts(path, start_coords);
    seen_paths extended_keys(all_keys);
    extended_keys.insert(0);
    extended_keys.insert(1);
    extended_keys.insert(2);
    extended_keys.insert(3);
    input_file.close();
    distances dist_cache;
    for (int prev_key: extended_keys){
        for (int key: all_keys){
            if (dist_cache.find(std::make_pair(prev_key, key)) != dist_cache.end() or key == prev_key){
                continue;
            }
            x = start_coords[prev_key].second;
            y = start_coords[prev_key].first;
            auto new_pos = find_distance(y, x, key, path);
            int dist = std::get<0>(new_pos);
            seen_paths doors = std::get<3>(new_pos);
            auto result = std::make_pair(dist, doors);
            dist_cache[std::make_pair(prev_key, key)] = result;
            dist_cache[std::make_pair(key, prev_key)] = result;
        }
    }
    seen_paths keys;
    score_keeping scores;
    info_long init_state = std::make_tuple(start_coords[0], start_coords[1], start_coords[2], start_coords[3], keys);
    std::cout << score(init_state, scores, dist_cache, path, start_coords, all_keys) << std::endl;
}
