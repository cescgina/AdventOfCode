#include <map>
#include <set>
#include <string>
#include <fstream>
#include <numeric>
#include <iostream>
#include <unistd.h>

typedef std::multimap<std::string,std::string> replacements;
typedef std::set<std::string> molecules;

void parse_command(const std::string& order, replacements& guide){
    std::string origin, dest;
    auto pos = order.find(" => ");
    origin = order.substr(0, pos);
    dest = order.substr(pos+4);
    guide.insert({origin, dest});
}

int find_replacements(const std::string& mol, replacements& guide){
    molecules final_mols;
    for (size_t i=0; i<mol.length();i++){
        if (guide.find(mol.substr(i, 1)) != guide.end()){
            auto range = guide.equal_range(mol.substr(i, 1));
            for (auto it = range.first; it != range.second; ++it){
                std::string copy_mol(mol);
                final_mols.insert(copy_mol.replace(i, 1, it->second));
            }
            continue;
        }
        else if (guide.find(mol.substr(i, 2)) != guide.end()){
            auto range = guide.equal_range(mol.substr(i, 2));
            for (auto it = range.first; it != range.second; ++it){
                std::string copy_mol(mol);
                final_mols.insert(copy_mol.replace(i, 2, it->second));
            }
        }
    }
    return final_mols.size();
}


int main(int argc, char** argv){
    std::string file_name, command, molecule;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    replacements transformations;
    std::ifstream input_file;
    input_file.open(file_name);
    while (std::getline(input_file, command)){
        if (command.length() == 0) break;
        parse_command(command, transformations);
    }
    std::getline(input_file, molecule);
    input_file.close();
    int n_rep = find_replacements(molecule, transformations);
    std::cout << "Number of unique molecules is " << n_rep << std::endl;
}
