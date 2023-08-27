#include <string>
#include <vector>
#include "sqlite3.h" 
/**
 * @class Dictionary
 * @brief Provides functions for translating English words to Maori.
 *
 * This class interfaces with an SQLite database to check for translations
 * of English words and fetch them if they exist.
 */
class Dictionary {
private:
    sqlite3* db; /**< SQLite3 database connection pointer. */

    /**
     * @brief Checks if a translation exists for a given word and optionally a part of speech.
     * @param word The English word to check.
     * @param pos The part of speech tag (optional).
     * @return `true` if the translation exists, `false` otherwise.
     */
    bool query_word(const std::string& word, const std::string& pos = "");

    /**
     * @brief Fetches translations of a given word and optionally for a specific part of speech.
     * @param word The English word to translate.
     * @param pos The part of speech tag (optional).
     * @return A list of translations in Maori.
     */
    std::vector<std::string> fetch_translations(const std::string& word, const std::string& pos = "");

public:
    /**
     * @brief Constructor that initializes the SQLite3 database connection.
     * @param db_path Path to the SQLite database file.
     */
    Dictionary(const std::string& db_path);

    /**
     * @brief Destructor that closes the SQLite3 database connection.
     */
    ~Dictionary();

    /**
     * @brief Checks if a translation can be made for a given word and optionally a part of speech.
     * @param word The English word to check.
     * @param pos The part of speech tag (optional).
     * @return `true` if a translation can be made, `false` otherwise.
     */
    bool can_translate(const std::string& word, const std::string& pos = "");

    /**
     * @brief Translates a given English word and optionally for a specific part of speech.
     * @param word The English word to translate.
     * @param pos The part of speech tag (optional).
     * @return A list of translations in Maori.
     */
    std::vector<std::string> translate(const std::string& word, const std::string& pos = "");
};
