#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

typedef std::unordered_map<char, int> counts;
typedef std::vector<std::string> to_parse;

counts count_repeats(const std::string& word){
    counts total_counts;
    for (char c:word){
        total_counts[c]++;
    }
    return total_counts;
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    int correct = 0;
    std::ifstream input_file(file_name);
    for (std::string line; std::getline(input_file, line);){
        to_parse entry;
        boost::split(entry, line, boost::is_any_of(" -:"), boost::token_compress_on);
        int min = std::stoi(entry[0]);
        int max = std::stoi(entry[1]);
        char letter = entry[2][0];
        counts repeats = count_repeats(entry[3]);
        correct += (repeats[letter] >= min and repeats[letter] <= max);
    }
    input_file.close();
    std::cout << "There are " << correct << " correct passwords "<< std::endl;
}
