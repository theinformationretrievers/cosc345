import os
import re
import sys
import json

# Define the directory path where your .txt files are located
directory_path = "../books/top_ebooks"

book_item_dictionary = {}

# Loop over each file in the directory
for filename in os.listdir(directory_path):
    # Check if the file is a .txt file
    if filename.endswith(".txt"):
        # Construct the full path to the file
        file_path = os.path.join(directory_path, filename)

        # Read the file contents as a string
        with open(file_path, "r", encoding='utf-8') as file:
            book_title = ""
            author = "N/A"
            
            for line in file:
                line = line.strip()
                
                match = re.search(r'Title: (.+)', line)
                if match:
                    book_title = match.group(1)
                
                match = re.search(r'Author: (.+)', line)
                if match:
                    author = match.group(1)
                    break
            
                # Replace any disallowed characters with underscores
                cleaned_filename = "".join(c if c.isalnum() or c in ['_', '-', ' '] else '_' for c in book_title)
                
                # Remove leading and trailing spaces
                cleaned_filename = cleaned_filename.strip()

            if book_title != "" and author != "":
                
                book_item_dictionary[cleaned_filename] = {
                    "Title" : book_title, 
                    "Author" : author, 
                    "Path" : f"../../Data/books/top_ebooks/{os.path.basename(file_path)}"
                }
                # with open(f"../books/book_object_items/{cleaned_filename}.txt", 'w') as file2:
                #     file2.write(book_title + "\n")
                #     file2.write(author + "\n")
                #     file2.write(f"../../Data/books/top_ebooks/{os.path.basename(file_path)}")
            else:
                print("Error finding file title: " + file_path)
                sys.exit(0)

with open("../books/book_object_items/books.json", "w") as json_file:
    json.dump(book_item_dictionary, json_file)
