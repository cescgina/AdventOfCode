#include <set>
#include <iostream>
#include <fstream>
#include <utility>
#include <unistd.h>

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    char step = 'c';
    auto coords = std::make_pair(0, 0);
    std::set<std::pair<int, int>> visited = {coords};
    input_file.open(file_name);
    while (input_file.get(step)){
        if (step == '>'){
            coords.second++;
        }
        else if (step == '<'){
            coords.second--;
        }
        else if (step == 'v'){
            coords.first--;
        }
        else if (step == '^'){
            coords.first++;
        }
        visited.insert(coords);
    }
    input_file.close();
    std::cout << "Total houses visited is " << visited.size() << std::endl;
}
