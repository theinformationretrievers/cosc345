#include <cstdlib> // for system
#include <fstream>
#include <iostream>
#include <string>
#include <translator.h>
#include <vector>
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
