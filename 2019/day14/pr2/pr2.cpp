#include <map>
#include <queue>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

typedef std::map<std::string, std::pair<long long, std::vector<std::pair<long long, std::string>>>> reaction_map;
bool is_terminal(const std::string& reactant, std::unordered_set<std::string>& terminals){
    return terminals.find(reactant) != terminals.end();
}

long long find_amount_react(long long quant_desired, long long quant_to_react){
    if (quant_desired <= quant_to_react) return 1;
    if (quant_desired % quant_to_react == 0) return quant_desired/quant_to_react;
    else return 1+quant_desired/quant_to_react;
}

bool has_waste(long long quant_desired, long long quant_to_react){
    if (quant_desired <= quant_to_react) return true;
    if (quant_desired % quant_to_react == 0) return false;
    else return true;
}

long long find_minimal_ore(reaction_map& reactions, std::unordered_set<std::string>& terminals, long long fuel_level){
    std::unordered_map<std::string, long long> waste;
    std::queue<std::pair<long long, std::string>> to_explore;
    to_explore.push(std::make_pair(fuel_level, "FUEL"));
    long long ore = 0;
    while (not to_explore.empty()){
        auto element = to_explore.front();
        to_explore.pop();
        long long quantity = element.first;
        std::string node = element.second;
        long long quant_product = find_amount_react(quantity, reactions[node].first);
        bool wasted = has_waste(quantity, reactions[node].first);
        if (wasted){
            if (waste[node]+(quant_product-1)*reactions[node].first >= quantity){
                waste[node] -= quantity-((quant_product-1)*reactions[node].first);
                quant_product--;
            }
            else{
                waste[node] += quant_product*reactions[node].first-quantity;
            }
        }
        for (auto& reactant: reactions[node].second){
            if (is_terminal(reactant.second, terminals)){
                long long needed = quant_product*reactant.first;
                long long quant_ore = find_amount_react(needed, reactions[reactant.second].first);
                bool wasted_ore = has_waste(needed, reactions[reactant.second].first);
                if (wasted_ore){
                    if (waste[reactant.second]+(quant_ore-1)*reactions[reactant.second].first >= needed){
                        waste[reactant.second] -= needed-((quant_ore-1)*reactions[reactant.second].first);
                        quant_ore--;
                    }
                    else{
                        waste[reactant.second] += quant_ore*reactions[reactant.second].first-needed;
                    }
                }
                ore += quant_ore*reactions[reactant.second].second[0].first;
            }
            else{
                to_explore.push(std::make_pair(quant_product*reactant.first, reactant.second));
            }
        }
    }
    return ore;
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    reaction_map reactions;
    std::unordered_set<std::string> terminals;
    for (std::string line; std::getline(input_file, line);){
        std::string reactant, product;
        auto pos = line.find(" => ");
        reactant = line.substr(0, pos);
        product = line.substr(pos+4);
        std::vector<std::string> parse;
        boost::split(parse, product, boost::is_any_of(" ,"), boost::token_compress_on);
        product = parse[1];
        long long product_n = std::stoi(parse[0]);
        reactions[product] = std::make_pair(product_n, std::vector<std::pair<long long, std::string>>());
        parse.clear();
        boost::split(parse, reactant, boost::is_any_of(" ,"), boost::token_compress_on);
        for (size_t i = 0; i < parse.size(); i+= 2){
            reactions[product].second.push_back(std::make_pair(std::stoi(parse[i]), parse[i+1]));
        }
        if (parse.size() == 2 and parse[1] == "ORE"){
            terminals.insert(product);
        }
    }
    long long max_ore = 1000000000000;
    long long left = 2, right = 82892753;
    long long fuel_level = (left+right)/2;
    while (left <= right){
        fuel_level = (left+right)/2;
        long long needed_ore = find_minimal_ore(reactions, terminals, fuel_level);
        if (needed_ore > max_ore){
            right = fuel_level-1;
        }
        else if (needed_ore < max_ore){
            left = fuel_level+1;
        }
    }
    while (find_minimal_ore(reactions, terminals, fuel_level) >= max_ore) fuel_level--;
    std::cout << "Maximum fuel is " << fuel_level << " with " << find_minimal_ore(reactions, terminals, fuel_level) << std::endl;
    std::cout << "Maximum fuel is " << fuel_level+1 << " with " << find_minimal_ore(reactions, terminals, fuel_level+1) << std::endl;
}
