#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <translator.h>
// #include "dictionary.h"
#include <filesystem>
/**
 * @brief Test the POS tagging function on a valid file.
 *
 * This test case ensures that the `get_pos_tags` function
 * correctly processes a valid file and produces the expected output.
 */
TEST_CASE("TestPosTaggingOnValidFile", "[get_pos_tags]")
{
    // Get the POS tags for a known valid test file
    std::vector<std::string> hound_test_output = get_pos_tags("./pos_data/hound");

    // Vector to hold the expected results for the valid file
    std::vector<std::string> succeed_case;

    // Open the expected results file for the valid test case
    std::string pos_path = "./pos_data/hound.test";
    std::ifstream file(pos_path);
    std::string tag;

    if (!file) {
        std::cerr << "Unable to open the expected results file." << std::endl;
    }

    // Load expected results from file into the succeed_case vector
    while (file >> tag) {
        succeed_case.push_back(tag);
    }

    // Ensure the output matches the expected results for the valid file
    REQUIRE(hound_test_output == succeed_case);
}

/**
 * @brief Test the POS tagging function on an invalid file.
 *
 * This test case ensures that the `get_pos_tags` function
 * correctly handles an invalid file (e.g., a file that does not exist)
 * and produces the expected output (an empty vector).
 */
TEST_CASE("TestPosTaggingOnInvalidFile", "[get_pos_tags]")
{
    // Expected result for an invalid file should be an empty vector
    std::vector<std::string> fail_case = {};

    // Get the POS tags for an invalid file path
    std::vector<std::string> fail_case_output = get_pos_tags("bogus_file_path");

    // Ensure the output matches the expected result for the invalid file
    REQUIRE(fail_case_output == fail_case);
}

TEST_CASE("Test translate_and_replace", "[translate_and_replace]") {
    // Read expected output from file
    std::ifstream expectedFile("./test_data/translated.test");
    std::string expectedOutput((std::istreambuf_iterator<char>(expectedFile)), std::istreambuf_iterator<char>());

    // Prepare input for translate_and_replace
    std::ifstream inputFile("./test_data/small_test.txt");
    std::stringstream inputStream;
    inputStream << inputFile.rdbuf();

    // Get output from translate_and_replace
    std::string actualOutput = translate_and_replace(inputStream, 42);

    // Compare
    REQUIRE(expectedOutput == actualOutput);
}

TEST_CASE("Test Dictionary Database Connection", "[Dictionary]")
{
    // Setup: Create a test SQLite database
    const std::string valid_db_path = "./dict.sqlite";

    SECTION("Connect to valid database")
    {
        REQUIRE_NOTHROW(Dictionary(valid_db_path)); // Should not throw any exceptions
    }
}
TEST_CASE("Test Dictionary Database Connection Failure", "[Dictionary]") {
    SECTION("Connect to invalid database path") {
        // Create a directory with the same name to simulate an error
        const std::string invalid_db_path = "invalid_db_directory";
        
        // Ensure the directory doesn't exist before the test
        if (std::filesystem::exists(invalid_db_path)) {
            std::filesystem::remove_all(invalid_db_path);
        }

        std::filesystem::create_directory(invalid_db_path);

        // Use a scope guard for cleanup to ensure the directory is removed even if the test fails
        struct CleanupGuard {
            std::string path;
            ~CleanupGuard() {
                std::filesystem::remove_all(path);
            }
        } cleanup{invalid_db_path};

        REQUIRE_THROWS(Dictionary(invalid_db_path)); // Should throw an exception
    }
}