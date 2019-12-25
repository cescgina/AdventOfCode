#include <string>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <unistd.h>

bool checkNice(std::string& answer){
    std::string duo = "xx";
    std::unordered_set<std::string> pairs;
    bool has_repeat = false, has_pairs = false;
    for (size_t i=0; i<answer.length()-1; i++){
        if (i < answer.length()-2){
            if (answer[i] == answer[i+2]) has_repeat = true;
        }
        duo = answer.substr(i, 2);
        auto search = pairs.find(duo);
        if (search != pairs.end()){
            // check that repeated pairs do not overlap
            if (duo != answer.substr(i-1, 2)) has_pairs = true;
        }
        else{
            pairs.insert(duo);
        }
    }    
    return has_pairs and has_repeat;
}

int main(int argc, char** argv){
    std::string file_name, str_check;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    int nice_strings = 0;
    std::ifstream input_file;
    input_file.open(file_name);
    while (std::getline(input_file, str_check)){
        if (checkNice(str_check)) nice_strings++; 
    }
    input_file.close();
    std::cout << "Answer is " << nice_strings << std::endl;
}
