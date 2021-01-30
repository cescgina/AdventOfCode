#include <map>
#include <set>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

bool DEBUG = false;

typedef std::unordered_map<std::string, int> counts;
typedef std::vector<std::string> listing;
typedef std::unordered_set<std::string> compounds;
typedef std::unordered_map<std::string, compounds> matching;
typedef std::map<std::string, std::string> dangerous_map;

std::ostream& operator<<(std::ostream& out, dangerous_map& input_compounds){
    for (auto& el: input_compounds){
        out << el.second << ",";
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, compounds& input_compounds){
    for (auto& el: input_compounds){
        out << el << " ";
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, matching& input_matching){
    for (auto& el: input_matching){
        out << el.first << ": " << el.second << std::endl;
    }
    return out;
}

template<typename T>
bool contains(T& haystack, std::string needle){
    return haystack.find(needle) != haystack.end();
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    counts ingredient_count;
    compounds all_ingredients;
    matching alergens_to_ingr;
    std::ifstream input_file(file_name);
    for (std::string line; std::getline(input_file, line);){
        auto pos = line.find("(");
        std::string ingredients_str = line.substr(0, pos-1);
        std::string alergends_str = line.substr(pos+10, line.size()-pos-10-1);
        listing found_ingredients;
        boost::split(found_ingredients, ingredients_str, boost::is_any_of(" "), boost::token_compress_on);
        compounds curr_ingr;
        for (auto& ingr: found_ingredients){
            ingredient_count[ingr]++;
            all_ingredients.insert(ingr);
            curr_ingr.insert(ingr);
        }
        listing found_alergens;
        boost::split(found_alergens, alergends_str, boost::is_any_of(", "), boost::token_compress_on);
        for (auto& alerg: found_alergens){
            compounds common;
            if (contains(alergens_to_ingr, alerg)){
                for (auto& ingr: found_ingredients){
                    if (contains(alergens_to_ingr[alerg], ingr)) {
                        common.insert(ingr);
                    }
                }
                alergens_to_ingr[alerg] = common;
            }
            else{
                alergens_to_ingr[alerg] = curr_ingr;
            }
        }
    }
    input_file.close();
    for (auto& alerg_ingr: alergens_to_ingr){
        for (auto& ingr: alerg_ingr.second){
            if (contains(all_ingredients, ingr)) all_ingredients.erase(all_ingredients.find(ingr));
        }
    }
    if (DEBUG){
        std::cout << alergens_to_ingr << std::endl;
    }
    dangerous_map dangerous;
    while (not alergens_to_ingr.empty()){
        std::string alergen_to_rm;
        std::string ingr_to_rm;
        for (auto& alerg_ingr: alergens_to_ingr){
            if (alerg_ingr.second.size() == 1){
                alergen_to_rm = alerg_ingr.first;
                ingr_to_rm = *alerg_ingr.second.begin();
                dangerous[alergen_to_rm] = ingr_to_rm;
                break;
            }
        }
        alergens_to_ingr.erase(alergens_to_ingr.find(alergen_to_rm));
        for (auto& alerg_ingr: alergens_to_ingr){
            if (contains(alerg_ingr.second, ingr_to_rm)){
                alerg_ingr.second.erase(alerg_ingr.second.find(ingr_to_rm));
            }
        }
    } 
    std::cout << dangerous << std::endl;
}
