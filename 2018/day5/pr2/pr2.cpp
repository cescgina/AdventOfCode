#include <set>
#include <cctype>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>

int collapse_polymer(std::string& pol){
    auto it = pol.begin();
    while (it != pol.end()){
        if (std::abs(*it-*(it+1)) == 32){
            it = pol.erase(it, it+2)-1;
        }
        else{
            it++;
        }
    }
    return pol.size();
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::string polymer; 
    std::getline(input_file, polymer);
    std::set<char> explored;
    int min_pol = 100000000;
    for (char c: polymer){
        char upper = std::toupper(c);
        if (explored.find(upper) == explored.end()){
            explored.insert(upper);
            char lower = std::tolower(c);
            std::string trim_polymer(polymer);
            trim_polymer.erase(std::remove(trim_polymer.begin(), trim_polymer.end(), upper), trim_polymer.end());
            trim_polymer.erase(std::remove(trim_polymer.begin(), trim_polymer.end(), lower), trim_polymer.end());
            min_pol = std::min(min_pol, collapse_polymer(trim_polymer));
        }
    }
    std::cout << min_pol << std::endl;
}
