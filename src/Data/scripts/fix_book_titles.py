import os
import re

# Define the directory path where your .txt files are located
directory_path = "../books/top_ebooks"

# Loop over each file in the directory
for filename in os.listdir(directory_path):
    # Check if the file is a .txt file
    if filename.endswith(".txt"):
        # Construct the full path to the file
        file_path = os.path.join(directory_path, filename)

        # Read the file contents as a string
        with open(file_path, "r") as file:
            file_contents = file.read()
            for line in file_contents:
                match = re.search(r'Title: ')
            print("-" * 30)  # Separator for readability
