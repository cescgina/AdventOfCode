#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

typedef std::map<std::string, std::string> passport;
typedef std::vector<std::string> to_parse;
const to_parse required_fields = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"};

bool contains(passport& pass, const std::string& field){
    return pass.find(field) != pass.end();
}

bool is_valid(passport& pass){
    for (auto& c: required_fields){
        if (not contains(pass, c)) return false;
    }
    return true;
}

std::ostream& operator<<(std::ostream& out, passport& pass){
    for (auto& c: pass){
        out << c.first << ", ";
    }
    return out;
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    int correct = 0;
    std::ifstream input_file(file_name);
    passport pass;
    for (std::string line; std::getline(input_file, line);){
        to_parse entry;
        if (line.empty()){
            if (is_valid(pass)) {
                correct++;
            }
            pass.clear();
            continue;
        }
        boost::split(entry, line, boost::is_any_of(" "), boost::token_compress_on);
        for (auto& field: entry){
            to_parse values;
            boost::split(values, field, boost::is_any_of(":"), boost::token_compress_on);
            pass[values[0]] = values[1];
        }
    }
    input_file.close();
    if (is_valid(pass)) {
        correct++;
    }
    std::cout << "There are " << correct << " correct passwords "<< std::endl;
}
