#include <map>
#include <set>
#include <cctype>
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
const std::set<std::string> valid_eye_colors = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};

std::ostream& operator<<(std::ostream& out, passport& pass){
    for (auto& c: pass){
        out << c.first << ": " << c.second << ", ";
    }
    return out;
}

template <typename T>
bool contains(T& pass, const std::string& field){
    return pass.find(field) != pass.end();
}

bool my_isxdigit(char ch)
{
    return std::isxdigit(static_cast<unsigned char>(ch));
}

bool my_isdigit(char ch)
{
    return std::isdigit(static_cast<unsigned char>(ch));
}

bool is_valid(passport& pass){
    for (auto& c: required_fields){
        if (not contains(pass, c)) return false;
    }
    bool valid_pass = true;
    int year = std::stoi(pass["byr"]);
    valid_pass &= (year >= 1920 and  year <= 2002);
    year = std::stoi(pass["iyr"]);
    valid_pass &= (year >= 2010 and  year <= 2020);
    year = std::stoi(pass["eyr"]);
    valid_pass &= (year >= 2020 and  year <= 2030);
    std::string height = pass["hgt"];
    if (height.size() < 3) return false;
    std::string unit = height.substr(height.size()-2);
    int height_val = std::stoi(height.substr(0, height.size()-2));
    if (unit == "cm"){
        valid_pass &= (height_val >= 150 and height_val <= 193);
    }
    else{
        valid_pass &= (height_val >= 59 and height_val <= 76);
    }
    std::string hair = pass["hcl"].substr(1);
    valid_pass &= std::all_of(hair.begin(), hair.end(), my_isxdigit);
    valid_pass &= (hair.size() == 6);
    valid_pass &= contains(valid_eye_colors, pass["ecl"]);
    valid_pass &= std::all_of(pass["pid"].begin(), pass["pid"].end(), my_isdigit);
    valid_pass &= (pass["pid"].size() == 9);
    return valid_pass;
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
