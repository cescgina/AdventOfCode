#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <unistd.h>

int parse_command(const std::string& direction, const int& race_time){
    std::string name;
    std::string::size_type n, n_prev;
    int dist = 0, run_time = 0, rest_time = 0, speed = 0;
    n = direction.find(" can ");
    n_prev = n;
    name = direction.substr(0, n);
    n = direction.find(" km/s ", n_prev+1);
    speed = std::stoi(direction.substr(n_prev+9, n-n_prev-8));
    n_prev = n;
    n = direction.find(" seconds, ", n_prev+1);
    run_time = std::stoi(direction.substr(n_prev+10, n-n_prev-9));
    n_prev = n;
    n = direction.find(" seconds.", n_prev+1);
    rest_time = std::stoi(direction.substr(n_prev+33, n-n_prev-31));
    auto div_res = std::div(race_time, run_time+rest_time);
    dist += speed*div_res.quot*run_time;
    if (div_res.rem < run_time){
        dist += div_res.rem * speed;
    }
    else{
        dist += run_time * speed;
    }
    return dist;
}

int main(int argc, char** argv){
    std::string file_name, command;
    int dist = 0, max_dist = 0, race_duration = 0;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::getline(input_file, command);
    race_duration = std::stoi(command);
    while (std::getline(input_file, command)){
        dist = parse_command(command, race_duration);
        if (dist > max_dist) max_dist = dist;
    }
    input_file.close();
    std::cout << "Winner has traveled " << max_dist << std::endl;
}
