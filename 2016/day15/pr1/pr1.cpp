#include <set>
#include <map>
#include <vector>
#include <fstream>
#include <utility>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>


int main(int argc, char** argv){
    std::string file_name, line;
    std::vector<std::string> parsed_line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::vector<int> position, slots;
    for(std::string line; std::getline(input_file, line);){
        boost::split(parsed_line, line, boost::is_any_of(" "), boost::token_compress_on);
        position.push_back(std::stoi(parsed_line[11]));
        slots.push_back(std::stoi(parsed_line[3]));
    }
    input_file.close();
    int time = 1, sum = 0, n = position.size();
    while (true){
        sum = 0;
        for (int i = 0; i < n; i++){
            sum += (position[i] + time + i + 1) % slots[i];
        }
        if (sum == 0) break;
        time++;
    }
    std::cout << "First time is " << time << std::endl;
}
