#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

typedef std::map<std::string, std::tuple<int, int, int>> directions;

int distance(const int x1, const int y1, const int z1){
    return std::max(std::max(std::abs(y1), std::abs(z1)), std::abs(x1));
}

int main(int argc, char** argv){
    std::string file_name, des_reg, cond_reg;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    directions compass;
    compass["n"] = std::make_tuple(0, 1, -1);
    compass["ne"] = std::make_tuple(1, 0, -1);
    compass["nw"] = std::make_tuple(-1, 1, 0);
    compass["s"] = std::make_tuple(0, -1, 1);
    compass["se"] = std::make_tuple(1, -1, 0);
    compass["sw"] = std::make_tuple(-1, 0, 1);
    for (std::string line; std::getline(input_file, line);){
        if (line.size() < 20) std::cout << line << " ";
        std::vector<std::string> parsed_line;
        boost::split(parsed_line, line, boost::is_any_of(","));
        int x = 0, y = 0, z = 0;
        for (auto move: parsed_line){
            auto d = compass[move];
            x += std::get<0>(d);
            y += std::get<1>(d);
            z += std::get<2>(d);
        }
        std::cout << distance(x, y, z) << std::endl;
    }

}
