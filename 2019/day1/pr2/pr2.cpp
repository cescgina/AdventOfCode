#include <fstream>
#include <iostream>
#include <unistd.h>

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    int total = 0;
    std::ifstream input_file;
    input_file.open(file_name);
    for (std::string line; std::getline(input_file, line);){
        int remaining = (std::stoi(line)/3)-2;
        total += remaining;
        while (remaining > 0){
            remaining = std::max(0, (remaining/3)-2);
            total += remaining;
        }
    }
    std::cout << total << std::endl;
}
