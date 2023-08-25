import csv
import json
import argparse


def normalize_counts(counts):
    """
    Normalizes word counts in a dictionary.

    Args:
        counts (dict): A dictionary containing word counts.

    Returns:
        dict: A dictionary containing normalized probabilities based on word counts.
    """
    total_count = sum(counts.values())
    normalized_counts = {word: count /
                         total_count for word, count in counts.items()}
    return normalized_counts


def handle_args():
    """
    Handles command-line arguments for custom output and input file paths.

    This function uses the argparse library to handle the --filepath and --input arguments,
    allowing users to specify custom paths for both the input CSV file and the output JSON file.
    If no input CSV file is provided, a default input path is used.

    Returns:
        tuple: A tuple containing the output file path and the input CSV file path.
    """
    parser = argparse.ArgumentParser(
        description="Input --filepath to describe custom output path, otherwise placed in clean_data.")
    parser.add_argument('--filepath', help='Path to the file')
    parser.add_argument('--input', help='Path to the input file')
    args = parser.parse_args()

    if args.input is None:
        csv_file = "../raw_data/unigram_freq.csv"
    else:
        csv_file = args.input

    return args.filepath, csv_file


if __name__ == "__main__":
    """
    Entry point for generating normalized word frequencies in JSON format.

    This block handles command-line arguments for customizing output file paths and processes
    a CSV file containing word counts. The word counts are normalized and saved in JSON format.
    """
    filepath, csv_file = handle_args()

    word_counts = {}

    # Gather word counts
    with open(csv_file, "r") as file:
        csv_reader = csv.DictReader(file)
        for row in csv_reader:
            word = row["word"]
            count = int(row["count"])
            word_counts[word] = count

    normalized_probabilities = normalize_counts(word_counts)

    if filepath is None:
        filename = "../clean_data/english_frequencies.json"
    else:
        filename = filepath

    with open(filename, "w") as file:
        json.dump(normalized_probabilities, file)
