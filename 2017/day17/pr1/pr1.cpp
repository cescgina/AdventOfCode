#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <iostream>
#include <unistd.h>

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::vector<int> values = {0};
    std::vector<std::string> parsed_line;
    std::getline(input_file, line);
    input_file.close();
    int curr_pos = 0, steps = std::stoi(line), value = 1;
    while(value <= 2017) {
        curr_pos = ((curr_pos+steps) % values.size())+1;
        values.insert(values.begin()+curr_pos, value);
        value++;
    }
    std::cout << values[curr_pos+1] << std::endl;
}
