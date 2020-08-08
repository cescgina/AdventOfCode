#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>

typedef std::vector<std::map<char, int>> freqs;

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    freqs sorted_chars;
    std::getline(input_file, line);
    size_t n = line.size();
    for (size_t i=0; i<n; i++){
        std::map<char, int> col;
        col[line[i]] = 1;
        sorted_chars.push_back(col);
    }
    for (std::string line; std::getline(input_file, line);){
        for (size_t i=0; i<n; i++){
            if (sorted_chars[i].find(line[i]) != sorted_chars[i].end()){
                sorted_chars[i][line[i]] += 1;
            }
            else{
                sorted_chars[i][line[i]] = 1;
            }
        }
    }
    for (size_t i=0; i<n; i++){
        int max = 0;
        char correct = '-';
        for (auto it=sorted_chars[i].begin(); it != sorted_chars[i].end(); it++){
            if (it->second > max){
                max = it->second;
                correct = it->first;
            }
        }
        std::cout << correct;
    }
    std::cout << std::endl;
}
