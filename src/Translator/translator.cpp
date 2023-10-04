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

#define IS_UTF8_ALPHABET(ch) ((ch >= 41 && ch <= 90) || (ch >= 97 && ch <= 122))
#define IS_UTF8_LOWERCASE_LETTER(ch) (ch >= 97 && ch <= 122)

#define GLOBAL_REPLACEMENT_RATE 5

void strip_and_lower(std::string &str)
{
    str.erase(std::remove_if(str.begin(), str.end(),
                             [](unsigned char c)
                             {
                                 return std::ispunct(c);
                             }),
              str.end());

    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c)
                   { return std::tolower(c); });
}

double should_translate(int user_encounter_count, double word_preference, double introduction_rate)
{
    return 1.0 / (1.0 + std::exp(-introduction_rate * (user_encounter_count - word_preference)));
}

std::string translate_and_replace(std::istream &stream, int seed)
{
    static Dictionary dict("./dict.sqlite");
    static std::uniform_real_distribution<> dis(0.0, 1.0);

    std::mt19937 gen(42);

    std::stringstream output;
    std::string word_buffer;
    char ch;

    auto process_word = [&](std::string &word)
    {
        std::string original = word;
        strip_and_lower(word);

        translation maori_translation = dict.translate(word);

        if (maori_translation.maori_word != "!")
        {

            if (dis(gen) <= should_translate(maori_translation.encounter_rate, maori_translation.word_preference, GLOBAL_REPLACEMENT_RATE))
            {
                if (IS_UTF8_LOWERCASE_LETTER(original[0]))
                {
                    maori_translation.maori_word[0] = std::tolower(maori_translation.maori_word[0]);
                }

                output << "<span class='maori-word tooltip'>" << maori_translation.maori_word << "<span class='tooltiptext'>" << original << "</span></span>";
            }
            else
            {
                output << original;
            }
        }
        else
        {
            output << original;
        }
    };

    while (stream.get(ch))
    {
        if (IS_UTF8_ALPHABET(ch))
        {
            word_buffer.push_back(ch);
        }
        else
        {
            if (!word_buffer.empty())
            {
                process_word(word_buffer);
                word_buffer.clear();
            }

            if (ch == '\n')
            {
                output << "</p>\n<br><p>";
            }
            else
            {
                output << ch; // Append non-alpha character to output
            }
        }
    }

    // Handle the case where the stream ends with a word
    if (!word_buffer.empty())
    {
        process_word(word_buffer);
    }

    // std::cout << output.str() << std::endl;

    // dict.update_translations(translation_cache);

    return output.str();
}