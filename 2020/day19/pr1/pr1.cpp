#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unistd.h>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

bool DEBUG = false;
typedef std::vector<std::string> rules;
struct rule{
    rules subrules;
    bool is_terminal;
};
typedef std::unordered_map<std::string, std::vector<rule>> rule_description;

std::pair<bool, size_t> matches_rule(const std::string& msg, const std::string rule, size_t index, rule_description& rules_list){
    if (DEBUG){
        std::cout << msg << " " << rule << " -- " << index << std::endl;
    }
    if (rules_list[rule][0].is_terminal){
        if (DEBUG){
            std::cout << msg << " " << msg[index] << " " << rule << " " << index << " " << rules_list[rule][0].subrules[0][0] << std::endl;
        }
        return std::make_pair(msg[index] == rules_list[rule][0].subrules[0][0], index+1);
    }
    size_t orig_index = index;
    for (auto& possible_rule: rules_list[rule]){
        index = orig_index;
        bool all_match = true;
        for (auto& rule_to_check: possible_rule.subrules){
            auto res = matches_rule(msg, rule_to_check, index, rules_list);
            all_match &= res.first;
            if (not all_match) break;
            index = res.second;
        }
        if (all_match) return std::make_pair(true, index);
    }
    return std::make_pair(false, index);
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    rule_description rule_list;
    std::ifstream input_file(file_name);
    for (std::string line; std::getline(input_file, line);){
        if (line.empty()) break;
        auto pos = line.find(":");
        std::string rule_name = line.substr(0, pos);
        std::string rules_desc = line.substr(pos+2);
        rules rules_to_parse;
        boost::split(rules_to_parse, rules_desc, boost::is_any_of("|"), boost::token_compress_on);
        for (std::string ruleset: rules_to_parse){
            rules rules_children;
            boost::trim(ruleset);
            boost::split(rules_children, ruleset, boost::is_any_of(" "), boost::token_compress_on);
            bool terminal = (rules_children.size() == 1 and rules_children[0][0] == '"');
            if (terminal){
                rules_children[0] = rules_children[0].substr(1, 1);
            }
            rule_list[rule_name].push_back(rule{rules_children, terminal});
        }
    }
    if (DEBUG){
        for (auto& el: rule_list){
            std::cout << el.first << ": ";
            for (auto& ruless: el.second){
                for (auto& rule_to_check: ruless.subrules){
                    std::cout << rule_to_check << " ";
                }
                std::cout << " | ";
            }
            std::cout << std::endl;
        }
    }
    int valid_msg = 0;
    for (std::string line; std::getline(input_file, line);){
        if (line.empty()) break;
        auto ret = matches_rule(line, "0", 0, rule_list);
        valid_msg += (ret.first and ret.second == line.size());
        if (DEBUG and ret.first and ret.second == line.size()){
            std::cout << "Is valid " << line << std::endl;
        }
    }
    input_file.close();
    std::cout << valid_msg << std::endl;
}
