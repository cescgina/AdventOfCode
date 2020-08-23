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
    int sum = 0;
    for (size_t i=0; i<line.size()-1; i++){
        if (line[i] == line[i+1]){
            sum += line[i]-'0';
        }
    }
    if (line[line.size()-1] == line[0]) sum += line[0]-'0';
    std::cout << sum << std::endl;
}
