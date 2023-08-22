import argparse
import nltk
import json
import string
from nltk import pos_tag


#generator which breaks if any condition is false
def all_punctuation(s):
    return all(char in string.punctuation for char in s)

# Test



def main():
    parser = argparse.ArgumentParser(
        description="POS tag the content of a given file.")
    parser.add_argument(
        "filename", type=str, help="The name of the file containing the text to be POS tagged.")

    args = parser.parse_args()

    # Read the content of the file
    with open(args.filename, 'r') as file:
        content = file.read()

    split_on_space = content.split(" ")
    # POS tag the content
    tagged_content = pos_tag((content.split(" ")))
    output_string = ''
    index = 0
    for token in split_on_space:
        if all_punctuation(token):
            output_string += token
        else:
            _, tag = tagged_content[index]
            output_string += tag + ' '
            index += 1
    # Write the tagged content to a JSON file
    output_filename = args.filename.split('.')[0] + ".pos"
    with open(output_filename, 'w') as out:
        out.write((output_string))

    # print(f"POS tagged content written to {output_filename}")


if __name__ == "__main__":
    main()
