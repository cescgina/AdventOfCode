#include <map>
#include <set>
#include <queue>
#include <bitset>
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <unistd.h>

static const std::string hex_digits = "0123456789abcdef";
static const std::vector<std::pair<int, int>> moves = {std::make_pair(-1, 0), std::make_pair(1, 0), std::make_pair(0, -1), std::make_pair(0, 1)};

void print_grid(const std::vector<std::vector<bool>>& grid){
    for (auto x: grid){
        for (auto y: x){
            if (y){
                std::cout << "#";
            }
            else{
                std::cout << ".";
            }
        }
        std::cout << std::endl;
    }
}

void explore_region(const std::vector<std::vector<bool>>& grid, const int& x, const int& y, std::set<std::pair<int, int>>& history){
    std::queue<std::pair<int, int>> to_explore;
    to_explore.push(std::make_pair(x, y));
    int ii, jj, dx, dy;
    while (!to_explore.empty()){
        auto point = to_explore.front();
        to_explore.pop();
        if (history.find(point) != history.end()) continue;
        history.insert(point);
        ii = point.first;
        jj = point.second;
        for (auto move: moves){
            dx = move.first;
            dy = move.second;
            if (grid[ii+dx][jj+dy]){
                to_explore.push(std::make_pair(ii+dx, jj+dy));
            }
        }
    }
}

std::string knot_hash(const std::string& line){
    int size = 256;
    std::string hash;
    std::vector<int> lengths;
    for (const auto& c: line){
        lengths.push_back(c);
    }
    lengths.push_back(17);
    lengths.push_back(31);
    lengths.push_back(73);
    lengths.push_back(47);
    lengths.push_back(23);
    std::vector<int> numbers = std::vector<int>(size, 0);
    std::iota(numbers.begin(), numbers.end(), 0);
    int current = 0, skip = 0;
    for (int i=0; i<64; i++){
        for (auto length: lengths){
            if (length > 1){
                std::rotate(numbers.begin(), numbers.begin()+current, numbers.end());
                std::reverse(numbers.begin(), numbers.begin()+length);
                std::rotate(numbers.rbegin(), numbers.rbegin()+current, numbers.rend());
            }
            current += length+skip;
            current %= size;
            skip++;
        }
    }
    for (int i=0; i<16; i++){
        int pos = 16*i;
        int val = numbers[pos];
        for (int j=1; j<16; j++){
            val ^= numbers[pos+j];
        }
        // turn val into hex
        int upper = (val & 240) >> 4;
        hash += hex_digits[upper];
        hash += hex_digits[val & 15];
    }
    return hash;
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::string key; 
    std::getline(input_file, key);
    input_file.close();
    std::map<char, int> bits;
    for (unsigned int j=0; j<16; j++){
        bits[hex_digits[j]] = j;
    }
    std::vector<std::vector<bool>> bits_grid(130, std::vector<bool>(130, false));
    unsigned int regions = 0;
    for (int i=1; i<129; i++){
        std::string hash_row = knot_hash(key+"-"+std::to_string(i-1));
        size_t k = 1;
        for (auto hex_char: hash_row){
            std::bitset<4> hex_bits(bits[hex_char]);
            for (int l=3;l>=0;l--){
                bits_grid[i][k] = hex_bits[l];
                k++;
            }
        }
    }
    std::set<std::pair<int, int>> visited;
    for (int i=1; i<129; i++){
        for (int j=1; j<129; j++){
            if (visited.find(std::make_pair(i, j)) != visited.end()) continue;
            if (bits_grid[i][j]){
                regions++;
                explore_region(bits_grid, i, j, visited);
            }
        }
    }
    std::cout << regions << std::endl;
}
