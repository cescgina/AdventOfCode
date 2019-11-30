#include <iostream>
#include <fstream>
#include <unistd.h>

int main(int argc, char** argv){
    int floor_val = 0, pos = 0;
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    char step = 'c';
    std::ifstream input_file;
    input_file.open(file_name);
    while (input_file.get(step)){
        ++pos;
        if (step == '(') ++floor_val;
        else if (step == ')') --floor_val;
        if (floor_val == -1) break;
    }

    input_file.close();
    std::cout << "Floor is reached at position " << pos << std::endl;
}
