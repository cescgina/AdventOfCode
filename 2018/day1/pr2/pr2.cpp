#include <set>
#include <vector>
#include <fstream>
#include <iostream>
#include <unistd.h>

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::set<int> frequencies_seen;
    std::vector<int> frequencies;
    int freq = 0;
    std::ifstream input_file;
    input_file.open(file_name);
    for (std::string line; std::getline(input_file, line);){
        frequencies.push_back(std::stoi(line));
        freq += std::stoi(line);
        if (frequencies_seen.find(freq) != frequencies_seen.end()){
            std::cout << freq << std::endl;
            return 0;
        }
        frequencies_seen.insert(freq);
    }
    while (true){
        for (int f: frequencies){
            freq += f;
            if (frequencies_seen.find(freq) != frequencies_seen.end()){
                std::cout << freq << std::endl;
                return 0;
            }
            frequencies_seen.insert(freq);
        }
    }
}
