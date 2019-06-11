//
// Created by Victor Zhang on 2019/6/8.
//

#include <iostream>
#include "vanilla_option.h"

int main() {
    vanilla_option option(100, 0.05, 1.0, 100, 0.2);
    std::cout<<"Put price: "<< option.calc_put_price()<<" Call price: "<< option.calc_call_price()<<std::endl;
    return 0;
}