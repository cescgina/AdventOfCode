#include <map>
#include <set>
#include <tuple>
#include <queue>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <numeric>
#include <utility>
#include <iostream>
#include <algorithm>
#include <unistd.h>

typedef std::vector<std::vector<int>> blueprint;
typedef std::set<int> locations;
typedef std::set<std::pair<int, int>> seen_coords;
typedef std::tuple<int, int, int> info;

void print_vector(const blueprint& state){
    for (size_t ii=0; ii<state.size(); ii++){
        for (size_t jj=0; jj<state[ii].size(); jj++){
            if (state[ii][jj] == -2) std::cout << '#';
            else if (state[ii][jj] == -1) std::cout << '.';
            else std::cout << state[ii][jj];
        }
        std::cout << std::endl;
    }
}

int distance(const int& x_pos, const int& y_pos, const std::map<int, std::pair<int, int>>& points, const locations& seen){
    int tot_dist = 0;
    for(auto coords: points){
        if (seen.find(coords.first) != seen.end()) continue;
        tot_dist += std::abs(x_pos-coords.second.first)+std::abs(y_pos-coords.second.second);
    }
    return tot_dist;
}

int find_distance(const blueprint& grid, const int src, const int dest, std::map<int, std::pair<int, int>>& point_coords){
    int steps = 0, x=point_coords[src].first, y=point_coords[src].second; 
    auto cmp = [](info left, info right) { return std::get<0>(left) > std::get<0>(right); };
    std::priority_queue<info, std::vector<info>, decltype(cmp)> paths(cmp);
    paths.push(std::make_tuple(steps, x, y));
    info element;
    seen_coords visited_pos;
    std::vector<std::pair<int,int>> neighbors = {std::make_pair(-1,0), std::make_pair(1,0), std::make_pair(0,-1), std::make_pair(0,1)};
    while (!paths.empty()){
        element = paths.top();
        paths.pop();
        steps = std::get<0>(element);
        x = std::get<1>(element);
        y = std::get<2>(element);
        if (grid[x][y] == dest) return steps;
        for (auto neigh: neighbors){
            if (visited_pos.find(std::make_pair(x+neigh.first,y+neigh.second)) != visited_pos.end()) continue;
            if (grid[x+neigh.first][y+neigh.second] > -2){
                visited_pos.insert(std::make_pair(x+neigh.first, y+neigh.second));
                paths.push(std::make_tuple(steps+1, x+neigh.first, y+neigh.second));
            }
        }
    }
    return -1;
}

int main(int argc, char** argv){
    std::string file_name, line, node;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    blueprint grid;
    locations all, visited;
    std::map<int, std::pair<int, int>> points;
    int i=0, j=0;
    input_file.open(file_name);
    for (std::string line; std::getline(input_file, line);){
        std::vector<int> row;
        j = 0;
        for (auto point: line){
            if (point == '#') row.push_back(-2);
            else if (point == '.') row.push_back(-1);
            else{
                row.push_back(point-'0');
                all.insert(point-'0');
                points[point-'0'] = std::make_pair(i,j);
            }
            j++;
        }
        grid.push_back(row);
        i++;
    }
    input_file.close();
    visited.insert(0);
    int n_points = points.size();
    blueprint point_distances = blueprint(n_points, std::vector<int>(n_points, 0));
    for (int i=0; i<n_points; i++){
        for (int j=i+1; j<n_points; j++){
            point_distances[i][j] = find_distance(grid, i, j, points);
            point_distances[j][i] = point_distances[i][j];
        }
    }
    std::vector<int> nodes = std::vector<int>(n_points-1, 0); 
    std::iota(nodes.begin(), nodes.end(), 1);
    int min_dist = 100000000, src = 0;
    do {
        src = 0;
        int dist = 0;
        for (int l=0; l<n_points-1; l++){
           dist += point_distances[src][nodes[l]];
           src = nodes[l];
        }
        dist += point_distances[src][0];
        min_dist = std::min(min_dist, dist);
    } while(std::next_permutation(nodes.begin(), nodes.end()));
    std::cout << min_dist << std::endl;
}
