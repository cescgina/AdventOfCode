#include <set>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::set<char> letters;
    int total_ans = 0;
    std::ifstream input_file(file_name);
    for (std::string line; std::getline(input_file, line);){
        if (line.empty()){
            total_ans += letters.size();
            letters.clear();
            continue;
        }
        for (char c: line){
            letters.insert(c);
        }
    }
    input_file.close();
    total_ans += letters.size();
    std::cout << "Total answers " << total_ans << std::endl;
}
