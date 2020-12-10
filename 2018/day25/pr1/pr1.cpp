#include <map>
#include <set>
#include <queue>
#include <string>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <unistd.h>


typedef std::vector<int> point;
typedef std::vector<point> points;


int manhattan(const point& coord1, const point& coord2){
    int distance = 0;
    for (size_t i = 0; i< coord1.size(); i++){
        distance += std::abs(coord1[i]-coord2[i]);
    }
    return distance;
}

bool check_belongs_to_cluster(const points& elements, const point& current_point){
    for (auto& element: elements){
        if (manhattan(element, current_point) < 4){
            return true;
        }
    }
    return false;
}

bool check_mergeable_clusters(const points& cluster1, const points& cluster2){
    for (auto& element: cluster2){
        if (check_belongs_to_cluster(cluster1, element)){
            return true;
        }
    }
    return false;
}

std::ostream& operator<<(std::ostream& out, const point& out_point){
    for (auto& element: out_point){
        out << element << " ";
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const std::set<size_t>& out_set){
    for (auto& element: out_set){
        out << element << " ";
    }
    return out;
}

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    std::vector<points> all_points;
    input_file.open(file_name);
    for(std::string line; std::getline(input_file, line);){
        std::vector<std::string> parsed;
        boost::split(parsed, line, boost::is_any_of(","), boost::token_compress_on);
        point current;
        std::transform(parsed.begin(), parsed.end(), std::back_inserter(current), [](const std::string& s){ return std::stoi(s); });
        bool is_new_cluster = true;
        for (auto& cluster: all_points){
            bool belongs = check_belongs_to_cluster(cluster, current);
            if (belongs){
                cluster.push_back(current);
                is_new_cluster = false;
                break;
            }
        }
        if (is_new_cluster){
            points new_cluster = {current};
            all_points.push_back(new_cluster);
        }
    }
    input_file.close();
    std::map<size_t, std::set<size_t>> mergeable;
    size_t n = all_points.size();
    for (size_t i=0; i<n; i++){
        if (mergeable.find(i) == mergeable.end()){
            mergeable[i] = {};
        }
        for (size_t j=i+1; j<n; j++){
            if (check_mergeable_clusters(all_points[i], all_points[j])){
                if (mergeable.find(i) != mergeable.end()){
                    mergeable[i].insert(j);
                }
                else{
                    mergeable[i] = {j};
                }
                if (mergeable.find(j) != mergeable.end()){
                    mergeable[j].insert(i);
                }
                else{
                    mergeable[j] = {i};
                }
            }
        }
    }
    std::set<size_t> visited;
    for (auto element: mergeable){
        size_t source = element.first;
        std::set<size_t> children = element.second;
        if (children.empty()) continue;
        if (visited.find(source) != visited.end()) continue;
        std::queue<size_t, std::deque<size_t>> pending;
        for (size_t node: children){
            pending.push(node);
        }
        visited.insert(source);
        pending.push(source);
        while (not pending.empty()){
            size_t node = pending.front();
            pending.pop();
            if (visited.find(node) != visited.end()) continue;
            visited.insert(node);
            mergeable[source].insert(node);
            for (size_t child: mergeable[node]){
                pending.push(child);
            }
            mergeable.erase(mergeable.find(node));
        }
    }
    std::cout << "There are " << mergeable.size() << " constellations" << std::endl;
}
