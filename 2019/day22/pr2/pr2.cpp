#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

const int64_t m = 119315717514047, n=101741582076661, needle=2020;

// https://en.wikipedia.org/wiki/Exponentiation_by_squaring
const auto combine = [](auto f, int64_t unit, int64_t a, int64_t b) {
  for (int64_t r = unit;; b >>= 1, a = f(a, a)) {
    if (!b) return r;
    if (b & 1) r = f(r, a);
  }
};

static int64_t add(int64_t a, int64_t b) { return (m + (a + b) % m) % m; }  // +  (mod m)
static int64_t mul(int64_t a, int64_t b) { return combine(add, 0, a, b); }  // *  (mod m)
static int64_t pow(int64_t a, int64_t b) { return combine(mul, 1, a, b); }  // ** (mod m)

bool DEBUG = false;
typedef std::vector<std::string> instrution;
typedef std::vector<std::pair<int, long long>> instrutions;

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    instrutions all_instructions;
    std::ifstream input_file;
    input_file.open(file_name);
    for (std::string line; std::getline(input_file, line);){
        instrution to_parse;
        boost::split(to_parse, line, boost::is_any_of(" "), boost::token_compress_on);
        if (to_parse[0] == "cut"){
            all_instructions.push_back(std::make_pair(2, std::stoll(to_parse[1])));
        }
        else{
            if (to_parse[1] == "into"){
                all_instructions.push_back(std::make_pair(0, 0));
            }
            else{
                all_instructions.push_back(std::make_pair(1, std::stoll(to_parse[3])));
            }
        }
    }

    int64_t k = 1, b = 0, x;
    for (auto& instruction_info: all_instructions){
        switch (instruction_info.first) {
            case 0:
                // reverse
                k = add(0, -k);
                b = add(-1, -b);
                break;
            case 1:
                // deal with increment
                k = mul(k, instruction_info.second);
                b = mul(b, instruction_info.second);
                break;
            case 2:
                // cut
                b = add(b, -instruction_info.second);
                break;
        }
    }
    x = mul(b, pow(k-1, m-2));
    std::cout << add(mul(add(x, needle), pow(pow(k, m-2), n)), -x) << std::endl;
}
