import csv
import json
import argparse

def normalize_counts(counts):
    total_count = sum(counts.values())
    normalized_counts = {word: count / total_count for word, count in counts.items()}
    return normalized_counts


parser = argparse.ArgumentParser(description="Input --filepath to describe custom output path, otherwise placed in clean_data.")
parser.add_argument('--filepath', help='Path to the file')
parser.add_argument('--input', help='Path to the input file')
args = parser.parse_args()

if args.input is None:
    csv_file = "../raw_data/unigram_freq.csv"
else:
    csv_file = args.input


word_counts = {}

with open(csv_file, "r") as file:
    csv_reader = csv.DictReader(file)
    for row in csv_reader:
        word = row["word"]
        count = int(row["count"])
        word_counts[word] = count

normalized_probabilities = normalize_counts(word_counts)

if args.filepath is None:
    filename = "../clean_data/english_frequencies.json"
else:
    filename = args.filepath

with open(filename, "w") as file:
    json.dump(normalized_probabilities, file)

