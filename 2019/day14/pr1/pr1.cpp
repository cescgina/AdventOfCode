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

typedef std::map<std::string, std::pair<int, std::vector<std::pair<int, std::string>>>> reaction_map;
bool is_terminal(const std::string& reactant, std::unordered_set<std::string>& terminals){
    return terminals.find(reactant) != terminals.end();
}

int find_amount_react(int quant_desired, int quant_to_react){
    if (quant_desired <= quant_to_react) return 1;
    if (quant_desired % quant_to_react == 0) return quant_desired/quant_to_react;
    else return 1+quant_desired/quant_to_react;
}

bool has_waste(int quant_desired, int quant_to_react){
    if (quant_desired <= quant_to_react) return true;
    if (quant_desired % quant_to_react == 0) return false;
    else return true;
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
        int product_n = std::stoi(parse[0]);
        reactions[product] = std::make_pair(product_n, std::vector<std::pair<int, std::string>>());
        parse.clear();
        boost::split(parse, reactant, boost::is_any_of(" ,"), boost::token_compress_on);
        for (size_t i = 0; i < parse.size(); i+= 2){
            reactions[product].second.push_back(std::make_pair(std::stoi(parse[i]), parse[i+1]));
        }
        if (parse.size() == 2 and parse[1] == "ORE"){
            terminals.insert(product);
        }
    }
    std::unordered_map<std::string, int> waste;
    std::queue<std::pair<int, std::string>> to_explore;
    to_explore.push(std::make_pair(1, "FUEL"));
    int ore = 0;
    while (not to_explore.empty()){
        auto element = to_explore.front();
        to_explore.pop();
        int quantity = element.first;
        std::string node = element.second;
        int quant_product = find_amount_react(quantity, reactions[node].first);
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
                int needed = quant_product*reactant.first;
                int quant_ore = find_amount_react(needed, reactions[reactant.second].first);
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
    std::cout << "Total required ore is " << ore << std::endl;
}
