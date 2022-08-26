#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <unistd.h>
#include <boost/algorithm/string.hpp>

int main(int argc, char** argv){
    std::string file_name, line;
    std::vector<int> fishes;
    std::vector<std::string> nums;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file(file_name);
    std::getline(input_file, line);
    boost::split(nums,line,boost::is_any_of(","));
    std::transform(nums.begin(), nums.end(), std::back_inserter(fishes),
                   [](const std::string& s) {return std::stoi(s);});
    const unsigned int days = 34;

    for (size_t i=0; i<days; i++){
        size_t existing_fishes = fishes.size();
        for (size_t j=0; j<existing_fishes; j++){
            fishes[j]--;
            if (fishes[j] < 0){
                fishes.push_back(8);
                fishes[j] = 6;
            }
        }
        existing_fishes = fishes.size();
        std::cout << "Day " << i+1 << " " << existing_fishes << " fishes ";
        for (size_t j=0; j<existing_fishes; j++){
            std::cout << fishes[j] << ",";
        }
        std::cout << std::endl;
    }
}
