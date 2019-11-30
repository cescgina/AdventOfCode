#include <iostream>
#include <fstream>
#include <algorithm>
#include <unistd.h>

int main(int argc, char** argv){
    int total_area = 0, h, w, l, slack;
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
        slack = std::min({h*w, w*l, l*h}); 
        total_area += 2*l*w + 2*w*h + 2*h*l;
        total_area += slack;
    }
    input_file.close();
    std::cout << "Total area needed is " << total_area << std::endl;
}
