import requests
from bs4 import BeautifulSoup

# URL of the website you want to scrape

MAX_PAGE_NUMBER = 14

pos_tags = ['a', 'ad', 'arch', 'art', 'conj', 'def', 'fig', 'freq', 'ib', 'indef', 'int', 'inter', 'ln', 'mod', 'n', 'num', 'pass', 'pers', 'pl', 'pos', 'prep', 'pron', 'pt', 'qv', 'sing', 'sp spp', 'var', 'vi', 'vt', 'v.i', 'v.t', 'N', 'v']


output = {}

def main():
    
    with open('testscraper.txt', 'w', encoding='utf-8') as output_file:

        for page_number in range(1, MAX_PAGE_NUMBER+1):
            url = f'https://nzetc.victoria.ac.nz/tm/scholarly/tei-WillDict-t1-body-d1-d{page_number}.html'
            
            # Send an HTTP GET request to the URL
            response = requests.get(url)

            print(response.status_code)
            # Check if the request was successful
            if response.status_code == 200:
                # Parse the HTML content of the page
                soup = BeautifulSoup(response.content, 'html.parser')
                
                # Find elements with a specific class
                section_elements = soup.find_all(class_='section')
                
                for word in section_elements:
                    
                    if "tei-WillDict" in word['id']:
                        continue
                    
                    translation_section = word.find(class_="hang")
                    maori_words = retrieve_maori_words(translation_section)
                    if maori_words == None:
                        continue
                    
                    e_definitions = retrieve_english_definitions(word)
                    if e_definitions == None:
                        continue

                    
                    for word in maori_words:
                        output_file.write(word+'\n')
                        
                        for d in e_definitions:
                            if word in output:
                                output[word].append(d)
                            else:
                                output[word] = [d]
            else:
                print('Failed to retrieve the webpage')
        
        
        inverted_dict = {}

        for key, values in output.items():
            for value in values:
                if value in inverted_dict:
                    inverted_dict[value].append(key)
                else:
                    inverted_dict[value] = [key]
        
        for k,v in inverted_dict.items():
            print(k,v)

def retrieve_english_definitions(word):
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
    try:
        maori_word = translation_section.find_next(class_="foreign bold").get_text()
    except:
        return None
    
    if "‖" in maori_word or "(" in maori_word or "——" in maori_word:
        return None
    
    if maori_word[-1] in [":", ";"]:
        maori_word = maori_word[:-1]
        
    maori_word.strip()
    
    maori_word = maori_word.replace(". ",",")
    maori_word = maori_word.replace("=",",")
    maori_word = maori_word.replace(".—",",")
    
    if "," in maori_word:
        maori_words = maori_word.split(",")

    else:
        maori_words = [maori_word]
        
    for i, mword in enumerate(maori_words):
        maori_words[i] = mword.strip()
        
    for mword in maori_words:
        if mword in pos_tags:
            maori_words.remove(mword)
        if mword.isnumeric():
            maori_words.remove(mword)
        if len(mword) == 0:
            maori_words.remove(mword)
    
    if maori_words:
        return maori_words
    else:
        return None
    
    
    
if __name__ == "__main__":
    main()