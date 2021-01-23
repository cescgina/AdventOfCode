#include <map>
#include <set>
#include <queue>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

typedef std::vector<long long> numbers;
bool has_sum(const numbers & cypher, size_t i, int preamble){
    for (size_t j = i-preamble; j < i; j++){
        if (std::find(cypher.begin()+j+1, cypher.begin()+i, cypher[i]-cypher[j]) != cypher.begin()+i){
            return true;
        }
    }
    return false;
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file(file_name);
    numbers cypher;
    for (std::string line; std::getline(input_file, line);){
        cypher.push_back(std::stoll(line));
    }
    input_file.close();
    int preamble = 25;
    if (file_name == "data/test.txt") preamble = 5;
    for (size_t i = preamble+1; i < cypher.size(); i++){
        if (not has_sum(cypher, i, preamble)){
            std::cout << cypher[i] << std::endl;
        }
    }
}
