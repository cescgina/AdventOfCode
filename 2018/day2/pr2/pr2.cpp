#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <unistd.h>

bool check_differences(const std::string& s, const std::string& s2){
    int dist = 0;
    size_t index = 0;
    for (size_t i=0; i < s.size(); i++){
        if (s[i] != s2[i]){
            index = i;
            dist++;
        }
        if (dist > 1) return false;
    }
    if (dist == 1){
        for (size_t i=0; i < s.size(); i++){
            if (i == index){
                continue;
            }
            std::cout << s[i];
        }
        std::cout << std::endl;
        return true;
    }
    return false;
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::vector<std::string> ids;
    std::ifstream input_file;
    input_file.open(file_name);
    for (std::string line; std::getline(input_file, line);){
        ids.push_back(line);
    }
    input_file.close();
    std::sort(ids.begin(), ids.end());
    int n_words = ids.size();
    for (int i = 0; i<n_words; i++){
        for (int j = i+1; j<n_words; j++){
            bool found = check_differences(ids[i], ids[j]);
            if (found) return 0;
        }
    }
}
