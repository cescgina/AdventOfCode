#include <cmath>
#include <iostream>


int main(){
    int r0 = 0, r1 = 1, r4=10551275;
    while (r1 < sqrt(r4)){
        if (r4%r1 == 0){
            r0 += r1;
            r0 += r4/r1;
        }
        r1++;
    }
    std::cout << "Value of register 0 is " << r0 << std::endl;
}
