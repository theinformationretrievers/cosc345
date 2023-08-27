#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <cctype>
#include <random>
#include <windows.h>
#include "dictionary.h"

#pragma execution_character_set( "utf-8" )

struct translation {
    std::string maori_word;        // The Maori word that will replace the original word.
    int original_word_length;      // Length of the original word.
    std::streampos start_position; // Starting position of the original word in the stream.
};



void strip_and_lower(std::string& str) {
    str.erase(std::remove_if(str.begin(), str.end(),
        [](unsigned char c) {
            return std::ispunct(c);
        }),
        str.end());

    std::transform(str.begin(), str.end(), str.begin(),
        [](unsigned char c) { return std::tolower(c); });
}


double should_translate(int user_encounter_count, double word_preference, double introduction_rate) {
    return 1.0 / (1.0 + std::exp(-introduction_rate * (user_encounter_count - word_preference)));
}

std::vector<translation> translate(std::istream& stream) {
    
    Dictionary dict("C:\\Users\\illic\\source\\repos\\Diglot\\out\\build\\x64-Debug\\dict.sqlite");
    std::unordered_map<std::string, int> encounter_map;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
   

    std::string word;
    std::vector<translation> translations;
   
    while (stream >> word) {

        int word_length = word.length();
        std::string original = word;
        strip_and_lower(word);

        if (dict.can_translate(word) ){

            int encountered = encounter_map[word]++;

            if (dis(gen) <= should_translate(encountered, 1, 5)) {

                std::streampos start_pos = stream.tellg() - static_cast<std::streampos>(word_length);

                translation t;
                t.maori_word = dict.translate(word).front();
                t.original_word_length = word_length;
                t.start_position = start_pos;

                translations.push_back(t);
            }
       }

    }

    stream.clear();
    stream.seekg(0);
    return translations;
}
