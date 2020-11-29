#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <unistd.h>

struct nanobot{
    int id;
    int x;
    int y;
    int z;
    int radius;
};
typedef std::vector<nanobot> nanobots;

int distance(nanobot& nano1, nanobot& nano2){
    return std::abs(nano1.x-nano2.x)+std::abs(nano1.y-nano2.y)+std::abs(nano1.z-nano2.z);
}

nanobot parse_line(const std::string& line, int id_nano){
    int x, y, z, radius;
    auto pos = line.find("<");
    auto pos_end = line.find(">");
    radius = std::stoi(line.substr(pos_end+5));
    std::string coords = line.substr(pos+1, pos_end-pos);
    pos = coords.find(",");
    x = std::stoi(coords.substr(0,pos));
    pos_end = coords.find(",", pos+1);
    y = std::stoi(coords.substr(pos+1, pos_end-pos));
    z = std::stoi(coords.substr(pos_end+1));
    return nanobot{id_nano, x, y, z, radius};
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    int id_max = 0, max_radius = 0, id = 0;
    nanobots all_nanobots;
    for (std::string line; std::getline(input_file, line);){
        nanobot nano = parse_line(line, id);
        if (nano.radius > max_radius){
            max_radius = nano.radius;
            id_max = id;
        }
        all_nanobots.push_back(nano);
        id++;
    }
    input_file.close();
    int in_range = 1;
    for (auto& nano: all_nanobots){
        if (nano.id == id_max) continue;
        in_range += (distance(all_nanobots[id_max], nano) <= max_radius);
    }
    std::cout << in_range << " in radius of the largest signal radius" << std::endl;
}
