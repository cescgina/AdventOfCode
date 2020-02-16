#include <map>
#include <cctype>
#include <string>
#include <fstream>
#include <numeric>
#include <iostream>
#include <unistd.h>

typedef std::multimap<std::string,std::string> replacements;

void parse_command(const std::string& order, replacements& guide){
    std::string origin, dest;
    auto pos = order.find(" => ");
    origin = order.substr(0, pos);
    dest = order.substr(pos+4);
    guide.insert({origin, dest});
}

int main(int argc, char** argv){
    std::string file_name, command, molecule, origin_molecule="e";
    int ar=0, rn=0, y=0, total=0;
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
    for (size_t i=0; i<molecule.length()-1; i++){
        if (molecule.substr(i, 1) == "Y") y++;
        else if (molecule.substr(i, 2) == "Ar") ar++;
        else if (molecule.substr(i, 2) == "Rn") rn++;
        if (std::isupper(static_cast<unsigned char>(molecule[i]))) total++;
    }
    if (molecule.substr(molecule.length()-1, 1) == "Y") y++;
    std::cout << "Number of steps required is " << total-rn-ar-2*y-1 << std::endl;
}
