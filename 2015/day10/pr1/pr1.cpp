#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>

std::string process_str(const std::string& input){
    char c, prev_c='a';
    int counter = 0;
    std::string code;
    std::vector<int> counts;
    std::vector<char> chars;
    for (size_t i=0; i<input.size(); i++){
        c = input[i];
        if (prev_c != c and prev_c != 'a'){
           counts.push_back(counter);
           chars.push_back(prev_c);
           counter = 0; 
        }
        counter++;
        prev_c = c;
        if (i == input.size()-1){
           counts.push_back(counter);
           chars.push_back(c);
        }
    }
    for (size_t i=0; i<counts.size(); i++){
        code += std::to_string(counts[i]);
        code.push_back(chars[i]);
    }
    return code;
}

int main(int argc, char** argv){
    std::string file_name, input;
    int n_tries = 40;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::getline(input_file, input);
    input_file.close();
    for (int j=0; j<n_tries; j++){
        input = process_str(input);
    }
    std::cout << input.size() << std::endl;
}
