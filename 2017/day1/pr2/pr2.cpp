#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::string line; 
    std::getline(input_file, line);
    input_file.close();
    int n = line.size();
    int offset = n/2;
    int sum = 0;
    for (size_t i=0; i<line.size(); i++){
        if (line[i] == line[(i+offset) % n]){
            sum += line[i]-'0';
        }
    }
    std::cout << sum << std::endl;
}
