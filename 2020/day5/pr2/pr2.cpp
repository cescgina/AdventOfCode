#include <set>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

template <typename T>
bool contains(T& haystack, int needle){
    return haystack.find(needle) != haystack.end();
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::set<int> all_seats;
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
        all_seats.insert(row*8+seat);
    }
    input_file.close();

    int seat = max_id;
    while (contains(all_seats, seat)){
        seat--;
    }
    std::cout << "Your seat is " << seat << std::endl;
}
