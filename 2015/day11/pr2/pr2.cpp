#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>

std::string process_str(std::string& input){
    size_t index = input.size()-1;
    while (true){
        if (input[index] == 'z'){
            input.replace(index, 1, 1, 'a');
            index--;
        }
        else{
            input.replace(index, 1, 1, input[index]+1);
            break;
        }
    }
    return input;
}

int distance(const char& a, const char& b){
    return b-a;
}

bool is_valid_password(const std::string& pass){
    int pairs = 0, triplets = 0;
    for (size_t i=0; i<pass.size(); i++){
        if (pass[i] == 'i' or pass[i] == 'l' or pass[i] == 'o') return false;
        if (i < pass.size()-2){
            // check for increasing triplets
            if ((distance(pass[i], pass[i+1]) == 1 and distance(pass[i+1], pass[i+2]) == 1)) triplets++;
        }
        if (i < pass.size()-1){
            // accept only pairs that are not overlapping
            if ((pass[i] == pass[i+1]) and (pass[i-1] != pass[i])) pairs++;
        }
    }
    return (pairs > 1) and (triplets > 0);
}

int main(int argc, char** argv){
    std::string file_name, input;
    bool valid_pass = false;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::getline(input_file, input);
    input_file.close();
    while (!valid_pass){
        input = process_str(input);
        valid_pass = is_valid_password(input);
    }
    valid_pass = false;
    while (!valid_pass){
        input = process_str(input);
        valid_pass = is_valid_password(input);
    }
    std::cout << "Next password is " << input << std::endl;
}
