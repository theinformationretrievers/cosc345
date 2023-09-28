const readerHTML = `
<div class="reader">
    <header>
        <div class="header-menu">
        </div>
    </header>
    <div id="reader-content">
    </div>
    <footer>
        
    </footer>
</div>`;


const recentGridHTML = `
<div id="recent-grid" class="book-grid">
    <button onclick="addBook();" class="book-grid-item" id="btn-add-book">
        <h2>+ Add Book</h2>
        <p id="addbtntxt">Import a new file from your desktop</p>
    </button>
</div>`;

const libraryGridHTML = `
<div id="library-grid" class="book-grid">
</div>` ;
let contentCounter = 1;
let currentPage = 0;
let currentPath = ""
let lastScrollTop = 0; // This variable will store the previous scroll position
// const pages = [];
/*! 
 * @brief Add a book to the users local library
 * @details Opens a file dialog to select a file. Then saves the filepath
 * locally and adds the book to the users recently opened books
 */
function addBook() {
    const filePath = GetFilePath();
    const result = writeLocalBookJS(filePath);
    if (result == "Success") {
        createBook(filePath);
    }
}

/*! 
 * @brief Creates a book grid item from a filepath in the users recent books
 * @param filePath the filePath to the local file
 */
function createBook(filePath) {
    const recentGrid = document.getElementById("recent-grid");

    const button = document.createElement('button');
    button.className = 'book-grid-item';
    button.onclick = () => openBook(filePath);

    const filename = getFileNameFromPath(filePath);

    const h3 = document.createElement('h3');
    h3.textContent = filename;

    const p = document.createElement('p');
    p.textContent = "local book";

    button.appendChild(h3);
    button.appendChild(p);

    recentGrid.appendChild(button);
}

/*!
* @brief Add a book to the users local library
* @details Opens a file dialog to select a file. Then saves the filepath
* locally and adds the book to the users recently opened books
*/

function openBook(filePath) {
    console.log(filePath)
    currentPath = filePath;
    currentPage = 0;
    lastScrollTop = 0;
    contentCounter = 1;
    document.getElementById("view").innerHTML = readerHTML;
    const content = document.getElementById("view"); 
    const status = GetTranslatedText(filePath, 0); // Load the initial content
    content.addEventListener('scroll', function () {
        console.log("scrolling")
        // Check if user scrolled down
        if (content.scrollTop > lastScrollTop) {
            if (content.scrollTop + content.clientHeight >= content.scrollHeight) {
                nextPage();
            }
        }
        // Check if user scrolled up
        else if (content.scrollTop < lastScrollTop) {
            prevPage();
        }
        // Update the lastScrollTop with the current scrollTop value
        lastScrollTop = content.scrollTop;
    });
}
function changePage(direction) {
    if (direction === "next" && currentPage) {
        currentPage++;
    } else if (direction === "prev" && currentPage > 0) { // Ensure currentPage doesn't go negative
        currentPage--;
    }
    fetchTranslatedText();
}

function fetchTranslatedText() {
    const value = GetTranslatedText(currentPath, currentPage);
}



function nextPage() {
    changePage("next");
}

function prevPage() {
    changePage("prev");
}

function loadMoreText(filePath) {
    // Simulating an asynchronous call to fetch more content
    setTimeout(function () {
        const newText = document.createElement('pre');

        content.appendChild(newText);

        // Check if content fills the container
        if (content.scrollHeight <= content.clientHeight) {
            loadMoreText(filePath); // Load another chunk if the container isn't filled
        }
    }, 0); // 500ms delay to simulate network latency
}
function removeOldText() {
    const content = document.getElementById('reader-content');
    if (content.children.length > 5) { // Keep only the last 5 chunks
        content.removeChild(content.firstChild);
    }
}
/*! 
 * @brief Opens the recent books view
 * @details loads the users recently opened local files
 */
function openRecent() {
    document.getElementById("view").innerHTML = recentGridHTML;

    // Load and display recent files when the app initially loads
    const localFilepathList = getPreviousLocalFilesJS();
    localFilepathList.forEach((filepath) => {
        createBook(filepath);
    });
}

/*! 
 * @brief Opens the library view
 */
function openLibrary() {
    document.getElementById("view").innerHTML = libraryGridHTML;
    console.log("open library");
    readAndCreateBooks();
}

/*! 
 * @brief Loads built in books to library
 * @details Book metadata stored in a json file which is read,
 * and for each book a grid item is created in the library
 */
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

/*! 
 * @brief Separate the filename from the path and the extension
 */
function getFileNameFromPath(filePath) {
    // Use the last '/' or '\' (whichever is found last) to split the path
    const pathSeparator = filePath.lastIndexOf('/') > -1 ? '/' : '\\';
    const parts = filePath.split(pathSeparator);

    // Get the last part (the file name)
    const fileNameWithExtension = parts[parts.length - 1];

    // Split the file name to separate the name and extension
    const fileNameParts = fileNameWithExtension.split('.');

    // If there's more than one part, remove the last part (the extension)
    if (fileNameParts.length > 1) {
        fileNameParts.pop();
    }

    // Join the remaining parts to get the file name without the extension
    const fileNameWithoutExtension = fileNameParts.join('.');

    return fileNameWithoutExtension;
}

function handleScroll(event) {
    const content = event.target;
    console.log("scrolling");
    // Check if user scrolled down
    if (content.scrollTop > lastScrollTop) {
        if (content.scrollTop + content.clientHeight >= content.scrollHeight) {
            nextPage();
        }
    }
    // Check if user scrolled up
    else if (content.scrollTop < lastScrollTop) {
        prevPage();
    }
    // Update the lastScrollTop with the current scrollTop value
    lastScrollTop = content.scrollTop;
}




document.addEventListener('DOMContentLoaded', () => {
    openRecent();
});
