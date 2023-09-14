#include <fstream>
#include <iostream>
#include <sstream>
#include <translator.h>
int main(int argc, char const* argv[])
{
    std::ifstream inputFile("./test_data/test.txt");
    std::stringstream inputStream;
    inputStream << inputFile.rdbuf();

    std::string fileContent = translate_and_replace(inputStream, 42);

    std::ofstream outputFile("../test_data/translated.test");
    outputFile << fileContent;
    return 0;
}
