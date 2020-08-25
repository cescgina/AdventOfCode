#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <unistd.h>

int main(int argc, char** argv){
    std::string file_name, hex_digits = "0123456789abcdef";
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    int size = 256;
    input_file.open(file_name);
    for (std::string line; std::getline(input_file, line);){
        std::cout << line << "\t";
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
            std::cout << hex_digits[upper];
            std::cout << hex_digits[val & 15];
        }
        std::cout << std::endl;
    }
}
