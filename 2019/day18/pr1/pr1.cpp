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
typedef std::pair<int, seen_paths> info;
typedef std::tuple<int, int, seen_paths> info_short;
typedef std::unordered_map<info_short, int, boost::hash<info_short>> score_keeping;
typedef std::unordered_map<std::pair<int, int>, int, boost::hash<std::pair<int, int>>> map_paths;
typedef std::unordered_set<std::pair<int, int>, boost::hash<std::pair<int, int>>> seen_pos;
typedef std::unordered_map<std::pair<int, int>, std::pair<int, seen_paths>, boost::hash<std::pair<int, int>>> distances;

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
    moves.push(std::make_pair(0, doors));
    while (!moves.empty()){
        auto element = moves.top();
        int steps = std::get<0>(element);
        doors = std::get<1>(element);
        visited.insert(std::make_pair(y, x));
        moves.pop();
        for (auto& coords: {std::make_pair(y-1, x), std::make_pair(y+1, x), std::make_pair(y, x-1), std::make_pair(y, x+1)}){
            if (visited.find(coords) != visited.end()) continue;
            if (is_door(coords.first, coords.second, mapp)){
                doors.insert(mapp[coords]+32);
            }
            if (is_open(coords.first, coords.second, mapp)){
                if (mapp[coords] == key){
                    return std::make_pair(steps+1, doors);
                }
                moves.push(std::make_pair(steps+1, doors));
            }
        }
    }
    return std::make_pair(-1, doors);
}

bool is_blocked(seen_paths& keys, seen_paths& required_keys){
    for (int key: required_keys){
        if (keys.find(key) == keys.end()){
            return true;
        }
    }
    return false;
}


int score(int y, int x, seen_paths keys, score_keeping& scores, distances& dist_cache, map_paths& mapp, std::unordered_map<int, std::pair<int,int>>& start_coords, seen_paths& all_keys){
    if (scores.find(std::make_tuple(y, x, keys)) != scores.end()) return scores[std::make_tuple(y, x, keys)];
    int prev_key = mapp[std::make_pair(y, x)];
    if (prev_key == 46) prev_key = 0;
    int min_score = 1000000;
    if (keys == all_keys){
        return 0;
    }
    for (int key: all_keys){
        if (keys.find(key) != keys.end()){
            continue;
        }
        auto move_info = dist_cache[std::make_pair(prev_key, key)];
        int dist = move_info.first;
        seen_paths required_keys = move_info.second;
        if (is_blocked(keys, required_keys)) {
            continue;
        }
        int new_y = start_coords[key].first;
        int new_x = start_coords[key].second;
        seen_paths new_keys(keys);
        new_keys.insert(key);
        min_score = std::min(score(new_y, new_x, new_keys, scores, dist_cache, mapp, start_coords, all_keys)+dist, min_score);
    }
    scores[std::make_tuple(y, x, keys)] = min_score;
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
    seen_paths extended_keys(all_keys);
    extended_keys.insert(0);
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
            int dist = new_pos.first;
            seen_paths doors = new_pos.second;
            auto result = std::make_pair(dist, doors);
            dist_cache[std::make_pair(prev_key, key)] = result;
            dist_cache[std::make_pair(key, prev_key)] = result;
        }
    }
    seen_paths keys;
    score_keeping scores;
    std::cout << score(start_coords[0].first, start_coords[0].second, keys, scores, dist_cache, path, start_coords, all_keys) << std::endl;
}
