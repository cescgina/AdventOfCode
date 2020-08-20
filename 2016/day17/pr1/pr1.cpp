#include <set>
#include <map>
#include <queue>
#include <vector>
#include <fstream>
#include <utility>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <openssl/md5.h>

static const char hexDigits[17] = "0123456789abcdef";
typedef std::tuple<int, std::string, int, int> info;

void print_buffer(const std::vector<char>& buffer){
    for (auto x: buffer){
        std::cout << x;
    }
    std::cout << std::endl;
}

void calcMD5(const char* key, const size_t len, std::vector<char>& buffer){
    unsigned char digest[MD5_DIGEST_LENGTH];
    // Count digest
    MD5( (const unsigned char*)key, len, digest );
    // Convert the hash into a hex string form
    for(int i = 0; i < 2; i++ )
    {
        buffer[i*2]   = hexDigits[(digest[i] >> 4) & 0xF];
        buffer[i*2+1] = hexDigits[digest[i] & 0xF];
    }
}

int calc_dist(const int& x, const int& y){
    return std::abs(3-x)+std::abs(3-y);
}

int main(int argc, char** argv){
    std::string file_name, secret_key, shortest_path;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::getline(input_file, secret_key);
    input_file.close();
    std::vector<char> instructions_buffer = {' ', ' ', ' ', ' '};
    size_t len_key = secret_key.size(), min_len = 100000000;
    int x = 0, y = 0, dist = 0;
    auto cmp = [](info left, info right) {return std::get<0>(left) > std::get<0>(right);};
    std::priority_queue<info, std::vector<info>, decltype(cmp)> paths(cmp);
    paths.push(std::make_tuple(len_key, secret_key, x, y));
    info element;
    while (not paths.empty()){
        element = paths.top();
        paths.pop();
        dist = std::get<0>(element);
        secret_key = std::get<1>(element);
        x = std::get<2>(element);
        y = std::get<3>(element);
        calcMD5(secret_key.c_str(), secret_key.size(), instructions_buffer);
        if (x > 0 and instructions_buffer[0] > 'a'){
            paths.push(std::make_tuple(dist+1, secret_key+"U", x-1, y));
        }
        if (x < 3 and instructions_buffer[1] > 'a'){
            if (calc_dist(x+1, y) == 0 and (secret_key.size()+1-len_key) < min_len){
                min_len = secret_key.size()+1-len_key;
                shortest_path = secret_key+"D";
                std::cout << shortest_path.substr(len_key) <<std::endl;
                return 0;
            }
            paths.push(std::make_tuple(dist+1, secret_key+"D", x+1, y));
        }
        if (y > 0 and instructions_buffer[2] > 'a'){
            paths.push(std::make_tuple(dist+1, secret_key+"L", x, y-1));
        }
        if (y < 3 and instructions_buffer[3] > 'a'){
            if (calc_dist(x, y+1) == 0 and (secret_key.size()+1-len_key) < min_len){
                min_len = secret_key.size()+1-len_key;
                shortest_path = secret_key+"R";
                std::cout << shortest_path.substr(len_key) <<std::endl;
                return 0;
            }
            paths.push(std::make_tuple(dist+1, secret_key+"R", x, y+1));
        }
    }
}
