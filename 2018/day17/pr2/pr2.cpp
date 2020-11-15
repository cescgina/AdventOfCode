#include <vector>
#include <string>
#include <utility>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <unistd.h>
#include <boost/functional/hash.hpp>

struct Ray{
    int x, y, id, x_left, x_right;
};

typedef std::unordered_map<std::pair<int, int>, int, boost::hash<std::pair<int, int>>> map_clay;
typedef std::list<Ray> rays_set;

bool is_clay(int col, int row, map_clay& current_state){
    // a value of 1 means clay, 2 stagnant water and 3 flowing water
    auto coords = std::make_pair(row, col);
    if (current_state.find(coords) != current_state.end()){
        return current_state[coords] < 3; 
    }
    return false;
}

int find_bottom_pit(Ray& ray, map_clay& current_state, const int& y_max){
    while ((not is_clay(ray.x, ray.y+1, current_state)) and ray.y < y_max){
        ray.y++;
        current_state[std::make_pair(ray.y, ray.x)] = 3;
    }
    int status = 0;
    if (is_clay(ray.x-1, ray.y+1, current_state)){
        status = 1;
    }
    if (is_clay(ray.x+1, ray.y+1, current_state)){
        status += 2;
    }
    return status;
}

void find_walls(Ray& ray, map_clay& current_state){
    ray.x_left = ray.x-1;
    ray.x_right = ray.x+1;
    while (is_clay(ray.x_left-1, ray.y+1, current_state)){
        ray.x_left--;
    }
    while (is_clay(ray.x_right+1, ray.y+1, current_state)){
        ray.x_right++;
    }
}

bool is_walled(Ray& ray, map_clay& current_state){
    bool left_wall = false; 
    bool right_wall = false;
    ray.x_left = ray.x-1; 
    while (is_clay(ray.x_left, ray.y+1, current_state)){
        if (is_clay(ray.x_left, ray.y, current_state)){
            left_wall = true;
            break;
        }
        ray.x_left--;
    }
    ray.x_right = ray.x+1;
    while (is_clay(ray.x_right, ray.y+1, current_state)){
        if (is_clay(ray.x_right, ray.y, current_state)){
            right_wall = true;
            break;
        }
        ray.x_right++;
    }
    return (left_wall and right_wall);
}

void merge_rays(rays_set& rays_found){
    for (auto it = rays_found.begin(); it != rays_found.end(); it++){
        for (auto it2 = it; it2 != rays_found.end(); it2++){
            if (it->id == it2->id) continue;
            if ((it->y == it2->y) and (it->x_right == it2->x_right) and (it->x_left == it2->x_left) and (it->id != it2->id)){
                it2 = rays_found.erase(it2);
            } 
        }
    }
}

void determine_overflow_coords(Ray& ray, map_clay& current_state, rays_set& to_add, int& id){
    if (is_clay(ray.x_left, ray.y, current_state)){
        for(int l=ray.x_left+1; l<=ray.x_right; l++){
            // add filled spaces to the state
            current_state[std::make_pair(ray.y, l)] = 3;
        }
        ray.x = ray.x_right;
    }
    else if (is_clay(ray.x_right, ray.y, current_state)){
        ray.x = ray.x_left;
        for(int l=ray.x_right-1; l>=ray.x_left; l--){
            // add filled spaces to the state
            current_state[std::make_pair(ray.y, l)] = 3;
        }
    }
    else{
        //overflow both ends
        for(int l=ray.x_right; l>=ray.x_left; l--){
            // add filled spaces to the state
            current_state[std::make_pair(ray.y, l)] = 3;
        }
        ray.x = ray.x_right;
        id++;
        to_add.push_back(Ray {ray.x_left, ray.y, id, 0, 0});
    }
}

void print_state(int x, int x1, int y, int y1, map_clay& current_state){
    for (int i=y; i<=y1; i++){
        for (int j=x; j<=x1; j++){
            auto coords = std::make_pair(i, j);
            if (current_state.find(coords) != current_state.end()){
                if (current_state[coords] == 1){
                    std::cout << "#";
                }
                else if (current_state[coords] == 2){
                    std::cout << "~";
                }
                else{
                    std::cout << "|";
                }
            }
            else{
                std::cout << ".";
            }
        }
        std::cout << std::endl;
    }
}

int count_tiles(map_clay& current_state, int& y_min, int& y_max){
    int count = 0;
    for (auto it=current_state.begin(); it!= current_state.end(); it++) {
        if (it->first.first < y_min or it->first.first > y_max){
            continue;
        }
        count += (it->second == 2);
    }
    return count;
}
    
int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    map_clay state;
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    int y_min = 10000, y_max = 0;
    int x_min = 10000, x_max = 0;
    rays_set rays;
    for(std::string line;std::getline(input_file, line);){
        char dim1 = line[0];
        auto pos =line.find(", ");
        int coord1 = std::stoi(line.substr(2, pos-2));
        std::string range = line.substr(pos+4);
        pos = range.find("..");
        int init = std::stoi(range.substr(0, pos));
        int end = std::stoi(range.substr(pos+2));
        for (int i = init; i <= end; i++){
            if (dim1 == 'x'){
                state[std::make_pair(i, coord1)] = 1;
                y_min = std::min(i, y_min);
                y_max = std::max(i, y_max);
                x_min = std::min(coord1, x_min);
                x_max = std::max(coord1, x_max);
            }
            else{
                state[std::make_pair(coord1, i)] = 1;
                y_min = std::min(coord1, y_min);
                y_max = std::max(coord1, y_max);
                x_min = std::min(i, x_min);
                x_max = std::max(i, x_max);
            }
        }
    }
    input_file.close();
    int ray_num = 0;
    rays.push_back(Ray {500, 0, ray_num, 0, 0});
    while (not rays.empty()){
        rays_set new_rays;
        for (auto it = rays.begin(); it != rays.end(); ){
            int kind_wall = find_bottom_pit(*it, state, y_max);
            if (it->y >= y_max) {
                it = rays.erase(it);
            }
            else{
                if (kind_wall == 0){
                    // double overflow
                    ray_num++;
                    new_rays.push_back(Ray {it->x-1, it->y, ray_num, 0, 0});
                    ray_num++;
                    new_rays.push_back(Ray {it->x+1, it->y, ray_num, 0, 0});
                    it = rays.erase(it);
                }
                else{
                    find_walls(*it, state);
                    it++;
                }
            }
        }
        merge_rays(rays);
        for (auto it = rays.begin(); it != rays.end(); it++){
            while (is_walled(*it, state) and it->y > y_min){
                // substract one to get the number of emtpy spaces and one to avoid
                // overcounting the spaces from when the water is falling down
                for(int l=it->x_left+1; l<it->x_right; l++){
                    // add filled spaces to the state
                    state[std::make_pair(it->y, l)] = 2;
                }
                it->y--;
            }
            determine_overflow_coords(*it, state, new_rays, ray_num);
        }
        for (auto it_new = new_rays.begin(); it_new != new_rays.end(); it_new++){
            rays.push_back(*it_new);
        }
    }
    print_state(x_min-1, x_max+1, y_min-1, y_max+1, state);
    std::cerr << count_tiles(state, y_min, y_max) << " wet tiles" << std::endl;
}
