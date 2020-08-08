#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

template <typename A, typename B>
std::multimap<B, A> flip_map(std::map<A,B> & src) {
    std::multimap<B,A> dst;
    std::map<int, std::vector<std::string>> alph;
    std::vector<std::string> letters;
    for(typename std::map<A, B>::const_iterator it = src.begin(); it != src.end(); ++it)
        if (alph.find(it->second) != alph.end()){
            alph[it->second].push_back(it->first);
        }
        else{
            alph[it->second] = {it->first};
        }
    
    for(std::map<int, std::vector<std::string>>::const_reverse_iterator it = alph.rbegin(); it != alph.rend(); ++it){
        letters = it->second;
        std::sort(letters.begin(), letters.end());
        for (auto l=letters.rbegin(); l != letters.rend(); l++){
            dst.insert(std::pair<B, A>(it->first, *l));
        }
    }
    return dst;
}

int main(int argc, char** argv){
    std::string file_name, letter, id_check, check;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::vector<std::string> parsed_line;
    std::map<std::string,int> room_name;
    std::multimap<int, std::string> sorted_chars;
    int sum = 0, checksum, last_freq = 0, index = 0;
    for (std::string line; std::getline(input_file, line);){
        room_name.clear();
        boost::split(parsed_line, line, boost::is_any_of("-"), boost::token_compress_on);
        for (size_t i=0; i<parsed_line.size()-1; i++){
            for (size_t j=0; j<parsed_line[i].size(); j++){
                letter = parsed_line[i][j];
                if (room_name.find(letter) != room_name.end()){
                    room_name[letter] += 1;
                }
                else{
                    room_name[letter] = 1;
                }
            }
        }
        sorted_chars = flip_map(room_name);
        id_check = parsed_line.back();
        id_check.pop_back();
        auto pos = id_check.find("[");
        checksum = std::stoi(id_check.substr(0, pos));
        check = id_check.substr(pos+1);
        last_freq = 0;
        index = 0;
        bool decoy = false;
        for(std::multimap<int, std::string>::const_reverse_iterator it = sorted_chars.rbegin(); it != sorted_chars.rend(); ++it){
            if (index == 5) break;
            if (it->second != check.substr(index, 1)) {
                decoy = true;
                break;
            }
            if (it->first == last_freq and it->second > check.substr(index, 1)){
                decoy = true;
                break;
            }
            last_freq = it->first;
            index++;
        }
        if (decoy) continue;
        sum += checksum;
    }
    std::cout << "Total is " << sum << std::endl;
}
