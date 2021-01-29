#include <vector>
#include <stack>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

bool DEBUG = false;

std::ostream& operator<<(std::ostream& out, std::deque<long long int> numbers){
    for (size_t i = 0; i < numbers.size(); i++){
        out << numbers[i] << " ";
    }
    return out;
}

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

long long int resolve_operations(std::deque<long long int>& parens, std::deque<char>& parens_op){
    size_t n_operands = parens_op.size();
    parens.push_back(parens.front());
    parens.pop_front();
    while (n_operands > 0){
        if (parens_op.front() == '+'){
            parens_op.pop_front();
            long long temp = parens.back();
            parens.pop_back();
            parens.push_back(temp+parens.front());
            parens.pop_front();
        }
        else{
            parens.push_back(parens.front());
            parens.pop_front();
            parens_op.push_back(parens_op.front());
            parens_op.pop_front();
        }
        n_operands--;
    }
    long long int calculation = parens.front();
    parens.pop_front();
    while (not parens_op.empty() > 0){
        apply_operation(calculation, parens.front(), parens_op.front());
        parens.pop_front();
        parens_op.pop_front();
    }
    return calculation;
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
                std::deque<long long int> parens;
                std::deque<char> parens_op;
                parens.push_front(operands.back());
                operands.pop_back();
                for (int i = 0; i < n_operands; i++){
                    parens.push_front(operands.back());
                    parens_op.push_front(operations.back());
                    operands.pop_back();
                    operations.pop_back();
                }
                long long int calculation = resolve_operations(parens, parens_op);

                operands.push_back(calculation);
                n_operands = pending.back();
                pending.pop_back();
            }
            else{
                operands.push_back(c-'0');
            }
        }
        long long calculation = resolve_operations(operands, operations);
        total_result += calculation;
        if (DEBUG){
            std::cout << line << " " << calculation << std::endl;
        }
    }
    input_file.close();
    std::cout << total_result << std::endl;
}
