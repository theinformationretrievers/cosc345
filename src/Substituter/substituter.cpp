#include <iostream>
#include "substituter.h"

void test_func(void)
{
    std::cout << "Substituter extern test declaration" << std::endl;
}

int main(void)
{
    std::cout << "Substituter c file" << std::endl;
    return EXIT_SUCCESS;
}