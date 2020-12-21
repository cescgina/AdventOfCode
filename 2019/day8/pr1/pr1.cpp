#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::string line;
    std::getline(input_file, line);
    int rows = 6, cols = 25;
    int pixels = rows*cols;
    int zeros = 0, min_zeros = 1000, ones = 0, twos = 0;
    int product = 0;
    for (size_t i = 0; i < line.size(); i++){
        if (line[i] == '0'){
            zeros++;
        }
        else if (line[i] == '1'){
            ones++;
        }
        else if (line[i] == '2'){
            twos++;
        }
        if ((i % pixels) == 0 and (i > 0)){
            if (zeros < min_zeros){
                min_zeros = zeros;
                product = ones*twos;
            }
            zeros = 0;
            ones = 0;
            twos = 0;
        }
    }
    std::cout << "Number is " << product << std::endl;
}
