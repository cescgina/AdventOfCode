#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

bool DEBUG = false;

long long inverse(long long a, long long n){

    long long t = 0, newt = 1;
    long long r = n, newr = a;
    long long temp = 0;

    while (newr != 0){
        long long quotient = r /newr;
        temp = newt;
        newt = t - quotient*newt;
        t = temp;
        temp = newr;
        newr = r - quotient*newr;
        r = temp;
    }

    if (r > 1) {
        std::cerr << "a not invertible" << std::endl;
        return -1;
    }
    if (t < 0) t += n;
    return t;
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file(file_name);
    std::string line; 
    std::getline(input_file, line);
    std::getline(input_file, line);
    std::vector<std::string> to_parse;
    std::vector<int> buses;
    boost::split(to_parse, line, boost::is_any_of(","), boost::token_compress_on);
    int i = -1;
    std::map<int, int> offsets;
    long long N = 1;
    for (auto& bus: to_parse){
        i++;
        if (bus[0] == 'x') continue;
        buses.push_back(std::stoi(bus));
        offsets[buses.back()] = i;
        N *= buses.back();
    }
    long long time = 0;
    for (auto& element: offsets){
        // see https://brilliant.org/wiki/chinese-remainder-theorem/ for
        // explanation
        long long y = N/element.first;
        long long temp = ((element.first-element.second)% N)*(y%N);
        temp %= N;
        temp = (temp*(inverse(y, element.first) % N))%N;
        time = (time %N + temp %N)%N;
    }
    std::cout << time%N << std::endl;
}
