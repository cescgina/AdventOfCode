#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

typedef std::vector<long long> registers;
typedef std::vector<std::string> to_parse;

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::string line; 
    std::getline(input_file, line);
    to_parse before_reg;
    registers instructions;
    boost::split(before_reg, line, boost::is_any_of(", "), boost::token_compress_on);
    std::transform(before_reg.begin(), before_reg.end(), std::back_inserter(instructions), [](std::string& c) -> int { return std::stoi(c);});
    for (int ii = 0; ii < 100; ii++){
        for (int jj = 0; jj < 100; jj++){
            registers instructions_cp = instructions;
            instructions_cp[1] = ii;
            instructions_cp[2] = jj;
            int i = 0;
            int n = instructions_cp.size();
            while (i < n){
                if (instructions_cp[i] == 99){
                    break;
                }
                else if (instructions_cp[i] == 1){
                    instructions_cp[instructions_cp[i+3]] = instructions_cp[instructions_cp[i+1]]+instructions_cp[instructions_cp[i+2]];
                }
                else if (instructions_cp[i] == 2){
                    instructions_cp[instructions_cp[i+3]] = instructions_cp[instructions_cp[i+1]]*instructions_cp[instructions_cp[i+2]];
                }
                else{
                    std::cout << "Something wierd found " << i << " " << instructions_cp[i]<< std::endl;
                }
                i += 4;
            }
            if (instructions_cp[0] == 19690720){
                std::cout << "Value is " << 100*ii+jj << std::endl;
                return 0;
            }
        }

    }
}
