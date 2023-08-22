#include <cstdlib> // for system
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

std::vector<std::string> interop(std::string filename)
{
    std::string command;

#if defined(_WIN32) || defined(_WIN64)
    // We are on Windows
    command += ".\\dist\\pos_tagger.exe ";
#elif defined(__linux__)
    // We are on Linux
    command += "./dist/pos_tagger_linux ";
#else
    std::cout << "Unsupported OS" << std::endl;
    return ; // Return with an error code or handle it accordingly
#endif

    command += filename;
    std::cout << "Executing command: " << command << std::endl;
    int result = system(command.c_str());

    if (result == 0) {
        std::cout << "Python binary executed successfully!" << std::endl;
    } else {
        std::cout << "Error executing Python binary." << std::endl;
    }
    std::vector<std::string> pos_tags;
    std::string tag;
    std::string pos_path = filename + ".pos";
    std::ifstream file(pos_path);  // Replace with your file path

    if (!file) {
        std::cerr << "Unable to open file." << std::endl;
        return ;  // Exit with error code
    }

    while (file >> tag) {
        pos_tags.push_back(tag);
    }

    file.close();

    // Print the vector to verify


    return pos_tags;
}
int main(int argc, char const *argv[])
{
    if (argc < 2) {
        std::cerr << "Please provide an argument to pass to the Python binary." << std::endl;
        return 1;
    }

    std::cout << "Calling the Python binary with argument: " << argv[1] << std::endl;

    // Build the command string to include the argument
    interop(argv[1]);
    return 0;
}