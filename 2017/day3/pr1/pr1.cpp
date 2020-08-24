#include <cmath>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>

int find_spiral_dist(int number){
    if (number == 1){
        return 0;
    }
    int val = 0, i = 1;
    while (val < number){
        val = std::pow((2*i+1), 2);
        if (val > number){
            break;
        }
        i++;
    }
    i--;
    val = std::pow((2*i+1), 2);
    int n = 2*(i+1);
    if (val == number) return n;
    int diff = (number-val) % (n);
    return n-std::min(diff, n-diff);
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    int num = 0;
    input_file.open(file_name);
    std::string line; 
    std::getline(input_file, line);
    num = std::stoi(line);
    input_file.close();
    std::cout << find_spiral_dist(num) << std::endl;
}
