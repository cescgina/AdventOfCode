#include <deque>
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

std::ostream& operator<<(std::ostream& out, rule& obj){
    for (auto& el: obj.subrules){
        out << el << " ";
    }
    return out;
}

bool matches_rule(const std::string& msg, std::deque<std::string> rule_seq, size_t index, rule_description& rules_list){
    if (index == msg.size() or rule_seq.empty()){
        // if we run out of rules and message to check, msg is valid, otherwise
        // it is false
        return index == msg.size() and rule_seq.empty();
    }
    // process next sub-rule
    auto r = rules_list[rule_seq.front()];
    rule_seq.pop_front();
    if (r[0].is_terminal){
        if (msg[index] == r[0].subrules[0][0]){
            //if the sub-rule emits the correct character, keep exploring
            return matches_rule(msg, rule_seq, index+1, rules_list);
        }
        else{
            // if it's wrong, terminate search
            return false;
        }
    }
    bool any_match = false;
    for (auto& possible_rule: r){
        // push the new sub-rules to check to the stack and explore them
        std::deque<std::string> new_rules(rule_seq);
        new_rules.insert(new_rules.begin(), possible_rule.subrules.begin(), possible_rule.subrules.end());
        // if any of the combinations produce a valid path, message is valid
        any_match |= matches_rule(msg, new_rules, index, rules_list);
    }
    return any_match;
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    rule_description rule_list;
    std::ifstream input_file(file_name);
    std::string new_rule_8 = "8: 42 | 42 8";
    std::string new_rule_11 = "11: 42 31 | 42 11 31";
    for (std::string line; std::getline(input_file, line);){
        if (line.empty()) break;
        auto pos = line.find(":");
        std::string rule_name = line.substr(0, pos);
        if (rule_name == "8"){
            line = new_rule_8;
        }
        else if (rule_name == "11"){
            line = new_rule_11;
        }
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
        std::deque<std::string> start_rule = {"0"};
        bool is_valid_msg = matches_rule(line, start_rule, 0, rule_list);
        valid_msg += is_valid_msg;
        if (DEBUG and is_valid_msg){
            std::cout << "Is valid " << line << std::endl;
        }
    }
    input_file.close();
    std::cout << valid_msg << std::endl;
}
