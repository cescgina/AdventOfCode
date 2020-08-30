#include <fstream>
#include <iostream>
#include <unistd.h>

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::string polymer; 
    std::getline(input_file, polymer);
    auto it = polymer.begin();
    while (it != polymer.end()){
        if (std::abs(*it-*(it+1)) == 32){
            it = polymer.erase(it, it+2)-1;
        }
        else{
            it++;
        }
    }
    std::cout << polymer.size() << std::endl;
}
