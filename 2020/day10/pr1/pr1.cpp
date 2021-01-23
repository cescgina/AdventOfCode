#include <map>
#include <set>
#include <queue>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

typedef std::vector<long long> numbers;

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file(file_name);
    numbers voltages;
    for (std::string line; std::getline(input_file, line);){
        voltages.push_back(std::stoll(line));
    }
    input_file.close();
    std::sort(voltages.begin(), voltages.end());
    voltages.push_back(voltages[voltages.size()-1]+3);
    std::adjacent_difference(voltages.begin(), voltages.end(), voltages.begin());
    int ones = 0, threes = 0;
    for (size_t i = 0; i< voltages.size(); i++){
        ones += (voltages[i] == 1);
        threes += (voltages[i] == 3);
    }
    std::cout << ones*threes << std::endl;
}
