#include <iostream>
#include <fstream>
#include <unistd.h>

int main(int argc, char** argv){
    int floor_val = 0;
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    char step = 'c';
    std::ifstream input_file;
    input_file.open(file_name);
    while (input_file.get(step)){
        if (step == '(') ++floor_val;
        else if (step == ')') --floor_val;
    }

    input_file.close();
    std::cout << "Floor is " << floor_val << std::endl;
}
