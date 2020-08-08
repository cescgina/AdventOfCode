#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>

bool is_ABBA(const std::string & pattern){
    for (size_t j=0; j<pattern.size()-3; j++){
        if ((pattern[j] == pattern[j+3]) and (pattern[j+1] == pattern[j+2]) and (pattern[j] != pattern[j+1])) return true;
    }
    return false;
}

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    int sum_tls = 0;
    for (std::string line; std::getline(input_file, line);){
        size_t pos = 0, close_pos = -1;
        bool is_abba_in = false;
        pos = line.find("[");
        bool is_abba_out = false;
        while (pos != std::string::npos){
            is_abba_out |= is_ABBA(line.substr(close_pos+1, pos-close_pos-1));
            close_pos = line.find("]", pos+1);
            is_abba_in |= is_ABBA(line.substr(pos+1, close_pos-pos-1));
            pos = line.find("[", close_pos);
        } 
        is_abba_out |= is_ABBA(line.substr(close_pos+1));
        if (is_abba_out and not is_abba_in) sum_tls++;
    }
    std::cout << "There are " << sum_tls << " ips with TLS " << std::endl;
}
