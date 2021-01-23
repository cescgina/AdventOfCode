#include <set>
#include <queue>
#include <vector>
#include <string>
#include <fstream>
#include <numeric>
#include <algorithm>
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

long long get_sum_continguous(const numbers& cumsum, size_t i, long long number, const numbers& cypher){

    for (size_t j=i; j<cumsum.size(); j++){
        long long diff = cumsum[j]-cumsum[i-1];
        if ( diff > number) return -1;
        else if (diff == number){
            auto interval = std::minmax_element(cypher.begin()+i-1, cypher.begin()+j-1);
            return *interval.first+*interval.second;
        }
    }
    return -1;
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
    long long invalid = 0;
    if (file_name == "data/test.txt") preamble = 5;
    for (size_t i = preamble+1; i < cypher.size(); i++){
        if (not has_sum(cypher, i, preamble)){
            invalid = cypher[i];
        }
    }
    numbers cumulative(cypher.size(), 0);
    std::partial_sum(cypher.begin(), cypher.end(), cumulative.begin()+1);
    for (size_t i=0; i<cypher.size(); i++){
        long long contigous = get_sum_continguous(cumulative, i+1, invalid, cypher);
        if (contigous != -1){
            std::cout << contigous << std::endl;
            return 0;
        }
    }
}
