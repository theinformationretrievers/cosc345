#include <iostream>
#include "substituter.h"

void test_func()
{
    std::cout << "Substituter extern test declaration" << std::endl;
}
uint32_t factorial2(uint32_t number) {
    return number <= 1 ? number : factorial2(number - 1) * number;
}