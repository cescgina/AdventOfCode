#include <set>
#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <numeric>
#include <iostream>
#include <unordered_map>
#include <unistd.h>

typedef std::unordered_map<std::string, short unsigned int> variables;
typedef std::list<std::string> command_queue;
const std::set<std::string> operations({"AND", "LSHIFT", "RSHIFT", "OR", "NOT"});

bool isNumber(const std::string& str_chk){
    return str_chk.find_first_not_of("0123456789") == str_chk.npos;
}

void parse_command(const std::string& order, variables& values, command_queue& command_list){
    std::string op, output, action, token;
    std::string::size_type n, n_prev = 0;
    std::vector<std::string> operands;
    n = order.find("->");
    action = order.substr(0, n);
    output = order.substr(n+3);
    std::cout << order << std::endl;
    n = action.find(" ");
    while (n != action.npos){
        token = action.substr(n_prev, n-n_prev);
        n_prev = n+1;
        if (operations.find(token) != operations.end()){
            op = token;
        }
        else{
            operands.push_back(token); 
            if (values.find(token) == values.end()){
                if (isNumber(token)){
                    std::cout << token << std::endl;
                    values[token] = std::stoi(token);
                }
                else{
                    command_list.push_back(order);
                    return;
                }
            }
        }
        n = action.find(" ", n_prev);
    } 
    if (op == "NOT"){
        values[output] = ~values[operands[0]];
    }
    else if (op == "AND"){
        values[output] = values[operands[0]] & values[operands[1]];
    }
    else if (op == "OR"){
        values[output] = values[operands[0]] | values[operands[1]];
    }
    else if (op == "LSHIFT"){
        values[output] = values[operands[0]] << values[operands[1]];
    }
    else if (op == "RSHIFT"){
        values[output] = values[operands[0]] >> values[operands[1]];
    }
    else{
        values[output] = values[operands[0]];
    }
}

int main(int argc, char** argv){
    std::string file_name, command;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    variables variable_values; 
    command_queue commands;
    input_file.open(file_name);
    while (std::getline(input_file, command)){
        // populate command list
        commands.push_back(command);
    }
    input_file.close();
    // iterate over command list
    while (!commands.empty()){
        command = commands.front();
        commands.pop_front();
        parse_command(command, variable_values, commands);
    }
    for (auto it=variable_values.begin(); it != variable_values.end(); it++){
        std::cout << it->first << ": " << it->second << std::endl;
    }
}
