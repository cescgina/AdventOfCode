#include <set>
#include <iostream>
#include <fstream>
#include <utility>
#include <unistd.h>
#include <openssl/md5.h>

bool checkZeros(char* answer){
    for (int i=0; i<6; i++){
        if (answer[i] != '0') return false;
    }    
    return true;
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
    int suffix = 0;
    while (true){
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
        if (checkZeros(digest_str)) break;
        suffix++;
    }
    std::cout << "Answer is " << suffix << std::endl;
}
