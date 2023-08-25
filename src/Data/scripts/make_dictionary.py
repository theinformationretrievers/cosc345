import requests
from bs4 import BeautifulSoup
import re
import json
import argparse


MAX_PAGE_NUMBER = 14
pos_tags = ['a', 'ad', 'arch', 'art', 'conj', 'def', 'fig', 'freq', 'ib', 'indef', 'int',
            'inter', 'ln', 'mod', 'n', 'num', 'pass', 'pers', 'pl', 'pos', 'prep', 'pron',
            'pt', 'qv', 'sing', 'sp spp', 'var', 'vi', 'vt', 'v.i', 'v.t', 'N', 'v']
pos_tags_punc = ['a.', 'ad.', 'arch.', 'art.', 'conj.', 'def.', 'fig.', 'freq.', 'ib.', 'indef.',
                 'int.', 'inter.', 'l.n.', 'mod.', 'n.', 'num.', 'pass.', 'pers.', 'pl.', 'pos.',
                 'prep.', 'pron.', 'pt.', 'q.v.', 'sing.', 'sp.', 'spp.', 'var.', 'v.i.', 'v.t.']
pos_mappings = {
    "a.": "JJ",
    "ad.": "RB",
    "arch.": None,
    "art.": "DT",
    "conj.": "CC",
    "def.": "DT",
    "fig.": None,
    "freq.": None,
    "ib.": None,
    "indef.": "DT",
    "int.": "UH",
    "inter.": "WP",
    "l.n.": "NN",
    "mod.": "MD",
    "n.": "NN",
    "num.": "CD",
    "pass.": None,
    "pers.": "PRP",
    "pl.": "NNS",
    "pos.": "POS",
    "prep.": "IN",
    "pron.": "PRP",
    "pt.": "VBN",
    "q.v.": None,
    "sing.": "NN",
    "sp.": "NN",
    "spp.": "NNS",
    "var.": None,
    "v.i.": "VB",
    "v.t.": "VB"
}


def main():
    """
    Main entry point for the script to gather and process Maori dictionary data.

    It retrieves Maori words and their translations from web pages, processes the data 
    based on user options (part of speech or no part of speech), and creates an English-to-Maori 
    dictionary. The resulting dictionary is stored in a JSON file.

    Returns:
        None
    """

    filepath, option = handle_args()
    maori_to_eng = {}

    for page_number in range(1, MAX_PAGE_NUMBER+1):
        url = f'https://nzetc.victoria.ac.nz/tm/scholarly/tei-WillDict-t1-body-d1-d{page_number}.html'

        # Send an HTTP GET request to the URL
        response = requests.get(url)
        print(response.status_code)

        if response.status_code == 200:
            soup = BeautifulSoup(response.content, 'html.parser')

            # Find elements with a specific class, denotes each new word
            section_elements = soup.find_all(class_='section')

            for word in section_elements:

                # Skip outlier class="section" at start of every page.
                if "tei-WillDict" in word['id']:
                    continue

                # retrieve maori words from page, class="hang" contains one maori word, with multiple spelling(s)/variation(s)
                translation_section = word.find(class_="hang")

                maori_words = retrieve_maori_words(translation_section)

                if option == 'pos':
                    pos_and_defs = retrieve_pos_definitions(word)
                else:
                    e_definitions = retrieve_english_definitions(word)

                # create dictionary
                if maori_words is not None:
                    for word in maori_words:

                        # definitions: e_definitions, or pos_and_defs for no POS tags and POS tags respectively.
                        if option == 'pos':
                            gather_definitions(
                                maori_to_eng, word, pos_and_defs)
                        else:
                            gather_definitions(
                                maori_to_eng, word, e_definitions)
        else:
            print('Failed to retrieve the webpage')

        english_to_maori = create_inverted_dictionary(maori_to_eng)

    with open(filepath, "w") as file:
        json.dump(english_to_maori, file)

    print(len(english_to_maori.keys()))


def handle_args():
    """
    Parses command-line arguments to customize the behavior of the script.

    This function uses the argparse library to handle command-line arguments for specifying
    a file path and an option (part of speech or no part of speech). If no file path is provided,
    the function sets a default file path based on the chosen option.

    Returns:
        tuple: A tuple containing the file path and the chosen option.
    """
    parser = argparse.ArgumentParser(
        description="""Input filepath to describe custom output, otherwise placed in clean_data.\n
                    Can choose between pos or no_pos\nUses:\n--filepath\n--option [pos, no_pos]""")

    parser.add_argument('--filepath', help='Path to the file')
    parser.add_argument('--option', choices=['pos', 'no_pos'],
                        default='pos', help='Choose option: pos or no_pos (default: pos)')

    args = parser.parse_args()

    if args.filepath is None:
        if args.option == 'pos':
            args.filepath = '../clean_data/english_to_maori_dictionary_pos.json'
        elif args.option == 'no_pos':
            args.filepath = '../clean_data/english_to_maori_dictionary_no_pos.json'

    return args.filepath, args.option


