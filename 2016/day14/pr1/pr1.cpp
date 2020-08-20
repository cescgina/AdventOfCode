#include <set>
#include <map>
#include <vector>
#include <fstream>
#include <utility>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <openssl/md5.h>

typedef char patterns;

void print_repeats(const std::map<int, std::pair<patterns, patterns>>::iterator& it){
    std::cout << it->first << " " << it->second.first << " # " << it->second.second << std::endl;
}

char checkTriple(char* answer){
    for (int i=0; i<2*MD5_DIGEST_LENGTH-2; i++){
        if (answer[i] == answer[i+1] and answer[i] == answer[i+2]) return answer[i];
    }    
    return ' ';
}

char checkFive(char* answer){
    std::set<char> found;
    bool all_equal = false;
    for (int i=0; i<2*MD5_DIGEST_LENGTH-4; i++){
        all_equal = true;
        for (int j=1; j<5; j++){
            all_equal &= (answer[i] == answer[i+j]);
        }
        if (all_equal) return answer[i];
    }    
    return ' ';
}

int main(int argc, char** argv){
    static const char hexDigits[17] = "0123456789abcdef";
    unsigned char digest[MD5_DIGEST_LENGTH];
    char digest_str[2*MD5_DIGEST_LENGTH+1];
    std::string file_name, secret_key, complete_key;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::getline(input_file, secret_key);
    input_file.close();
    int indexes = 0, suffix = 0;
    char triple, fives;
    std::map<int, std::pair<patterns, patterns>> codes;
    while (indexes < 64){
        complete_key = secret_key + std::to_string(suffix); 
        // Count digest
        MD5( (const unsigned char*)complete_key.c_str(), complete_key.length(), digest );
        // Convert the hash into a hex string form
        for(int i = 0; i < MD5_DIGEST_LENGTH; i++ )
        {
            digest_str[i*2]   = hexDigits[(digest[i] >> 4) & 0xF];
            digest_str[i*2+1] = hexDigits[digest[i] & 0xF];
        }
        digest_str[MD5_DIGEST_LENGTH*2] = '\0';
        triple = checkTriple(digest_str);
        if (triple != ' '){
            fives = checkFive(digest_str);
            codes[suffix] = std::make_pair(triple, fives);
        }
        if (suffix % 1500 == 0 and suffix > 0){
            std::vector<int> to_erase;
            for (auto it=codes.begin(); it != codes.end(); it++){
                for (auto it2=it; it2 != codes.end(); it2++){
                    if (it->first == it2->first) continue;
                    if ((it2->first - it->first) >= 1000){
                        to_erase.push_back(it->first);
                        break;
                    }
                    triple = it->second.first;
                    fives = it2->second.second;
                    if (triple == fives and triple != ' ') {
                        indexes++;
                        if (indexes == 64) {
                            std::cout << "Final index is " << it->first << std::endl; 
                            return 0;
                        }
                        to_erase.push_back(it->first);
                        break;
                    }
                }
            }
            for (auto it=to_erase.begin(); it != to_erase.end(); it++){
                codes.erase(codes.find(*it));
            }
        };
        suffix++;
    }
}
