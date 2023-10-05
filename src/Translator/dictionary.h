#include <string>
#include <vector>
#include "sqlite3.h"
#include <unordered_map>

struct translation
{
    std::string maori_word;
    int encounter_rate;
    int word_preference;
    bool learned;
};

/**
 * @class Dictionary
 * @brief Provides functions for translating English words to Maori.
 *
 * This class interfaces with an SQLite database to check for translations
 * of English words and fetch them if they exist.
 */
class Dictionary
{
private:
    sqlite3 *db; /**< SQLite3 database connection pointer. */
    std::unordered_map<std::string, translation> translation_cache;

    /**
     * @brief Fetches translations of a given word and optionally for a specific part of speech.
     * @param word The English word to translate.
     * @param pos The part of speech tag (optional).
     * @return A list of translations in Maori.
     */
    translation fetch_translations(const std::string &word, const std::string &pos = "");

public:
    /**
     * @brief Constructor that initializes the SQLite3 database connection.
     * @param db_path Path to the SQLite database file.
     */
    Dictionary(const std::string &db_path);

    /**
     * @brief Destructor that closes the SQLite3 database connection.
     */
    ~Dictionary();

    /**
     * @brief Translates a given English word and optionally for a specific part of speech.
     * @param word The English word to translate.
     * @param pos The part of speech tag (optional).
     * @return A list of translations in Maori.
     */
    translation translate(const std::string &word, const std::string &pos = "");

    void update_blacklist_word(std::vector<std::string> &words);

    void update_translations(std::unordered_map<std::string, translation> &translations);
};