def gather_definitions(maori_to_eng, word, definitions):
    """
    Gathers and stores definitions for Maori words in a dictionary.

    Args:
        maori_to_eng (dict): A dictionary mapping Maori words to their English translations.
        word (str): The Maori word for which to gather definitions.
        definitions (list or None): A list of definitions for the given Maori word.

    Returns:
        bool: True if definitions were gathered and stored, False if no definitions were provided.
    """
    if definitions is not None:
        for d in definitions:
            if word in maori_to_eng:
                maori_to_eng[word].append(d)
            else:
                maori_to_eng[word] = [d]
        return True
    else:
        return False


def create_inverted_dictionary(dictionary):
    """
    Creates an inverted dictionary by swapping keys and values.

    Args:
        dictionary (dict): The dictionary to be inverted.

    Returns:
        dict: An inverted dictionary where values become keys and keys become values.
    """
    inverted_dict = {}

    for key, values in dictionary.items():
        for value in values:
            if value in inverted_dict:
                inverted_dict[value].append(key)
            else:
                inverted_dict[value] = [key]

    return inverted_dict


def retrieve_pos_definitions(word):
    """
    Retrieves part of speech and definitions from HTML tags for a given word.

    Args:
        word: BeautifulSoup object representing the word's HTML content.

    Returns:
        list or None: A list of part of speech and definition pairs, or None if not found.
    """
    p_tags = word.find_all("p")
    found_pos_tag = None
    pos_and_defs = []

    for p_tag in p_tags:

        # gather Pos tag for specific word
        for pos_tag in pos_tags_punc:
            if pos_tag in p_tag.get_text():
                found_pos_tag = pos_tag

        # convert POS tag from maori dictionary to Penn Treebank format.
        mapped_pos_tag = None
        if found_pos_tag:
            mapped_pos_tag = pos_mappings[found_pos_tag]

        # find corrolated definition
        definition = p_tag.find("i")
        if definition and mapped_pos_tag:
            pos_and_defs.append(mapped_pos_tag.upper() +
                                " | " + definition.get_text().lower())

    if len(pos_and_defs) > 0:
        return pos_and_defs
    else:
        return None


def retrieve_english_definitions(word):

    # find all html tags with a translation (itallics)
    # again, some garbage gets in as HTML is inconsistent. This is okay for english, it just won't get matched.
    i_tags = word.find_all("i")
    if not i_tags:
        return None

    for tag in i_tags:
        if "," in tag.get_text():
            tag_texts = tag.get_text().split(",")
            for i, text in enumerate(tag_texts):
                tag_texts[i] = text.strip()
        else:
            tag_texts = [tag.get_text()]

        return tag_texts


def retrieve_maori_words(translation_section):
    """
    Retrieves Maori words from the translation section of HTML content.

    This function searches for foreign bold elements within the translation section and
    extracts Maori words with emphasis. It performs cleaning and splitting to handle
    multiple spellings or variations. It also filters out unwanted characters and ensures
    valid part of speech tags.

    Args:
        translation_section: BeautifulSoup object representing the translation section's HTML content.

    Returns:
        list or None: A list of cleaned Maori words, or None if no valid words were found.
    """

    # look for foreign bolds, aka maori words with emphasis.
    try:
        maori_word = translation_section.find_next(
            class_="foreign bold").get_text()
    except AttributeError:
        return None

    # ignore any troubling translations
    bad_chars = ["‖", "(", "——"]
    if any(char in maori_word for char in bad_chars):
        return None

    # trim garbage, html is inconsitent on what is allowed in tags.
    if maori_word[-1] in [":", ";"]:
        maori_word = maori_word[:-1]

    # clean and split into multiple words if multiple spellings.
    maori_word = maori_word.strip()
    maori_word = re.sub(r'\. |=|\.\—', ',', maori_word)
    maori_words = [word.strip()
                   for word in maori_word.split(",") if word.strip()]

    # checking for part of speach tags ensures no garbage gets let through (again, inconsistent content inside html tags.)
    maori_words = [
        word for word in maori_words if word not in pos_tags and not word.isnumeric()]

    # return maori word(s)
    if maori_words:
        return maori_words
    else:
        return None


if __name__ == "__main__":
    main()
