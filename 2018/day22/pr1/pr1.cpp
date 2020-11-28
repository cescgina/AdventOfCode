#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <unistd.h>

typedef std::vector<long long> row;
typedef std::vector<row> grid;

void print_risk(grid& cave){
    int target_x = cave[0].size();
    int target_y = cave.size();
    for (int i=0; i<target_y; i++){
        for (int j=0; j<target_x; j++){
            if (cave[i][j] == 0){
                std::cout << '.';
            }
            else if (cave[i][j] == 1){
                std::cout << '=';
            }
            else if (cave[i][j] == 2){
                std::cout << '|';
            }
            else{
                std::cout << '~';
            }
        }
        std::cout << std::endl;
    }
}

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::getline(input_file, line);
    auto pos = line.find(":");
    int depth = std::stoi(line.substr(pos+2));
    std::getline(input_file, line);
    pos = line.find(":");
    std::string coords = line.substr(pos+2);
    pos = coords.find(",");
    int target_x = std::stoi(coords.substr(0, pos))+1;
    int target_y = std::stoi(coords.substr(pos+1))+1;
    input_file.close();
    grid erosion(target_y, row(target_x, 0));
    grid geological(target_y, row(target_x, 0));
    grid types(target_y, row(target_x, 0));
    int risk_total = 0;
    for (int i=0; i<target_y; i++){
        geological[i][0] = i*48271;
        erosion[i][0] = (geological[i][0]+depth) % 20183;
        types[i][0] = erosion[i][0] % 3;
        risk_total += types[i][0];
    }
    for (int i=0; i<target_x; i++){
        geological[0][i] = i*16807;
        erosion[0][i] = (geological[0][i]+depth) % 20183;
        types[0][i] = erosion[0][i] % 3;
        risk_total += types[0][i];
    }
    for (int i=1; i<target_y; i++){
        for (int j=1; j<target_x; j++){
            if ((i == (target_y-1)) and (j == (target_x-1))){
                erosion[i][j] = depth % 20183;
                types[i][j] = erosion[i][j] % 3;
                risk_total += types[i][j];
                continue;
            }
            geological[i][j] = erosion[i-1][j]*erosion[i][j-1];
            erosion[i][j] = (geological[i][j]+depth) % 20183;
            types[i][j] = erosion[i][j] % 3;
            risk_total += types[i][j];
        }
    }
    //print_risk(types);
    std::cout << "Total risk is " << risk_total << std::endl;
}
