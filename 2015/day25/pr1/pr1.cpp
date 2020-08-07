#include <vector>
#include <string>
#include <numeric>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>

long long moduloMultiplication(long long a, long long b, long long mod) {
    long long res = 0; // Initialize result

    // Update a if it is more than
    // or equal to mod
    a %= mod;

    while (b)
    {
        // If b is odd, add a with result
        if (b & 1)
            res = (res + a) % mod;

        // Here we assume that doing 2*a
        // doesn't cause overflow
        a = (2 * a) % mod;

        b >>= 1; // b = b / 2
    }
    return res;
}

int main(int argc, char** argv){
    std::string file_name, instruction, reg;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    long long value = 31916031, multiplier = 252533, modulo = 33554393;
    int row = 2, col = 1, frow = 0, fcol = 0;
    for (std::string line; std::getline(input_file, line);){
        auto pos = line.find("row");
        auto pos_sep = line.find(",", pos);
        frow = std::stoi(line.substr(pos+4, (pos_sep-pos)));
        pos = pos_sep+9;
        pos_sep = line.find(".", pos);
        fcol = std::stoi(line.substr(pos, (pos_sep-pos)));
    }
    while (col != fcol or row != frow){
        value = moduloMultiplication(value, multiplier, modulo);
        if (row == 1){
            std::swap(row, col);
            row++;
        }
        else {
            row--;
            col++;
        }
    }
    std::cout << "Value at position " << frow << "x" << fcol << " " << value << std::endl;
}
