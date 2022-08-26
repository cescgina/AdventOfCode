#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <unistd.h>
#include <boost/algorithm/string.hpp>

typedef std::unordered_map<unsigned int, long> store;
long fishes_spawned(int days, store& memory) {
    if (memory.find(days) != memory.end()){
        return memory[days];
    }
    else if (days < 6) return 1;
    int days_start = days;
    long total_fishes = 0;
    while (days >= 0){
        if (days > 8){
            total_fishes += fishes_spawned(days-9, memory);
        }
        total_fishes++;
        days -= 7;
    }
    memory[days_start] = total_fishes;
    return total_fishes;

}

long count_fishes(std::vector<long>& fishes, int days) {
    size_t existing_fishes = fishes.size();
    long total_fishes = existing_fishes;
    store all_fishes;
    for (size_t k=0; k<existing_fishes; k++){
        long remaining_days = days-fishes[k]-1;
        long new_fishes =fishes_spawned(remaining_days, all_fishes);
        total_fishes += new_fishes;
        // std::cout << "fish " << k << " " << new_fishes << std::endl;
    }
    return total_fishes;
}

int main(int argc, char** argv){
    std::string file_name, line;
    std::vector<long> fishes;
    std::vector<std::string> nums;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file(file_name);
    std::getline(input_file, line);
    boost::split(nums,line,boost::is_any_of(","));
    std::transform(nums.begin(), nums.end(), std::back_inserter(fishes),
                   [](const std::string& s) {return std::stol(s);});
    const int days = 80;

    std::cout << count_fishes(fishes, 10) << std::endl;
    std::cout << count_fishes(fishes, 18) << std::endl;
    std::cout << count_fishes(fishes, 80) << std::endl;
    std::cout << count_fishes(fishes, 256) << std::endl;
}
