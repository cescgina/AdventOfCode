#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

bool DEBUG = false;
typedef std::vector<int> row;
typedef std::vector<row> grid;
const std::vector<char> chars = {'.', '#'};
typedef std::map<int, grid> levels;

std::ostream& operator<<(std::ostream& out, grid& state){
    for (size_t j = 1; j < 6; j++){
        for (size_t i = 1; i < 6; i++){
           out << chars[state[j][i]]; 
        }
        out << std::endl;
    }
    return out;
}

int sum_bugs(grid& state){
    int sum = 0;
    for (size_t j = 1; j < 6; j++){
        for (size_t i = 1; i < 6; i++){
           sum += state[j][i]; 
        }
    }
    return sum;
}

std::string hash(grid & state){
    std::string hash;
    for (size_t j = 1; j < 6; j++){
        for (size_t i = 1; i < 6; i++){
           hash.push_back(char(48+state[j][i])); 
        }
    }
    return hash;
}

int count(grid& state, size_t j, size_t i){
    return state[j-1][i]+state[j+1][i]+state[j][i+1]+state[j][i-1];
}

grid get_empty_grid(){
    return grid(7, row(7, 0));
}

bool is_recursive_region_up(size_t j, size_t i){
    if ((j == 2 or j == 4) and i == 3) return true;
    if ((i == 2 or i == 4) and j == 3) return true;
    return false;
}

bool is_recursive_region_down(size_t j, size_t i){
    return (j == 1 or j == 5 or i == 1 or i == 5);
}

int count_recursive_neighbors_down(size_t j, size_t i, levels& all_levels, int level){
    int count = 0;
    if (all_levels.find(level-1) == all_levels.end()) all_levels[level-1] = get_empty_grid();
    grid recursive_state = all_levels[level-1];
    grid current = all_levels[level];
    if (i == 1) {
        count += recursive_state[3][2];
        if (j == 1){
            count += recursive_state[2][3]+current[1][2]+current[2][1];
        }
        else if (j == 5){
            count += recursive_state[4][3]+current[4][1]+current[5][2];
        }
        else{
            count += current[j-1][i]+current[j+1][i]+current[j][i+1];
        }
    }
    else if (i == 5){
        count += recursive_state[3][4];
        if (j == 1){
            count += recursive_state[2][3]+current[1][4]+current[2][5];
        }
        else if (j == 5){
            count += recursive_state[4][3]+current[5][4]+current[4][5];
        }
        else{
            count += current[j-1][i]+current[j+1][i]+current[j][i-1];
        }
    }
    else if (j == 1){
        count += recursive_state[2][3]+current[j+1][i]+current[j][i+1]+current[j][i-1];
    }
    else if (j==5){
        count += recursive_state[4][3]+current[j-1][i]+current[j][i+1]+current[j][i-1];
    }
    else{
        std::cout << "something went wrong at "<< j << " " << i << std::endl;
    }
    return count;
}

int count_recursive_neighbors_up(size_t j, size_t i, levels& all_levels, int level){
    int count = 0;
    if (all_levels.find(level+1) == all_levels.end()) all_levels[level+1] = get_empty_grid();
    grid recursive_state = all_levels[level+1];
    grid current = all_levels[level];
    if (i == 3) {
        if (j == 2){
            for (size_t ii = 1; ii < 6; ii++){
                count += recursive_state[1][ii];
            } 
            count += current[j-1][i]+current[j][i-1]+current[j][i+1];
        }
        else if (j == 4){
            for (size_t ii = 1; ii < 6; ii++){
                count += recursive_state[5][ii];
            } 
            count += current[j+1][i]+current[j][i-1]+current[j][i+1];
        }
        else{
            std::cout << "something went wrong at "<< j << " " << i << std::endl;
        }
    }
    else{
        if (i == 2){
            for (size_t ii = 1; ii < 6; ii++){
                count += recursive_state[ii][1];
            } 
            count += current[j+1][i]+current[j][i-1]+current[j-1][i];
        }
        else if (i == 4){
            for (size_t ii = 1; ii < 6; ii++){
                count += recursive_state[ii][5];
            } 
            count += current[j+1][i]+current[j][i+1]+current[j-1][i];
        }
        else{
            std::cout << "something went wrong at "<< j << " " << i << std::endl;
        }
    }
    return count;
}

void update_state(int level, levels& all_levels, levels& cp_levels){
    for (size_t j = 1; j < 6; j++){
        for (size_t i = 1; i < 6; i++){
            int neigh = 0;
            if (j == 3 and i == 3) continue;
            else if (is_recursive_region_up(j, i)){
                neigh = count_recursive_neighbors_up(j, i, all_levels, level);
            }
            else if (is_recursive_region_down(j, i)){
                neigh = count_recursive_neighbors_down(j, i, all_levels, level);
            }
            else{
                neigh = count(all_levels[level], j, i);
            }
            if (all_levels[level][j][i]){
                cp_levels[level][j][i] = (neigh == 1); 
            }
            else{
                cp_levels[level][j][i] = (neigh > 0 and neigh < 3); 
            }
        }
    }
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    grid state;
    std::ifstream input_file;
    input_file.open(file_name);
    row halo(7,0);
    state.push_back(halo);
    for (std::string line; std::getline(input_file, line);){
        row current_row{0};
        for (size_t j = 0; j < line.size(); j++){
            current_row.push_back(line[j] == '#');
        }
        current_row.push_back(0);
        state.push_back(current_row);
    }
    state.push_back(halo);
    grid new_state(state);
    int n_minutes = 200;
    if (file_name == "data/test.txt") n_minutes = 10;
    std::string hash_state = hash(state);
    levels all_levels;
    all_levels[0] = state;
    int n_levels = n_minutes/2;
    for (int i=1; i<=n_levels; i++){
        all_levels[i] = get_empty_grid();
        all_levels[-i] = get_empty_grid();
    }

    for (int time = 0; time < n_minutes; time++){
        levels cp_levels(all_levels);
        update_state(0, all_levels, cp_levels);
        for (int level = 1; level <= n_levels; level++){
            update_state(level, all_levels, cp_levels);
            update_state(-level, all_levels, cp_levels);
        }
        std::swap(all_levels, cp_levels);
        if (DEBUG){
            std::cout << "Round : " << time+1<<std::endl;
            std::cout << std::endl;
            for (int level = -n_levels; level <= n_levels; level++){
                std::cout << "Depth " << level << ":" << std::endl;
                std::cout << all_levels[level] << std::endl;
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
    }
    if (DEBUG){
        for (int level = -n_levels; level <= n_levels; level++){
            std::cout << "Depth " << level << ":" << std::endl;
            std::cout << all_levels[level] << std::endl;
            std::cout << std::endl;
        }
    }
    int total_bugs = 0;
    for (int level = -n_levels; level <= n_levels; level++){
        total_bugs += sum_bugs(all_levels[level]);
    }
    std::cout << "Total bugs is " << total_bugs << std::endl;
}
