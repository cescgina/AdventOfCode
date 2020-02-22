#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
#include <unistd.h>


int sum_factors(int number){
    int sum = 0, d = 0;
    for (int j=1; j<std::sqrt(number); j++){
        if ((number % j) == 0){
            d = number/j;
            if (j*50 > number) sum+= j;
            if (d*50 > number) sum+= d;
        }
    }
    return sum;
}

int main(int argc, char** argv){
    std::string file_name, command, molecule;
    getopt(argc, argv, "");
    int presents = 0, house = 0;
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    input_file >> presents;
    input_file.close();
    for (int i=1; i<presents; i++){
        if (11*sum_factors(i) >= presents) {
            house = i;
            break;
        }
    }
    std::cout << "House is " << house << std::endl;
}
