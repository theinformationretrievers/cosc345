// dictionary.cpp
#include "dictionary.h"
#include <filesystem>
#include <iostream>
#include <stdexcept>
#ifdef _WIN32
#include <Windows.h>
#endif

Dictionary::Dictionary(const std::string &db_path)
{
    int rc = sqlite3_open(db_path.c_str(), &db);
    if (rc)
    {
        throw std::runtime_error("Can't open database: " + std::string(sqlite3_errmsg(db)));
    }

    // Temporarily add indexes for testing
    char *errMsg = 0;

    // Example: Create an index on the `word` column of the `english_words` table
    const char *createIndexSQL = "CREATE INDEX IF NOT EXISTS idx_english_word ON english_words(word)";
    rc = sqlite3_exec(db, createIndexSQL, 0, 0, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Error creating index: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    createIndexSQL = "CREATE INDEX IF NOT EXISTS idx_mappings_word_id ON mappings(word_id)";
    rc = sqlite3_exec(db, createIndexSQL, 0, 0, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Error creating index: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    createIndexSQL = "CREATE INDEX IF NOT EXISTS idx_mappings_maori_id ON mappings(maori_id)";
    rc = sqlite3_exec(db, createIndexSQL, 0, 0, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Error creating index: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    // Add more indexes as needed...
}

Dictionary::~Dictionary()
{
    update_translations(translation_cache);
    sqlite3_close(db);
}

translation Dictionary::fetch_translations(const std::string &word, const std::string &pos)
{

    auto cached_translation = translation_cache.find(word);

    if (cached_translation != translation_cache.end())
    {
        cached_translation->second.encounter_rate++;
        translation_cache[word] = cached_translation->second;
        return cached_translation->second;
    }

    std::string query = "SELECT m.word AS maori_word, "
                        "e.encounter_rate, "
                        "e.word_preference, "
                        "e.learned "
                        "FROM mappings "
                        "JOIN english_words e ON e.word_id = mappings.word_id "
                        "JOIN maori_words m ON m.maori_id = mappings.maori_id "
                        "WHERE e.word = ?";

    if (!pos.empty())
    {
        query += "AND mappings.part_of_speech = ? ";
    }

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        throw std::runtime_error("Failed to prepare statement");
    }

    sqlite3_bind_text(stmt, 1, word.c_str(), -1, SQLITE_STATIC);
    if (!pos.empty())
    {
        sqlite3_bind_text(stmt, 2, pos.c_str(), -1, SQLITE_STATIC);
    }

    std::vector<translation> translations;

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        translation t;
        t.maori_word = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        t.encounter_rate = sqlite3_column_int(stmt, 1);
        t.word_preference = sqlite3_column_int(stmt, 2);
        t.learned = (sqlite3_column_int(stmt, 3) != 0);

        translations.push_back(t);
    }

    sqlite3_finalize(stmt);

    translation maori_translation;

    if (translations.size())
    {
        maori_translation = translations.front();
        maori_translation.encounter_rate++;
        translation_cache[word] = maori_translation;
    }
    else
    {
        maori_translation.maori_word = "!";
        translation_cache[word] = maori_translation;
    }

    return maori_translation;
}

translation Dictionary::translate(const std::string &word, const std::string &pos)
{
    return fetch_translations(word, pos);
}

void Dictionary::update_translations(std::unordered_map<std::string, translation> &translations)
{
    // Begin transaction
    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

    std::string update_word_query = "INSERT OR REPLACE INTO english_words (word, encounter_rate, word_preference, learned) "
                                    "VALUES (?, ?, ?, ?)";
    sqlite3_stmt *word_stmt;
    sqlite3_prepare_v2(db, update_word_query.c_str(), -1, &word_stmt, nullptr);

    std::string get_word_id_query = "SELECT word_id FROM english_words WHERE word = ?";
    sqlite3_stmt *get_word_id_stmt;
    sqlite3_prepare_v2(db, get_word_id_query.c_str(), -1, &get_word_id_stmt, nullptr);

    std::string update_maori_query = "INSERT OR REPLACE INTO maori_words (maori_id, word) VALUES (?, ?)";
    sqlite3_stmt *maori_stmt;
    sqlite3_prepare_v2(db, update_maori_query.c_str(), -1, &maori_stmt, nullptr);

    for (auto &trans : translations)
    {
        // Update or insert the English word
        sqlite3_bind_text(word_stmt, 1, trans.second.maori_word.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(word_stmt, 2, trans.second.encounter_rate);
        sqlite3_bind_int(word_stmt, 3, trans.second.word_preference);
        sqlite3_bind_int(word_stmt, 4, trans.second.learned);

        sqlite3_step(word_stmt);
        sqlite3_reset(word_stmt);

        // Get the ID of the English word
        sqlite3_bind_text(get_word_id_stmt, 1, trans.second.maori_word.c_str(), -1, SQLITE_STATIC);

        int word_id = -1;
        if (sqlite3_step(get_word_id_stmt) == SQLITE_ROW)
        {
            word_id = sqlite3_column_int(get_word_id_stmt, 0);
        }

        sqlite3_reset(get_word_id_stmt);

        if (word_id != -1)
        {
            // Update or insert the Maori word
            sqlite3_bind_int(maori_stmt, 1, word_id);
            sqlite3_bind_text(maori_stmt, 2, trans.second.maori_word.c_str(), -1, SQLITE_STATIC);

            sqlite3_step(maori_stmt);
            sqlite3_reset(maori_stmt);
        }
    }

    // Finalize statements
    sqlite3_finalize(word_stmt);
    sqlite3_finalize(get_word_id_stmt);
    sqlite3_finalize(maori_stmt);

    // Commit transaction
    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
}
