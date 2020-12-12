#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

typedef std::vector<int> digits;

bool matches_condition(int x){
    digits number;
    for (const auto digit : std::to_string(x)) {
        number.push_back(digit - '0');
    }
    bool is_sorted = std::is_sorted(number.begin(), number.end());
    bool repeat = false;
    for (size_t j=0; j < number.size()-1; j++){
        if (number[j] == number[j+1]) repeat = true;
    }
    return is_sorted and repeat;
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::string line;
    std::getline(input_file, line);
    input_file.close();
    auto pos = line.find("-");
    int start = std::stoi(line.substr(0, pos));
    int end = std::stoi(line.substr(pos+1));
    int num = 0;
    for (int i = start; i <= end; i++){
        if (matches_condition(i)) num++;
    }
    std::cout << num << " number match the condition" << std::endl;
}
