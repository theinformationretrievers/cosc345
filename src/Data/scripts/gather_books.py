import os
import requests
from bs4 import BeautifulSoup
import concurrent.futures

base_url = "https://www.gutenberg.org/browse/scores/top#books-last1"
output_folder = "../books/top_ebooks/"  # Folder to save downloaded text files


def get_book_links(url):
    """
    Retrieves a set of unique book links from the provided URL.

    Args:
        url (str): The URL to retrieve book links from.

    Returns:
        set: A set of unique book links.
    """
    response = requests.get(url)
    soup = BeautifulSoup(response.content, "html.parser")
    links = soup.find_all("a", href=True)
    book_links = {link["href"]
                  for link in links if link["href"].startswith("/ebooks/")}
    return book_links


def get_redirected_url(url):
    """
    Retrieves the final redirected URL from the provided URL.

    Args:
        url (str): The URL to retrieve the redirected URL for.

    Returns:
        str: The final redirected URL.
    """
    response = requests.head(url, allow_redirects=True)
    return response.url


def scrape_book_link(book_link):
    """
    Downloads and saves the text content of a book using a book link.

    Args:
        book_link (str): The book link for downloading the book text.

    Returns:
        None
    """
    book_id = book_link.split("/")[2]
    redirected_url = f"https://www.gutenberg.org/cache/epub/{book_id}/pg{book_id}.txt"

    if redirected_url.endswith(".txt"):
        response = requests.get(redirected_url)
        if response.status_code == 200:
            text = response.content.decode("utf-8")

            # Create the output folder if it doesn't exist
            if not os.path.exists(output_folder):
                os.makedirs(output_folder)

            # Save the text to a local file
            output_file_path = os.path.join(output_folder, f"{book_id}.txt")
            with open(output_file_path, "w", encoding="utf-8") as f:
                f.write(text)

            print(f"Downloaded and saved {book_id}.txt")
    else:
        print(f"Unable to download {book_id}.txt")


if __name__ == "__main__":
    all_book_links = get_book_links(base_url)
    num_links = len(all_book_links)
    print(f"Total unique book links: {num_links}")

    with concurrent.futures.ThreadPoolExecutor(max_workers=10) as executor:
        executor.map(scrape_book_link, all_book_links)
