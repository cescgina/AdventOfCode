#include <vector>
#include <string>
#include <numeric>
#include <fstream>
#include <iostream>
#include <unistd.h>


int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::getline(input_file, line);
    input_file.close();
    int n_recipes = std::stoi(line);
    std::vector<int> recipes = {3, 7};
    int curr_pos = 0, curr_pos_1 = 1, new_score = 0;
    while ((int)recipes.size()<(n_recipes+10-1)){
        new_score = recipes[curr_pos]+recipes[curr_pos_1]; 
        if (new_score < 10){
            recipes.push_back(new_score);
        }
        else{
            recipes.push_back((new_score/10) % 10);
            recipes.push_back(new_score % 10);
        }
        curr_pos = (1+curr_pos + recipes[curr_pos]) % recipes.size();
        curr_pos_1 = (1+curr_pos_1 + recipes[curr_pos_1]) % recipes.size();
    }
    for (auto it=(recipes.end()-9); it != recipes.end(); it++){
        std::cout << *it;
    }
    std::cout << std::endl;
}
