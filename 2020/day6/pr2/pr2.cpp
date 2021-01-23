#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

int count_answers(std::map<char, int>& letters, int group_size){
    return std::count_if(letters.begin(), letters.end(), [group_size](std::pair<char, int> x){return x.second == group_size;});
}
int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::map<char, int> letters;
    int total_ans = 0, group_size = 0;
    std::ifstream input_file(file_name);
    for (std::string line; std::getline(input_file, line);){
        if (line.empty()){
            total_ans += count_answers(letters, group_size);
            letters.clear();
            group_size = 0;
            continue;
        }
        for (char c: line){
            letters[c]++;
        }
        group_size++;
    }
    input_file.close();
    total_ans += count_answers(letters, group_size);
    std::cout << "Total answers " << total_ans << std::endl;
}
