#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

bool DEBUG = false;

typedef std::pair<int, int> limits;
typedef std::unordered_map<std::string, std::pair<limits, limits>> fields;
typedef std::vector<int> ticket;

bool contains(fields& haystack, std::string needle){
    return haystack.find(needle) != haystack.end();
}

bool is_valid_field(fields& all_fields, int value){
    for (auto& field: all_fields){
        limits limit1 = field.second.first;
        limits limit2 = field.second.second;
        if (value >= limit1.first and value <= limit1.second) return true;
        else if (value >= limit2.first and value <= limit2.second) return true;
    }
    return false;
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    fields all_fields;
    std::ifstream input_file(file_name);
    for (std::string line; std::getline(input_file, line);){
        if (line.empty()) break;
        auto pos = line.find(":");
        std::string class_name = line.substr(0, pos);
        std::string values = line.substr(pos+2);
        pos = values.find("or");
        std::string limits_1 = values.substr(0, pos-1);
        std::string limits_2 = values.substr(pos+3);
        pos = limits_1.find("-");
        int low1 = std::stoi(limits_1.substr(0, pos));
        int high1 = std::stoi(limits_1.substr(pos+1));
        pos = limits_2.find("-");
        int low2 = std::stoi(limits_2.substr(0, pos));
        int high2 = std::stoi(limits_2.substr(pos+1));
        all_fields[class_name] = std::make_pair(std::make_pair(low1, high1),std::make_pair(low2, high2));
    }
    std::string to_throw;
    std::getline(input_file, to_throw);
    ticket my_ticket;
    for (std::string line; std::getline(input_file, line);){
        if (line.empty()) break;
        std::vector<std::string> to_parse;
        boost::split(to_parse, line, boost::is_any_of(","), boost::token_compress_on);
        for (auto& c: to_parse){
            my_ticket.push_back(std::stoi(c));
        }
    }
    std::getline(input_file, to_throw);
    int error_tickets = 0;
    for (std::string line; std::getline(input_file, line);){
        std::vector<std::string> to_parse;
        boost::split(to_parse, line, boost::is_any_of(","), boost::token_compress_on);
        for (auto& c: to_parse){
            int value = std::stoi(c);
            if (not is_valid_field(all_fields, value)) error_tickets+=value;
        }
    }
    input_file.close();
    std::cout << error_tickets << std::endl;
}
