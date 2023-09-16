const readerHTML = `
<div class="reader">
    <header>
        <div class="header-menu">
        </div>
    </header>
    <div id="reader-content">
    </div>
    <footer>
        <div class="footer-menu">
            <div class="clickable-icon"><svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" class="svg-icon lucide-arrow-left"><line x1="19" y1="12" x2="5" y2="12"></line><polyline points="12 19 5 12 12 5"></polyline></svg></div>
            <div class="clickable-icon"><svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" class="svg-icon lucide-arrow-right"><line x1="5" y1="12" x2="19" y2="12"></line><polyline points="12 5 19 12 12 19"></polyline></svg></div>
        </div>
    </footer>
</div>`

const recentGridHTML = `
<div id="recent-grid" class="book-grid">
    <button onclick="addBook()" class="book-grid-item" id="btn-add-book">
        <h2>+ Add Book</h2>
        <p id="addbtntxt">Import a new file from your desktop</p>
    </button>
</div>`

const libraryGridHTML = `
<div id="library-grid" class="book-grid">
</div>` 

function addBook() {
    const filePath = GetFilePath();
    const recentGrid = document.getElementById("recent-grid");

    const button = document.createElement('button');
    button.className = 'book-grid-item';
    button.onclick = () => openBook(filePath);

    const h3 = document.createElement('h3');
    h3.textContent = "local book title";

    const p = document.createElement('p');
    p.textContent = "local book author";

    button.appendChild(h3);
    button.appendChild(p);

    recentGrid.appendChild(button);

    // TODO save opened book to a database
}

function openBook(filePath) {
    console.log(filePath);
    // ISSSUE library book file paths are wrong
    document.getElementById("view").innerHTML = readerHTML;
    const translatedText = GetTranslatedText(filePath);
    document.getElementById("reader-content").innerHTML = translatedText;
}

function openRecent() {
    // TODO load books from recent books in database
    document.getElementById("view").innerHTML = recentGridHTML;
}

function openLibrary() {
    document.getElementById("view").innerHTML = libraryGridHTML;
    console.log("open library");
    readAndCreateBooks();
}

function readAndCreateBooks() {
    fetch('books.json')
        .then(response => response.json())
        .then(data => {
            const libraryContainer = document.getElementById('library-grid'); // Replace with your actual class name

            Object.keys(data).forEach(bookKey => {
                const book = data[bookKey];

                const button = document.createElement('button');
                button.className = 'book-grid-item';
                button.onclick = () => openBook(book.Path);

                const h3 = document.createElement('h3');
                h3.textContent = book.Title;

                const p = document.createElement('p');
                p.textContent = book.Author;

                button.appendChild(h3);
                button.appendChild(p);

                libraryContainer.appendChild(button);
            });
        })
        .catch(error => {
            console.error('Error reading JSON file:', error);
        });
}

document.addEventListener('DOMContentLoaded', openRecent);