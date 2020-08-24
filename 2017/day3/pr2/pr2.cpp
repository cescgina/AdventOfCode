#include <map>
#include <cmath>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>

int find_spiral_dist(int number, std::map<int, int>& values){
    if (number < 1){
        return 0;
    }
    if (values.find(number) != values.end()) {
        return values[number];
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
    int n = 2*(i+1), prev_val = std::pow(2*(i-1)+1, 2);
    if (i==1) prev_val = 1; 
    int diff = (number-val);
    int offset = diff % n;
    int side = (diff / n)+1;
    int new_val = values[number-1];
    if (diff == 0){
        new_val += values[prev_val]+values[prev_val+1];
    }
    else if (diff == 1 or offset == 0){
        int new_pos = ((side-1)*(n-2))+prev_val+offset;
        new_val += values[new_pos];
    }
    else if (diff == 2){
        int new_pos = prev_val+offset*side;
        new_val += values[new_pos]+values[new_pos-1]+values[number-2];
    }
    else if (offset == 1){
        int new_pos = prev_val+(diff-(2*(side-1)));
        new_val += values[new_pos]+values[new_pos-1]+values[number-2];
    }
    else if (offset == (n-1)){
        int new_pos = prev_val+(offset-1)*side;
        new_val += values[new_pos]+values[new_pos-1];
        if (side == 4) new_val += values[new_pos+1];
    }
    else{
        int new_pos = prev_val+offset+(n-2)*(side-1);
        new_val += values[new_pos]+values[new_pos-1]+values[new_pos-2];

    }
    values[number] = new_val;
    return new_val;
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    int num = 0, index = 10;
    input_file.open(file_name);
    std::string line; 
    std::getline(input_file, line);
    num = std::stoi(line);
    input_file.close();
    std::map<int, int> values;
    values[1] = 1;
    values[2] = 1;
    values[3] = 2;
    values[4] = 4;
    values[5] = 5;
    values[6] = 10;
    values[7] = 11;
    values[8] = 23;
    values[9] = 25;
    values[10] = 26;
    while (values[index] <= num){
        index++;
        int c = find_spiral_dist(index, values);
        values[index] = c;
    }
    std::cout << values[index] << std::endl;
}
