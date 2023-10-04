import sqlite3
import json

with open('/home/matt/Documents/UniStuff/COSC345/project/src/Data/clean_data/english_to_maori_dictionary_pos.json', 'r') as f:
    data = json.load(f)
    
conn = sqlite3.connect('dict.sqlite')
cursor = conn.cursor()

# Drop tables if they exist
cursor.execute('DROP TABLE IF EXISTS mappings')
cursor.execute('DROP TABLE IF EXISTS maori_words')
cursor.execute('DROP TABLE IF EXISTS english_words')

# Create tables
cursor.execute('''
CREATE TABLE english_words (
    word_id INTEGER PRIMARY KEY,
    word TEXT UNIQUE,
    encounter_rate INTEGER DEFAULT 0,
    word_preference INTEGER DEFAULT 1,
    learned BOOLEAN DEFAULT 0
)''')

cursor.execute('''
CREATE TABLE maori_words (
    maori_id INTEGER PRIMARY KEY,
    word TEXT UNIQUE
)''')

cursor.execute('''
CREATE TABLE mappings (
    mapping_id INTEGER PRIMARY KEY,
    word_id INTEGER,
    maori_id INTEGER,
    part_of_speech TEXT,
    FOREIGN KEY(word_id) REFERENCES english_words(word_id),
    FOREIGN KEY(maori_id) REFERENCES maori_words(maori_id)
)''')

# Insert data
for key, maori_word_list in data.items():
    pos, english_word = key.split('|')
    pos = pos.strip()
    english_word = english_word.strip()

    cursor.execute('INSERT OR IGNORE INTO english_words (word) VALUES (?)', (english_word,))

    word_id = cursor.execute('SELECT word_id FROM english_words WHERE word = ?', (english_word,)).fetchone()[0]

    for maori_word in maori_word_list:
        cursor.execute('INSERT OR IGNORE INTO maori_words (word) VALUES (?)', (maori_word,))
        maori_id = cursor.execute('SELECT maori_id FROM maori_words WHERE word = ?', (maori_word,)).fetchone()[0]
        cursor.execute('INSERT INTO mappings (word_id, maori_id, part_of_speech) VALUES (?, ?, ?)', (word_id, maori_id, pos))

conn.commit()
conn.close()
