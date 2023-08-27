#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <cctype>
#include <random>
#include <windows.h>
#include "sqlite3.h"
#include "dictionary.h"

#pragma execution_character_set( "utf-8" )

std::unordered_map<std::string, int> test_user_map;


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

void translate(std::istream& stream) {
    std::string word;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    std::string newContent;

    while (stream >> word) {

        int word_length = word.length();
        std::string original = word;
        strip_and_lower(word);

   //     if (words.find(word) != words.end()) {

   //         int encountered = test_user_map[word]++;

   //         if (dis(gen) <= should_translate(encountered, 1, 5)) {
   //             std::streampos start = stream.tellg() - static_cast<std::streampos>(word_length);
   //             //std::cout << start << ",";
   ///*             std::cout << "Word: " << word << std::endl;
   //             std::cout << "Maori: " << words[word] << std::endl;
   //             std::cout << "Start: " << start << std::endl;
   //             std::cout << "length: " << word_length << std::endl << std::endl;*/

   //             original = words[word];
   //         }


   //     }

        newContent += original + " ";

    }

    std::cout << newContent << std::endl;
}


int callback(void* notUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }
    std::cout << std::endl;
    return 0;
}


int main() {

    SetConsoleOutputCP(CP_UTF8);

    Dictionary dict("dict.sqlite");

    std::string input_word;
    std::cout << "Enter English word to search: ";
    std::getline(std::cin, input_word);

    if (dict.can_translate(input_word)) {
        std::vector<std::string> translations = dict.translate(input_word);
        for (const auto& translation : translations) {
            std::cout << translation << std::endl;
        }
    }
    else {
        std::cerr << "No translations found for: " << input_word << std::endl;
    }

    return 0;
}



