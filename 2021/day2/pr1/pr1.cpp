#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    int depth = 0, horizontal = 0;
    file_name = std::string(argv[optind]);
    std::ifstream input_file(file_name);
    for (std::string line; std::getline(input_file, line);){
        auto pos = line.find(" ");
        std::string command = line.substr(0, pos);
        int units = std::stoi(line.substr(pos+1));
        if (command == "down"){
            depth += units;
        }
        else if (command == "up"){
            depth -= units;
        }
        else {
            horizontal += units;
        }
    }
    input_file.close();
    std::cout << horizontal*depth << std::endl;
}
