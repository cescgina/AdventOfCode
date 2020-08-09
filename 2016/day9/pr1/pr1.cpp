#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>


int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    int length_decompress = 0;
    std::getline(input_file, line);
    size_t n = line.size(), i=0;
    while ( i < n ){
        if (line[i] == '('){
            auto pos = line.find("x", i);
            int chars = std::stoi(line.substr(i+1, pos-i-1));
            auto close_pos = line.find(")", pos);
            int repeats = std::stoi(line.substr(pos+1, close_pos-pos-1));
            length_decompress += chars*repeats;
            i = close_pos+chars+1;
        }
        else{
            length_decompress++;
            i++;
        }
    }
    std::cout << "Total length of decompressed message is  " << length_decompress << std::endl;
}
