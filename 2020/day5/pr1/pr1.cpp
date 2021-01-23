#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    int max_id = 0;
    std::ifstream input_file(file_name);
    for (std::string line; std::getline(input_file, line);){
        int row = 0;
        int seat = 0;
        for (int i = 0; i < 7; i++){
            if (line[i] == 'B'){
                row += (1 << (7-i-1));
            }
        }
        for (int i = 0; i < 3; i++){
            if (line[i+7] == 'R'){
                seat += (1 << (3-i-1));
            }
        }
        max_id = std::max(max_id, row*8+seat);
    }
    input_file.close();

    std::cout << "Highest id is " << max_id << std::endl;
}
