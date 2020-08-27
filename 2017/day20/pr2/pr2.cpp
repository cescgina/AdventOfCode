#include <map>
#include <queue>
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

std::vector<int> update_position(const std::vector<int>particle, const int& t, const int& acc_factor){
    std::vector<int> new_pos(3, 0);
    new_pos[0] = particle[0]+particle[3]*t+acc_factor*particle[6];
    new_pos[1] = particle[1]+particle[4]*t+acc_factor*particle[7];
    new_pos[2] = particle[2]+particle[5]*t+acc_factor*particle[8];
    return new_pos;
}

std::vector<int> parse_particles(std::string& particle){
    boost::erase_all(particle, "p");
    boost::erase_all(particle, "v");
    boost::erase_all(particle, "a");
    boost::erase_all(particle, "<");
    boost::erase_all(particle, ">");
    boost::erase_all(particle, "=");
    boost::erase_all(particle, " ");
    std::vector<std::string> parsed;
    boost::split(parsed, particle, boost::is_any_of(", "), boost::token_compress_on);
    std::vector<int> values;
    std::transform(parsed.begin(), parsed.end(), std::back_inserter(values), [](const std::string& s){ return std::stoi(s); });
    return values;
}

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    std::map<std::pair<int, int>, char> grid;
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::map<int, std::vector<int>> particles, particle_positions;
    int i = 0;
    for(std::string line; std::getline(input_file, line);){
        particles[i] = parse_particles(line);
        i++;
    }
    input_file.close();
    int time = 1, n_particles = particles.size();
    for (int j = 0; j < n_particles; j++){
        particle_positions[j] = {0, 0, 0};
    }
    while (time < 1000){
        int accel_factor = (time*(time+1))/2;
        for (int j = 0; j < n_particles; j++){
            if (particle_positions.find(j) == particle_positions.end()){
                continue;
            }
            particle_positions[j] = update_position(particles[j], time, accel_factor);
        }
        time++;
        // check for collisions
        for (int m=0; m < n_particles; m++){
            if (particle_positions.find(m) == particle_positions.end()){
                continue;
            }
            std::vector<int> pos = particle_positions[m];
            bool collision = false;
            for(auto it = particle_positions.begin(); it != particle_positions.end(); ) {
                if(it->second == pos and it->first != m){
                    collision = true;
                    it = particle_positions.erase(it);
                }
                else{
                    ++it;
                }
            }
            if (collision){
                particle_positions.erase(particle_positions.find(m));
            }
        }
        n_particles = particle_positions.size();
    }
    std::cout << n_particles << std::endl;
}
