import csv
import json

def normalize_counts(counts):
    total_count = sum(counts.values())
    normalized_counts = {word: count / total_count for word, count in counts.items()}
    return normalized_counts

csv_file = "../raw_data/unigram_freq.csv"
word_counts = {}

with open(csv_file, "r") as file:
    csv_reader = csv.DictReader(file)
    for row in csv_reader:
        word = row["word"]
        count = int(row["count"])
        word_counts[word] = count

normalized_probabilities = normalize_counts(word_counts)

filename = "../clean_data/english_frequencies.json"
with open(filename, "w") as file:
    json.dump(normalized_probabilities, file)

