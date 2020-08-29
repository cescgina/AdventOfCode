#include <map>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

typedef std::vector<std::vector<std::string>> queue;

int main(){
    int b = 7900+100000;
    int c = b+17000;
    int h = 0;
    while(b <= c){                                                                             
        for (int d = 2; d <= sqrt(b); d++){
            if (b % d == 0){
                h++;
                break;
            }                                                                              
        }
        b += 17;
    }           
    std::cout << h << std::endl;
}
