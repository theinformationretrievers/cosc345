#include <vector>
#include <string>
#include "../dictionary.h"
std::vector<std::string> get_pos_tags(const std::string& filename);

struct translation {
    std::string maori_word;        // The Maori word that will replace the original word.
    int original_word_length;      // Length of the original word.
    std::streampos start_position; // Starting position of the original word in the stream.
};

std::vector<translation> translate(std::istream& stream);

std::string translate_and_replace(std::istream& stream, int);


std::string translate_and_replace_standard_seed(std::istream& stream);