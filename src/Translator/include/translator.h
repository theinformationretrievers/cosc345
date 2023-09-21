#include <vector>
#include <string>
#include "../dictionary.h"
std::vector<std::string> get_pos_tags(const std::string &filename);

std::string translate_and_replace(std::istream &stream, int);

std::string translate_and_replace_standard_seed(std::istream &stream);