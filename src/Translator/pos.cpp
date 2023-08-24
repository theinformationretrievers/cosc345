#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <translator.h>
#include <vector>

/**
 * @brief Fetch POS (Part-Of-Speech) tags for a given text file.
 *
 * This function runs an external command (a compiled python POS tagger binary) on the input file and
 * returns the POS tags as a vector of strings. The external command is OS-dependent.
 *
 * @param filename The path to the input text file.
 * @return A vector containing the POS tags.
 * @note This function assumes that the tagger outputs a file with the ".pos" extension
 *       and that each tag in this file is separated by whitespace.
 */
std::vector<std::string> get_pos_tags(std::string filename)
{
    std::string command;

#if defined(_WIN32) || defined(_WIN64)
    // We are on Windows
    command += ".\\pos_data\\pos_tagger.exe ";
#elif defined(__linux__)
    // We are on Linux
    command += "./pos_data/pos_tagger_linux ";
#else
    std::cout << "Unsupported OS" << std::endl;
    return; // Return with an error code or handle it accordingly
#endif

    command += filename;
    int result = system(command.c_str());

    if (result != 0) {
        std::cerr << "Error executing Python binary." << std::endl;
    }
    std::vector<std::string> pos_tags;
    std::string tag;
    std::string pos_path = filename + ".pos";
    std::ifstream file(pos_path); // Replace with your file path

    if (!file) {
        std::cerr << "Unable to open file in pos: " << pos_path << std::endl;
        return pos_tags;
    }

    while (file >> tag) {
        pos_tags.push_back(tag);
    }
    file.close();
    return pos_tags;
}
