/**
 * @file main.cpp
 * @brief Translates and replaces content from a test file.
 *
 * This program reads content from a test file, performs a translation and replacement operation
 * on the content, and then writes the modified content to a new test file.
 */

#include <fstream>
#include <sstream>
#include <iostream>
#include <translator.h>
#pragma execution_character_set("utf-8")


/**
 * @brief Main function to handle file translation and replacement.
 *
 * The function performs the following steps:
 * 1. Reads content from the file "../test_data/small_test.txt".
 * 2. Translates and replaces the content using the translate_and_replace function.
 * 3. Prints the translated content to the console.
 * 4. Writes the translated content to a new file "../test_data/translated.test".
 * @return Returns 0 upon successful execution.
 */
int main(void)
{
    // std::string in_path = "../test_data/small_test.txt";
    // std::string out_path = "../test_data/translated.test";
    // #ifdef _WIN32
    // in_path = "../../../test_data/small_test.txt";
    // out_path = "../../../test_data/translated.test";
    // #endif
    // std::ifstream inputFile(in_path);
    // std::stringstream inputStream;
    // inputStream << inputFile.rdbuf();

    // std::string fileContent = translate_and_replace(inputStream, 42);
    // std::cout << fileContent << std::endl;
    // std::ofstream outputFile(out_path);
    // outputFile << fileContent;
    return 0;
}
