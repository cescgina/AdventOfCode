#include <set>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    std::vector<int> blocks;
    std::vector<std::string> parsed_line;
    input_file.open(file_name);
    std::string line; 
    std::getline(input_file, line);
    input_file.close();
    boost::split(parsed_line, line, boost::is_any_of(" \t"), boost::token_compress_on);
    std::transform(parsed_line.begin(), parsed_line.end(), std::back_inserter(blocks), [](const std::string& s){ return std::stoi(s); });
    int n_blocks = blocks.size();
    int steps = 0;
    std::set<std::vector<int>> history;
    while (true){
        auto max_el = std::max_element(blocks.begin(), blocks.end());
        int i = max_el-blocks.begin();
        int n = blocks[i];
        blocks[i] = 0;
        i++;
        while (n > 0){
            blocks[i % n_blocks]++;
            n--;
            i++;
        }
        steps++;
        if (history.find(blocks) != history.end()){
            break;

        }
        else{
            history.insert(blocks);
        }
    }
    std::cout << steps << std::endl;
}
