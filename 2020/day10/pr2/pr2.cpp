#include <map>
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
typedef std::map<int, long long> cache_counts; 

bool contains(cache_counts& haystack, int needle){
    return haystack.find(needle) != haystack.end();
}

long long count_combinations(size_t index, numbers& voltages, cache_counts& cache){
    if (contains(cache, index)) return cache[index]; 
    if (index == voltages.size()-1) return 1;
    long long counts = 0;
    for (size_t i=index+1; i < voltages.size() and ((voltages[i] - voltages[index]) < 4); i++){
        counts += count_combinations(i, voltages, cache); 
    }
    cache[index] = counts;
    return counts;
}
int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file(file_name);
    numbers voltages = {0};
    for (std::string line; std::getline(input_file, line);){
        voltages.push_back(std::stoll(line));
    }
    input_file.close();
    cache_counts cache;
    std::sort(voltages.begin(), voltages.end());
    voltages.push_back(voltages[voltages.size()-1]+3);
    std::cout << count_combinations(0, voltages, cache) << std::endl;
}
