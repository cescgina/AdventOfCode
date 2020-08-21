#include <set>
#include <fstream>
#include <utility>
#include <iostream>
#include <unistd.h>

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    std::set<std::pair<unsigned int, unsigned int>> valid;
    std::set<std::pair<unsigned int, unsigned int>> new_add;
    std::set<std::pair<unsigned int, unsigned int>> to_delete;
    valid.insert(std::make_pair(0, 4294967295));
    input_file.open(file_name);
    unsigned int left = 0, right = 0;
    for (std::string line; std::getline(input_file, line);){
        auto pos = line.find("-");
        left = std::stoul(line.substr(0, pos));
        right = std::stoul(line.substr(pos+1));
        new_add.clear();
        to_delete.clear();
        for (auto it=valid.begin(); it != valid.end(); it++){
            if (it->first > right) break;
            if (left >= it->first or right <= it->second){
                to_delete.insert(*it);
                if (left > it->first) new_add.insert(std::make_pair(it->first, std::min(left-1, it->second)));
                if (right < it->second) new_add.insert(std::make_pair(right+1, it->second));
            }
        }
        for (auto it=to_delete.begin(); it != to_delete.end(); it++){
            valid.erase(valid.find(*it));
        }
        for (auto it=new_add.begin(); it != new_add.end(); it++){
            valid.insert(*it);
        }
    }
    input_file.close();
    std::cout << valid.begin()->first << std::endl;
}
