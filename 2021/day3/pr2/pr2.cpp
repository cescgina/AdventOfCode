#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <unistd.h>

char find_most_common(std::vector<std::string>& nums, size_t index){
    size_t size = nums.size();
    size_t ones = 0;
    for (auto&& num: nums){
        ones += (num[index] == '1');
    }
    if (ones >= (size-ones)){
        return '1';
    }
    else return '0';
}

char find_least_common(std::vector<std::string>& nums, size_t index){
    size_t size = nums.size();
    size_t ones = 0;
    for (auto&& num: nums){
        ones += (num[index] == '1');
    }
    if (ones >= (size-ones)){
        return '0';
    }
    else return '1';
}
std::string find_values(std::vector<std::string> nums, bool reverse=false){
    size_t index = 0;
    while (nums.size() > 1){
        char ref;
        if (reverse){
            ref = find_most_common(nums, index);
        }
        else {
            ref = find_least_common(nums, index);
        }
        std::vector<std::string> valid_nums;
        for (auto num: nums){
            if (num[index] == ref){
                valid_nums.push_back(num);
            }
        }
        index++;
        std::swap(valid_nums, nums);
    }
    return nums[0];
}

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file(file_name);
    std::vector<std::string> numbers_list;
    for (std::string line; std::getline(input_file, line);){
        numbers_list.push_back(line);
    }
    input_file.close();
    std::string co2_bin, oxygen_bin;
    oxygen_bin = find_values(numbers_list);
    co2_bin = find_values(numbers_list, true);
    int co2 = std::stoi(co2_bin, 0, 2);
    int oxygen = std::stoi(oxygen_bin, 0, 2);
    std::cout << co2 * oxygen << std::endl;
}
