#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <unordered_map>
#ifdef _WIN32
#include <windows.h>
#endif
#include "dictionary.h"

#pragma execution_character_set("utf-8")

struct translation {
    std::string maori_word; // The Maori word that will replace the original word.
    int original_word_length; // Length of the original word.
    std::streampos start_position; // Starting position of the original word in the stream.
};

void strip_and_lower(std::string& str)
{
    str.erase(std::remove_if(str.begin(), str.end(),
                  [](unsigned char c) {
                      return std::ispunct(c);
                  }),
        str.end());

    std::transform(str.begin(), str.end(), str.begin(),
        [](unsigned char c) { return std::tolower(c); });
}

double should_translate(int user_encounter_count, double word_preference, double introduction_rate)
{
    return 1.0 / (1.0 + std::exp(-introduction_rate * (user_encounter_count - word_preference)));
}

std::vector<translation> translate(std::istream& stream)
{

    Dictionary dict("./dict.sqlite");

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

        if (dict.can_translate(word)) {

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

std::string translate_and_replace(std::istream& stream, int seed)
{
    Dictionary dict("./dict.sqlite");

    std::unordered_map<std::string, int> encounter_map;
    std::unordered_map<std::string, std::string> translation_cache;

    // std::random_device rd;
    std::mt19937 gen(42);
    std::uniform_real_distribution<> dis(0.0, 1.0);

    std::stringstream output;
    std::string word_buffer;
    char ch;

    // output << "<pre>";
    output << "<p>";


    auto process_word = [&](std::string& word) {
        std::string original = word;
        strip_and_lower(word);

        std::string maori_translation;
        auto cached_translation = translation_cache.find(word);

        if (cached_translation != translation_cache.end()) {
            maori_translation = cached_translation->second;
        }
        else {
            if (dict.can_translate(word)) {
                maori_translation = dict.translate(word).front();
                translation_cache[word] = maori_translation;
            }
            else {
                maori_translation = "<notranslation>";
                translation_cache[word] = maori_translation;
            }
        }

        if (maori_translation != "<notranslation>") {
            int encountered = encounter_map[word]++;
            if (dis(gen) <= should_translate(encountered, 1, 5)) {
                output << "<span class=\"maori-word tooltip\">" << maori_translation << "<span class=\"tooltiptext\">" << original << "</span></span>";
            }
            else {
                output << original;
            }
        }
        else {
            output << original;
        }
        };

    while (stream.get(ch)) {
        if ((ch >= 41 && ch <= 90) || (ch >= 97 && ch <= 122)) {
            word_buffer.push_back(ch);
        }
        else {
            if (!word_buffer.empty()) {
                process_word(word_buffer);
                word_buffer.clear();
            }

            if (ch == '\n') {
                output << "</p>\n<br><p>";
            }
            else {
                output << ch; // Append non-alpha character to output
            }
        }
    }

    // Handle the case where the stream ends with a word
    if (!word_buffer.empty()) {
        process_word(word_buffer);
    }


    // output << "</pre>";
    output << "</p>";

    return output.str();
}