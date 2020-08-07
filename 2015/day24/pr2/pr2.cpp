#include <vector>
#include <string>
#include <numeric>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>

typedef std::vector<int> elements;

template <typename T>
void print_vector(std::vector<T>& vec){
    for (auto it=vec.begin(); it != vec.end(); it++){
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

bool can_split(elements & groups, int weigth_goal){
    do {
        int sum = groups[0];
        size_t i = 1, n = groups.size()-1, j = n-1;
        for (; i < groups.size(); i++ ){
            sum += groups[i];
            if (sum == weigth_goal){
                break;
            }         
        }
        int sum_back = groups[n];
        for ( ; j > 1; j-- ){
            sum_back += groups[j];
            if (sum_back == weigth_goal){
                break;
            }         
        }
        if (j > i and std::accumulate(groups.begin()+i+1, groups.begin()+j, 0) == weigth_goal){
            return true;
        }
    } while (std::next_permutation(groups.begin(), groups.end()));
    return false;
}

std::vector<elements> comb(int N, int K, elements & pack, int goal_sum) {
    std::vector<elements> possible_comb;
    std::string bitmask(K, 1); // K leading 1's
    bitmask.resize(N, 0); // N-K trailing 0's

    // print integers and permute bitmask
    do {
        elements combination = elements(K, 0);
        int sum = 0, j = 0;
        for (int i = 0; i < N; ++i) // [0..N-1] integers
        {
            if (bitmask[i]) {
                sum += pack[i];
                combination[j] = i;
                j++;
            }
        }
        if (sum == goal_sum){
            possible_comb.push_back(combination);
        }
    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));
    return possible_comb;
}

int main(int argc, char** argv){
    std::string file_name, instruction, reg;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    elements packages;
    for (std::string line; std::getline(input_file, line);){
        packages.push_back(std::stoi(line));
    }
    int total_sum = std::accumulate(packages.begin(), packages.end(), 0);
    int weight = total_sum / 4, size = 2; 
    long qe = 1000000000000;
    bool found_groups = false, is_split;
    std::cout << "Total weight " << total_sum << " goal " <<  weight << std::endl;
    print_vector(packages);
    while (size <= (int)packages.size()/4){
        std::cout << "Starting with size " << size << std::endl;
        // find possible group1 of specified size
        std::vector<elements> possible_comb = comb(packages.size(), size, packages, weight);
        for (auto it=possible_comb.begin(); it != possible_comb.end(); it++){
            // check if provide possible equal weight groupings
            elements new_groups(packages);
            elements indices = *it;
            long new_qe = 1;
            for (auto it_comb=indices.end()-1; it_comb != indices.begin()-1; it_comb--){
                new_groups.erase(new_groups.begin()+*it_comb);
                new_qe *= packages[*it_comb];
            }
            if (new_qe >= qe){
                continue;
            }
            is_split = can_split(new_groups, weight);   
            if (is_split){
                found_groups = true;
                qe = std::min(qe, new_qe);
            }
        }
        if (found_groups){
            break;
        }
        // if any found, return the one with lowest QE
        size++;
    }
    std::cout << "QE for optimal groups is " << qe << std::endl;
}
