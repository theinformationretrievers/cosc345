#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <iostream>
#include <translator.h>
#include <fstream>

TEST_CASE("Pos-tagging", "[get_pos_tags]")
{
    std::vector<std::string> output = get_pos_tags("./pos_data/hound");
    std::vector<std::string> test_case;
    std::string pos_path = "./pos_data/hound.test";
    std::ifstream file(pos_path);  // Replace with your file path
    std::string tag;
    std::cout << "test";
    if (!file) {
        std::cerr << "Unable to open file." << std::endl;
    }

    while (file >> tag) {
        test_case.push_back(tag);
    }


    REQUIRE(output == test_case);
    // REQUIRE(factorial(2) == 2);
    // REQUIRE(factorial(3) == 6);
    // REQUIRE(factorial(10) == 3'628'800);
}