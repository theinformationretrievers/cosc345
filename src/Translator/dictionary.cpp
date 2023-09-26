// dictionary.cpp
#include "dictionary.h"
#include <filesystem>
#include <iostream>
#include <stdexcept>
#ifdef _WIN32
#include <Windows.h>
#endif

Dictionary::Dictionary(const std::string& db_path) {
    int rc = sqlite3_open(db_path.c_str(), &db);
    if (rc) {
        throw std::runtime_error("Can't open database: " + std::string(sqlite3_errmsg(db)));
    }

    // Temporarily add indexes for testing
    char* errMsg = 0;

    // Example: Create an index on the `word` column of the `english_words` table
    const char* createIndexSQL = "CREATE INDEX IF NOT EXISTS idx_english_word ON english_words(word)";
    rc = sqlite3_exec(db, createIndexSQL, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Error creating index: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    createIndexSQL = "CREATE INDEX IF NOT EXISTS idx_mappings_word_id ON mappings(word_id)";
    rc = sqlite3_exec(db, createIndexSQL, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Error creating index: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    createIndexSQL = "CREATE INDEX IF NOT EXISTS idx_mappings_maori_id ON mappings(maori_id)";
    rc = sqlite3_exec(db, createIndexSQL, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Error creating index: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    // Add more indexes as needed...
}


// Dictionary::Dictionary(const std::string& db_path)
// {
//     if (!std::filesystem::exists(db_path)) {
//         throw std::runtime_error("Database file does not exist at the specified path: " + db_path);
//     }

//     // Open an in-memory database
//     int rc = sqlite3_open(":memory:", &db);
//     if (rc) {
//         throw std::runtime_error("Can't open in-memory database: " + std::string(sqlite3_errmsg(db)));
//     }

//     // Attach the disk-based database
//     std::string attachSQL = "ATTACH DATABASE '" + db_path + "' AS diskDB";
//     rc = sqlite3_exec(db, attachSQL.c_str(), 0, 0, 0);
//     if (rc != SQLITE_OK) {
//         throw std::runtime_error("Can't attach disk-based database: " + std::string(sqlite3_errmsg(db)));
//     }

//     // Increase cache size for the session
//     sqlite3_exec(db, "PRAGMA cache_size=10000;", 0, 0, 0);

//     // Begin a transaction
//     sqlite3_exec(db, "BEGIN TRANSACTION;", 0, 0, 0);

//     // Copy data from disk to memory (assuming tables are 'mappings', 'english_words', and 'maori_words')
//     std::vector<std::string> tables = { "mappings", "english_words", "maori_words" };
//     for (const auto& table : tables) {
//         std::string copySQL = "CREATE TABLE " + table + " AS SELECT * FROM diskDB." + table;
//         rc = sqlite3_exec(db, copySQL.c_str(), 0, 0, 0);
//         if (rc != SQLITE_OK) {
//             // If there's an error, rollback the transaction to avoid partial data in memory
//             sqlite3_exec(db, "ROLLBACK;", 0, 0, 0);
//             throw std::runtime_error("Can't copy table " + table + ": " + std::string(sqlite3_errmsg(db)));
//         }
//     }

//     // Commit the transaction
//     sqlite3_exec(db, "COMMIT;", 0, 0, 0);
// }


Dictionary::~Dictionary()
{
    sqlite3_close(db);
}

bool Dictionary::query_word(const std::string& word, const std::string& pos)
{
    std::string query = "SELECT 1 FROM mappings "
                        "JOIN english_words e ON e.word_id = mappings.word_id "
                        "WHERE e.word = ? ";
    if (!pos.empty()) {
        query += "AND mappings.part_of_speech = ? ";
    }
    query += "LIMIT 1";

    sqlite3_stmt* stmt;
    try {
        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error("Failed to prepare statement");
        }

        sqlite3_bind_text(stmt, 1, word.c_str(), -1, SQLITE_STATIC);
        if (!pos.empty()) {
            sqlite3_bind_text(stmt, 2, pos.c_str(), -1, SQLITE_STATIC);
        }

        bool exists = sqlite3_step(stmt) == SQLITE_ROW;

        sqlite3_finalize(stmt);
        return exists;
    } catch (const std::exception& e) {
        std::string errorMessage = "Exception: " + std::string(e.what()) + "\n";
        // OutputDebugStringA(errorMessage.c_str());

        if (stmt) {
            const char* sqliteErrorMessage = sqlite3_errmsg(db);
            // if (sqliteErrorMessage) {
            //     OutputDebugStringA(("SQLite Error: " + std::string(sqliteErrorMessage) + "\n").c_str());
            // }
        }

        // Check if the database connection is still valid
        // if (sqlite3_db_handle(stmt) == nullptr) {
        //     OutputDebugStringA("Database connection is invalid.\n");
        // }

        return false;
    }
}

std::vector<std::string> Dictionary::fetch_translations(const std::string& word, const std::string& pos)
{
    std::string query = "SELECT m.word FROM mappings "
                        "JOIN english_words e ON e.word_id = mappings.word_id "
                        "JOIN maori_words m ON m.maori_id = mappings.maori_id "
                        "WHERE e.word = ? ";
    if (!pos.empty()) {
        query += "AND mappings.part_of_speech = ? ";
    }

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare statement");
    }

    sqlite3_bind_text(stmt, 1, word.c_str(), -1, SQLITE_STATIC);
    if (!pos.empty()) {
        sqlite3_bind_text(stmt, 2, pos.c_str(), -1, SQLITE_STATIC);
    }

    std::vector<std::string> translations;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        translations.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    }

    sqlite3_finalize(stmt);
    return translations;
}

bool Dictionary::can_translate(const std::string& word, const std::string& pos)
{
    return query_word(word, pos);
}

std::vector<std::string> Dictionary::translate(const std::string& word, const std::string& pos)
{
    return fetch_translations(word, pos);
}
