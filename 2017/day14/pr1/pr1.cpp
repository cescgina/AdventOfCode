#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <unistd.h>

static const std::string hex_digits = "0123456789abcdef";

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
    for (int j=0; j<16; j++){
        bits[hex_digits[j]] = __builtin_popcount(j);
    }
    unsigned int set_bits = 0;
    for (int i=0; i<128; i++){
        std::string hash_row = knot_hash(key+"-"+std::to_string(i));
        for (auto hex_char: hash_row){
            set_bits += bits[hex_char];
        }
    }
    std::cout << set_bits << std::endl;
}
