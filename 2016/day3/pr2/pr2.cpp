#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::vector<std::string> parsed_line;
    std::vector<int> sides = {0, 0, 0};
    std::vector<std::vector<int>> saved_sides = {{0, 0, 0},{0, 0, 0},{0, 0, 0}};
    int possible = 0, i_line = 1;
    for (std::string line; std::getline(input_file, line);){
        boost::split(parsed_line, line, boost::is_any_of("  "), boost::token_compress_on);
        std::transform(parsed_line.begin()+1, parsed_line.end(), sides.begin(), [](std::string c) -> int {return boost::lexical_cast<int>(c);});
        saved_sides[i_line-1] = sides;
        if (i_line == 3){
            for (int j=0; j < 3; j++){
                sides = {saved_sides[0][j], saved_sides[1][j], saved_sides[2][j]};
                std::sort(sides.begin(), sides.end());
                if ((sides[0] + sides[1]) > sides[2]) possible++;
            }
            i_line = 0;
        }
        i_line++;
    }
    std::cout << "There are " << possible << " possible triangles" << std::endl;
}
