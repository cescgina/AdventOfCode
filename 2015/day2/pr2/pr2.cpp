#include <iostream>
#include <fstream>
#include <algorithm>
#include <unistd.h>

int main(int argc, char** argv){
    int total_length = 0, h, w, l, bow;
    size_t pos, n_pos;
    std::string file_name, dimensions;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    while (std::getline(input_file, dimensions)){
        pos = dimensions.find("x", 0);
        h = std::stoi(dimensions.substr(0, pos));
        n_pos = dimensions.find("x", pos+1);
        w = std::stoi(dimensions.substr(pos+1, n_pos-pos-1));
        l = std::stoi(dimensions.substr(n_pos+1));
        bow = 2*std::min({h+w, w+l, l+h}); 
        total_length += h*l*w;
        total_length += bow;
    }
    input_file.close();
    std::cout << "Total length needed is " << total_length << std::endl;
}
