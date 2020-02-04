#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <iostream>
#include <algorithm>
#include <unistd.h>

typedef std::map<std::string, int> weight_map;
typedef std::map<std::string, weight_map> santa_map;
typedef std::vector<std::string> nodes;

void parse_command(const std::string& direction, santa_map& distances, nodes& nodes_list){
    std::string source, destination;
    std::string::size_type n, n_prev;
    int dist;
    n = direction.find(" to ");
    n_prev = n;
    source = direction.substr(0, n);
    n = direction.find(" = ", n_prev+4);
    destination = direction.substr(n_prev+4, n-n_prev-4);
    dist = std::stoi(direction.substr(n+3));
    if (std::find(nodes_list.begin(), nodes_list.end(), source) == nodes_list.end()){
        nodes_list.push_back(source);
    }
    if (std::find(nodes_list.begin(), nodes_list.end(), destination) == nodes_list.end()){
        nodes_list.push_back(destination);
    }
    if (distances.find(source) != distances.end()){
        distances[source].insert(std::make_pair(destination, dist));
    }
    else{
        distances[source] = {std::make_pair(destination, dist)};
    }
    if (distances.find(source) != distances.end()){
        distances[destination].insert(std::make_pair(source, dist));
    }
    else{
        distances[destination] = {std::make_pair(source, dist)};
    }
}

void print_vector(const nodes& elements){
    for (auto it=elements.begin(); it!=elements.end(); it++){
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}
int get_longest_distance(santa_map& distances, nodes& nodes_left){
    int dist = 0, max_dist=0, n=nodes_left.size();
    std::sort(nodes_left.begin(), nodes_left.end());
    while(std::next_permutation(nodes_left.begin(), nodes_left.end())){
        dist = 0;
        for (int it=0; it < n-1; it++){
            dist += distances[nodes_left[it]][nodes_left[it+1]];
        }
        if (dist > max_dist){
            max_dist = dist;
        }
    }
    return max_dist;
}

int main(int argc, char** argv){
    std::string file_name, command;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    santa_map dist_map;
    nodes nodes_queue;
    while (std::getline(input_file, command)){
        parse_command(command, dist_map, nodes_queue);
    }
    input_file.close();
    std::cout << "Longest distance is " << get_longest_distance(dist_map, nodes_queue) << std::endl;
}
