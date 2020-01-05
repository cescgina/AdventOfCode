#include <vector>
#include <string>
#include <fstream>
#include <numeric>
#include <iostream>
#include <unistd.h>

typedef std::vector<std::vector<int>> grid;

void parse_command(std::string& order, grid& light_grid){
    std::string action, modifier = "", initial, end;
    std::string::size_type n, n_prev;
    int x_init, y_init, y_end, x_end;
    n = order.find(" ");
    action = order.substr(0, n);
    n_prev = n;
    if (action == "turn"){
        n = order.find(" ", n+1);
        modifier = order.substr(n_prev+1, n-n_prev-1);
        n_prev = n;
    }
    n = order.find(" ", n+1);
    initial = order.substr(n_prev+1, n-n_prev);
    n_prev = n;
    // skip word "through" and spaces
    end = order.substr(n+9);
    n = initial.find(",");
    y_init = std::stoi(initial.substr(0, n));
    x_init = std::stoi(initial.substr(n+1));
    n = end.find(",");
    y_end = std::stoi(end.substr(0, n));
    x_end = std::stoi(end.substr(n+1));
    for (int i=x_init; i<=x_end; i++){
        for (int j=y_init; j<=y_end; j++){
            if (modifier == "off"){
                light_grid[i][j] = 0;
            }
            else if (modifier == "on"){
                light_grid[i][j] = 1;
            }
            else{
                light_grid[i][j] = light_grid[i][j] ? 0 : 1;        
            }
        }
    }

}

int main(int argc, char** argv){
    std::string file_name, command;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    int ligths_on = 0;
    std::ifstream input_file;
    grid ligths(1000, std::vector<int>(1000, 0)); 
    input_file.open(file_name);
    while (std::getline(input_file, command)){
        parse_command(command, ligths);
    }
    input_file.close();
    for (int i=0; i < 1000; i++){
        ligths_on += std::accumulate(ligths[i].begin(), ligths[i].end(), 0);
    }
    std::cout << "Answer is " << ligths_on << std::endl;
}
