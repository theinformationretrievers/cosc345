#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <fstream>
#include <iostream>
#include <translator.h>

TEST_CASE("Pos-tagging", "[get_pos_tags]")
{
    std::vector<std::string> hound_test_output = get_pos_tags("./pos_data/hound");
    std::vector<std::string> succeed_case;
    std::vector<std::string> fail_case = {};
    std::vector<std::string> fail_case_output = get_pos_tags("bogus_file_path");

    std::string pos_path = "./pos_data/hound.test";
    std::ifstream file(pos_path); // Replace with your file path
    std::string tag;
    std::cout << "test";
    if (!file) {
        std::cerr << "Unable to open file." << std::endl;
    }

    while (file >> tag) {
        succeed_case.push_back(tag);
    }

    REQUIRE(hound_test_output == succeed_case);
    REQUIRE(fail_case_output == fail_case);
}