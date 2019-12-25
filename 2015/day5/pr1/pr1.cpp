#include <set>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>

bool isVowel(char check){
    return check == 'a' or check == 'e' or check == 'i' or check == 'o' or check == 'u';
}

bool checkNice(std::string& answer){
    int vowels = 0;
    std::string duo = "xx";
    char vowel;
    bool has_repeat = false;
    for (size_t i=0; i<answer.length()-1; i++){
        duo = answer.substr(i, 2);
        vowel = answer[i];
        if (duo == "ab" or duo == "cd" or duo == "pq" or duo == "xy") return false;
        if (isVowel(vowel)) vowels++;
        if (duo[0] == duo[1]) has_repeat = true;
    }    
    if (isVowel(answer[answer.length()-1])) vowels++;
    return vowels > 2 and has_repeat;
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
