#include <vector>
#include <stack>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

bool DEBUG = false;

bool is_operation(char symbol){
    return symbol == '+' or symbol == '*';
}
void apply_operation(long long int& op1, long long int op2, char op_sym){
    if (op_sym == '+'){
        op1 += op2;
    }
    else{
        op1 *= op2;
    }
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file(file_name);
    long long int total_result = 0;
    for (std::string line; std::getline(input_file, line);){
        std::deque<char> operations;
        std::deque<long long int> operands, pending;
        long long int n_operands = 0;
        for (auto c: line){
            if (c == ' ') continue;
            else if (is_operation(c)){
                if (not pending.empty()){
                    n_operands++;
                }
                operations.push_back(c);
            }
            else if (c == '('){
                pending.push_back(n_operands);
                n_operands = 0;
            }
            else if (c == ')'){
                std::stack<long long int> parens;
                std::stack<char> parens_op;
                parens.push(operands.back());
                operands.pop_back();
                for (int i = 0; i < n_operands; i++){
                    parens.push(operands.back());
                    parens_op.push(operations.back());
                    operands.pop_back();
                    operations.pop_back();
                }
                long long int calculation = parens.top();
                parens.pop();
                while (n_operands > 0){
                    apply_operation(calculation, parens.top(), parens_op.top());
                    parens.pop();
                    parens_op.pop();
                    n_operands--;
                }
                operands.push_back(calculation);
                n_operands = pending.back();
                pending.pop_back();
            }
            else{
                operands.push_back(c-'0');
            }
        }
        long long int calculation = operands.front();
        operands.pop_front();
        while (not operations.empty() > 0){
            apply_operation(calculation, operands.front(), operations.front());
            operations.pop_front();
            operands.pop_front();
            n_operands--;
        }
        total_result += calculation;
        if (DEBUG){
            std::cout << line << " " << calculation << std::endl;
        }
    }
    input_file.close();
    std::cout << total_result << std::endl;
}
