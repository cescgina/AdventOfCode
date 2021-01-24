#include <set>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

bool DEBUG = false;

typedef std::pair<int, int> limits;
typedef std::unordered_map<std::string, std::pair<limits, limits>> fields;
typedef std::vector<int> ticket;

std::ostream& operator<<(std::ostream& out, std::set<std::string>& fields_store){
    for (auto& el: fields_store){
        out << el << " ";
    }
    return out;
}

template<typename T>
bool contains(T& haystack, std::string needle){
    return haystack.find(needle) != haystack.end();
}

void add_matching_fields(fields& all_fields, int value, std::set<std::string>& fields_store){
    for (auto& field: all_fields){
        limits limit1 = field.second.first;
        limits limit2 = field.second.second;
        if (value >= limit1.first and value <= limit1.second){
            fields_store.insert(field.first);
            continue;
        }
        else if (value >= limit2.first and value <= limit2.second) {
            fields_store.insert(field.first);
        }
    }
}

void get_intersecting_fields(fields& all_fields, int value, std::unordered_map<size_t, std::set<std::string>>& fields_store, size_t index){
    std::set<std::string> new_set;
    add_matching_fields(all_fields, value, new_set);
    std::set<std::string> intersection;
    std::set_intersection(new_set.begin(), new_set.end(), fields_store[index].begin(), fields_store[index].end(), std::inserter(intersection, intersection.end()));
    fields_store[index] = intersection;
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
    std::vector<ticket> tickets;
    for (std::string line; std::getline(input_file, line);){
        std::vector<std::string> to_parse;
        boost::split(to_parse, line, boost::is_any_of(","), boost::token_compress_on);
        ticket new_ticket;
        bool valid = true;
        for (auto& c: to_parse){
            int value = std::stoi(c);
            if (not is_valid_field(all_fields, value)) {
                valid=false;
                break;
            }
            new_ticket.push_back(value);
        }
        if (valid){
            tickets.push_back(new_ticket);
        }
    }
    input_file.close();

    size_t n_fields = all_fields.size();
    std::unordered_map<size_t, std::set<std::string>> fields_matching;
    for (size_t i=0; i<n_fields; i++){
        add_matching_fields(all_fields, my_ticket[i], fields_matching[i]);
    }
    for (ticket c: tickets){
        for (size_t i=0; i<n_fields; i++){
            get_intersecting_fields(all_fields, c[i], fields_matching, i);
        }
    }
    std::unordered_map<size_t, std::string> fields_assign;
    while (fields_assign.size() < n_fields){
        std::string field_to_assign;
        size_t index;
        for (auto& c: fields_matching){
            if (c.second.size() == 1){
                index = c.first;
                field_to_assign = *c.second.begin();
                break;
            }
        }
        fields_assign[index] = field_to_assign;
        fields_matching.erase(fields_matching.find(index));
        for (auto& c: fields_matching){
            if (contains(c.second, field_to_assign)){
                c.second.erase(c.second.find(field_to_assign));
            }
        }
    }
    long long total = 1;
    for (auto& c: fields_assign){
        auto pos = c.second.find("departure");
        if (pos != std::string::npos){
            total *= my_ticket[c.first];
        }
    }
    std::cout << total <<std::endl;
}
