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
    std::vector<int> target;
    for (auto c: line){
        target.push_back(c-'0');
    }
    size_t len_num = 6;
    if (file_name == "data/test.txt") len_num = 5;
    std::vector<int> recipes = {3, 7};
    int curr_pos = 0, curr_pos_1 = 1, new_score = 0;
    while (true){
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
        auto it = recipes.end()-len_num-1;
        if (recipes.size() > len_num and std::equal(it, it+len_num, target.begin(), target.end())){
            std::cout << recipes.size()-len_num-1 << std::endl;
            return 0;
        }
        it = recipes.end()-len_num;
        if (recipes.size() > len_num and std::equal(it, it+len_num, target.begin(), target.end())){
            std::cout << recipes.size()-len_num << std::endl;
            return 0;
        }
    }
}
