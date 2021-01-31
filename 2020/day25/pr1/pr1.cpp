#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file(file_name);
    long long int door_pub, card_pub;
    std::getline(input_file, line);
    card_pub = std::stoll(line);
    std::getline(input_file, line);
    door_pub = std::stoll(line);
    input_file.close();
    long long int factor = 20201227;
    long long subject = 7;
    long long number = 1;
    int loop_size = 1;
    while (true){
        number = ((number % factor)*(subject % factor)) % factor;
        if (number == card_pub) break;
        loop_size++;
    }
    subject = door_pub;
    number = 1;
    for (int i = 0; i < loop_size; i++){
        number = ((number % factor)*(subject % factor)) % factor;
    }
    std::cout << number << std::endl;
}
