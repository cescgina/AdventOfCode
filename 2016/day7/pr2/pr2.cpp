#include <map>
#include <set>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>

std::pair<bool, std::vector<size_t>> is_ABA(const std::string & pattern){
    bool found = false;
    std::vector<size_t> indices;
    for (size_t j=0; j<pattern.size()-2; j++){
        if ((pattern[j] == pattern[j+2]) and (pattern[j] != pattern[j+1])){
            found = true;
            indices.push_back(j);
        };
    }
    return std::make_pair(found, indices);
}

bool match_ABA_BAB(const std::set<std::string> & ABAs, const std::set<std::string> & BABs){
    for (auto it_ABA=ABAs.begin(); it_ABA != ABAs.end(); it_ABA++){
        for (auto it_BAB=BABs.begin(); it_BAB != BABs.end(); it_BAB++){
            if ((it_ABA->at(0) == it_BAB->at(1)) and (it_ABA->at(1) == it_BAB->at(0))) return true;
        }
    }
    return false;
}

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    int sum_ssl = 0;
    for (std::string line; std::getline(input_file, line);){
        size_t pos = 0, close_pos = -1;
        pos = line.find("[");
        std::set<std::string> list_ABA;
        std::set<std::string> list_BAB;
        while (pos != std::string::npos){
            auto res = is_ABA(line.substr(close_pos+1, pos-close_pos-1));
            if (res.first){
                for (auto it=res.second.begin(); it != res.second.end(); it++){
                    list_ABA.insert(line.substr(close_pos+1+*it, 3));
                }
            }
            close_pos = line.find("]", pos+1);
            res = is_ABA(line.substr(pos+1, close_pos-pos-1));
            if (res.first){
                for (auto it=res.second.begin(); it != res.second.end(); it++){
                    list_BAB.insert(line.substr(pos+1+*it, 3));
                }
            }
            pos = line.find("[", close_pos);
        } 
        auto res = is_ABA(line.substr(close_pos+1));
        if (res.first){
            for (auto it=res.second.begin(); it != res.second.end(); it++){
                list_ABA.insert(line.substr(close_pos+1+*it, 3));
            }
        }
        if (match_ABA_BAB(list_ABA, list_BAB)) sum_ssl++;
    }
    std::cout << "There are " << sum_ssl << " ips with SSL " << std::endl;
}
