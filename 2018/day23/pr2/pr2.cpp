#include <tuple>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <unistd.h>

struct nanobot{
    int id;
    int x;
    int y;
    int z;
    int radius;
};
typedef std::vector<nanobot> nanobots;
typedef std::vector<int> coordinates;
typedef std::tuple<int, int, int, int, int> info;
typedef std::vector<info> results;

void print_info(const results& list){
    std::cout << "[";
    for (auto& best: list){
        std::cout << "(" << std::get<0>(best);
        std::cout << ", " << std::get<1>(best);
        std::cout << ", " << std::get<2>(best);
        std::cout << ", " << std::get<3>(best);
        std::cout << ", " << std::get<4>(best) << "), ";
    }
    std::cout<< "]" << std::endl;
}
int distance(nanobot& nano1, nanobot& nano2){
    return std::abs(nano1.x-nano2.x)+std::abs(nano1.y-nano2.y)+std::abs(nano1.z-nano2.z);
}

int distance(nanobot& nano1, int x, int y, int z){
    return std::abs(nano1.x-x)+std::abs(nano1.y-y)+std::abs(nano1.z-z);
}

int distance(int x, int y, int z, int x1, int y1, int z1){
    return std::abs(x1-x)+std::abs(y1-y)+std::abs(z1-z);
}

int distance(int x, int y, int z){
    return std::abs(x)+std::abs(y)+std::abs(z);
}

bool is_in_range(nanobot& nano1, int x, int y, int z){
    return distance(nano1, x, y, z) <= nano1.radius;
}

std::pair<int, int> find(const nanobots& bots, coordinates xs, coordinates ys, coordinates zs, int dist, int forced_count, int ox, int oy, int oz){
    results at_target;

    auto result = std::minmax_element(xs.begin(), xs.end());
    int min_x = *result.first, max_x = *result.second;
    result = std::minmax_element(ys.begin(), ys.end());
    int min_y = *result.first, max_y = *result.second;
    result = std::minmax_element(zs.begin(), zs.end());
    int min_z = *result.first, max_z = *result.second;
               
    for (int x = min_x; x <= max_x; x += dist){
        for (int y = min_y; y <= max_y; y += dist){
            for (int z = min_z; z <= max_z; z += dist){
                int count = 0;
                for (auto bot: bots){
                    if (dist == 1){
                        count += is_in_range(bot, x, y, z); 
                    }
                    else{
                        int calc = std::abs((ox+x)-(ox+bot.x));
                        calc += std::abs((oy+y)-(oy+bot.y));
                        calc += std::abs((oz+z)-(oz+bot.z));
                        if (((calc / dist)-3) <= (bot.radius / dist)) count++;
                    }
                }
                if (count >= forced_count){
                    at_target.push_back(std::make_tuple(x, y, z, count, distance(x, y, z)));
                }
            }
        }
    }
    while (not at_target.empty()){
        info best;
        int best_i = -1;
        int i = 0;
        for (auto& possible: at_target){
            if (best_i == -1 or std::get<4>(possible) < std::get<4>(best)){
                best = possible;
                best_i = i;
            }
            i++;
        }
        if (dist == 1){
            return std::make_pair(std::get<4>(best), std::get<3>(best));
        }
        else{
            int xx = std::get<0>(best), yy = std::get<1>(best), zz = std::get<2>(best);
            xs = {xx, xx+(dist/2)};
            ys = {yy, yy+(dist/2)};
            zs = {zz, zz+(dist/2)};
            auto found_result = find(bots, xs, ys, zs, dist / 2, forced_count, ox, oy, oz);
            if (found_result.first == -1){
                at_target.erase(at_target.begin()+best_i);
            }
            else{
                return found_result;
            }
        }
    }

    return std::make_pair(-1, -1);
}
nanobot parse_line(const std::string& line, int id_nano){
    int x, y, z, radius;
    auto pos = line.find("<");
    auto pos_end = line.find(">");
    radius = std::stoi(line.substr(pos_end+5));
    std::string coords = line.substr(pos+1, pos_end-pos);
    pos = coords.find(",");
    x = std::stoi(coords.substr(0,pos));
    pos_end = coords.find(",", pos+1);
    y = std::stoi(coords.substr(pos+1, pos_end-pos));
    z = std::stoi(coords.substr(pos_end+1));
    return nanobot{id_nano, x, y, z, radius};
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    int id = 0;
    nanobots all_nanobots;
    coordinates xs, ys, zs;
    int max_x = 0, max_y = 0, max_z = 0, min_x = 100000000, min_y = min_x, min_z = min_x;
    for (std::string line; std::getline(input_file, line);){
        nanobot nano = parse_line(line, id);
        xs.push_back(nano.x);
        ys.push_back(nano.y);
        zs.push_back(nano.z);
        max_x = std::max(nano.x, max_x);
        max_y = std::max(nano.y, max_y);
        max_z = std::max(nano.z, max_z);
        min_x = std::min(nano.x, min_x);
        min_y = std::min(nano.y, min_y);
        min_z = std::min(nano.z, min_z);
        all_nanobots.push_back(nano);
        id++;
    }
    input_file.close();
    xs.push_back(0);
    ys.push_back(0);
    zs.push_back(0);
    max_x = std::max(0, max_x);
    max_y = std::max(0, max_y);
    max_z = std::max(0, max_z);
    min_x = std::min(0, min_x);
    min_y = std::min(0, min_y);
    min_z = std::min(0, min_z);
    int ox = -min_x, oy = -min_y, oz = -min_z;
    int dist = 1;
    while ((dist < (max_x-min_x)) or (dist < (max_y-min_y)) or (dist < (max_z-min_z))) {
        dist *= 2;
    }
    int span = 1;
    while (span < (int)all_nanobots.size()){
        span *= 2;
    }
    int forced_check = 1;
    std::unordered_map<int, std::pair<int, int>> tried;
    int best_val = -1, best_count = -1;
    while (true){
        if (tried.find(forced_check) == tried.end()){
            tried[forced_check] = find(all_nanobots, xs, ys, zs, dist, forced_check, ox, oy, oz);
        }
        auto result = tried[forced_check];
        if (result.first == -1){
            if (span > 1) span /=2;
            forced_check = std::max(1, forced_check-span);
        }
        else{
            if (best_count == -1 or result.second > best_count){
                best_val = result.first;
                best_count = result.second;
            }
            if (span == 1){
                break;
            }
            forced_check += span;
        }
    }
    std::cout << "Best location value " << best_val << std::endl;
}
