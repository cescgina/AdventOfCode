#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <unistd.h>

typedef std::vector<std::vector<int>> distance_matrix;

void print_vector(const std::vector<int>& elements){
    for (auto it=elements.begin(); it!=elements.end(); it++){
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

void parse_command(const std::string& direction, const int& race_time, distance_matrix& dist_mat){
    std::string name;
    std::string::size_type n, n_prev;
    int run_time = 0, rest_time = 0, speed = 0, time_elapsed = 1, increase = 0;
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
    std::vector<int> local_dist(race_time, speed);
    while (time_elapsed < run_time){
        local_dist[time_elapsed] = local_dist[time_elapsed-1]+speed;
        time_elapsed++;
    }
    while (time_elapsed < race_time){
        if (time_elapsed % (run_time+rest_time) == 0){
            for (int j=0; j<run_time; j++){
                local_dist[time_elapsed] = local_dist[time_elapsed-1]+speed;
                time_elapsed++;
                if (time_elapsed == race_time){
                    break;
                }
            }
        } 
        else{
            if (time_elapsed+rest_time >= race_time){
                increase = race_time-time_elapsed;
            }
            else{
                increase = rest_time;
            }
            std::fill(local_dist.begin()+time_elapsed, local_dist.begin()+time_elapsed+increase, local_dist[time_elapsed-1]);
            time_elapsed += increase;
        }
    }
    dist_mat.push_back(local_dist);
}


int score_races(const int& race_time, const int& n_reindeers, const distance_matrix& dist_mat){
    int opt_score = 0, local_score = 0;
    std::vector<int> scores(n_reindeers, 0), max_dist;
    for (int i=0; i<race_time; i++){
        local_score = 0;
        max_dist = {};
        for (int j=0; j<n_reindeers; j++){
            if (dist_mat[j][i] > local_score){
                local_score = dist_mat[j][i];
                max_dist = {j};
            }
            else if (dist_mat[j][i] == local_score){
                max_dist.push_back(j);
            }
        }
        for (auto it=max_dist.begin(); it != max_dist.end(); it++){
            scores[*it]++;
        }
    }
    opt_score = *std::max_element(scores.begin(), scores.end());
    return opt_score;
}

int main(int argc, char** argv){
    std::string file_name, command;
    int race_duration = 0, n_reinders = 0, max_score = 0;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::getline(input_file, command);
    race_duration = std::stoi(command);
    distance_matrix distances;
    while (std::getline(input_file, command)){
        parse_command(command, race_duration, distances);
    }
    input_file.close();
    n_reinders = distances.size();
    max_score = score_races(race_duration, n_reinders, distances);
    std::cout << "Winner has scored " << max_score << std::endl;
}
