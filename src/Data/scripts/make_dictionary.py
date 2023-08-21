import requests
from bs4 import BeautifulSoup
import sys
import re
import json


MAX_PAGE_NUMBER = 14
pos_tags = ['a', 'ad', 'arch', 'art', 'conj', 'def', 'fig', 'freq', 'ib', 'indef', 'int', 'inter', 'ln', 'mod', 'n', 'num', 'pass', 'pers', 'pl', 'pos', 'prep', 'pron', 'pt', 'qv', 'sing', 'sp spp', 'var', 'vi', 'vt', 'v.i', 'v.t', 'N', 'v']
pos_tags_punc = ['a.', 'ad.', 'arch.', 'art.', 'conj.', 'def.', 'fig.', 'freq.', 'ib.', 'indef.', 'int.', 'inter.', 'l.n.', 'mod.', 'n.', 'num.', 'pass.', 'pers.', 'pl.', 'pos.', 'prep.', 'pron.', 'pt.', 'q.v.', 'sing.', 'sp.,', 'spp.', 'var.', 'v.i.', 'v.t.']


def main():
    maori_to_eng = {}
    
    with open('check_output.txt', 'w', encoding='utf-8') as output_file:

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
                    
                    #Skip outlier class="section" at start of every page.
                    if "tei-WillDict" in word['id']:
                        continue
                    
                    # retrieve maori words from page, class="hang" contains one maori word, with multiple spelling(s)/variation(s)
                    translation_section = word.find(class_="hang")
                    
                    maori_words = retrieve_maori_words(translation_section)
                    e_definitions = retrieve_english_definitions(word)
                    pos_and_defs = retrieve_pos_definitions(word)

                    # create dictionary
                    if maori_words is not None:
                        for word in maori_words:
                            
                            # definitions: e_definitions, or pos_and_defs for no POS tags and POS tags respectively.
                            gather_definitions(maori_to_eng, word, pos_and_defs)
            else:
                print('Failed to retrieve the webpage')
        

        english_to_maori = create_inverted_dictionary(maori_to_eng)
        
        # write to file for checking.
        for k,v in english_to_maori.items():
            output_file.write(k + " " + str(v) + "\n")
            
    filename = "../clean_data/english_to_maori_dictionary_pos.json"
    with open(filename, "w") as file:
        json.dump(english_to_maori, file)
        
    print(len(english_to_maori.keys()))


def gather_definitions(maori_to_eng, word, definitions):
    if definitions is not None:
        for d in definitions:
            if word in maori_to_eng:
                maori_to_eng[word].append(d.lower())
            else:
                maori_to_eng[word] = [d.lower()]
        return True
    else:
        return False


def create_inverted_dictionary(dictionary):
    inverted_dict = {}

    for key, values in dictionary.items():
        for value in values:
            if value in inverted_dict:
                inverted_dict[value].append(key)
            else:
                inverted_dict[value] = [key]
    
    return inverted_dict


def retrieve_pos_definitions(word):
    p_tags = word.find_all("p")
    found_pos_tag = None
    pos_and_defs = []
    
    for p_tag in p_tags:
        
        for pos_tag in pos_tags_punc:
            if pos_tag in p_tag.get_text():
                found_pos_tag = pos_tag
                
        definition = p_tag.find("i")
        if definition and found_pos_tag:
            pos_and_defs.append(found_pos_tag + " | " + definition.get_text())
            # pos_and_defs.append([found_pos_tag, definition.get_text()])

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
    # look for foreign bolds, aka maori words with emphasis.
    try:
        maori_word = translation_section.find_next(class_="foreign bold").get_text()
    except:
        return None
    
    # ignore any troubling translations
    bad_chars = ["‖", "(", "——"]
    if any(char in maori_word for char in bad_chars):
        return None
    
    # trim garbage, html is inconsitent on what is allowed in tags.
    if maori_word[-1] in [":", ";"]:
        maori_word = maori_word[:-1]
        
    #clean and split into multiple words if multiple spellings.
    maori_word = maori_word.strip()
    maori_word = re.sub(r'\. |=|\.\—', ',', maori_word)
    maori_words = [word.strip() for word in maori_word.split(",") if word.strip()]
    
    # checking for part of speach tags ensures no garbage gets let through (again, inconsistent content inside html tags.)
    maori_words = [word for word in maori_words if word not in pos_tags and not word.isnumeric()]
    
    # return maori word(s)
    if maori_words:
        return maori_words
    else:
        return None
    
    
if __name__ == "__main__":
    main()