#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <unistd.h>
#include <boost/algorithm/string.hpp>

int calculate_cost(int min, const std::vector<int>& values){
    int cost = 0;
    int steps = 0;
    for (auto x: values){
        steps = std::abs(x-min);
        cost += steps*(steps+1)/2;
    }
    return cost;
}

int main(int argc, char** argv){
    std::string file_name, line;
    std::vector<int> numbers;
    std::vector<std::string> nums;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file(file_name);
    std::getline(input_file, line);
    boost::split(nums,line,boost::is_any_of(","));
    std::transform(nums.begin(), nums.end(), std::back_inserter(numbers),
                   [](const std::string& s) {return std::stoi(s);});
    std::sort(numbers.begin(), numbers.end());
    int size = numbers.size();
    int sum = std::accumulate(numbers.begin(), numbers.end(), 0);
    int min_pos = sum/size;
    std::cout << min_pos << ", " << calculate_cost(min_pos, numbers) << std::endl;
    std::cout << min_pos+1 << ", " << calculate_cost(min_pos+1, numbers) << std::endl;
    std::cout << min_pos+2 << ", " << calculate_cost(min_pos+2, numbers) << std::endl;
    std::cout << min_pos-1 << ", " << calculate_cost(min_pos-1, numbers) << std::endl;
    std::cout << min_pos-2 << ", " << calculate_cost(min_pos-2, numbers) << std::endl;
}
