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
            <button class="clickable-icon" onclick="prevPage();"><svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" class="svg-icon lucide-arrow-left"><line x1="19" y1="12" x2="5" y2="12"></line><polyline points="12 19 5 12 12 5"></polyline></svg><span id="pageNumber">0</span></button>
            <button class="clickable-icon" onclick="nextPage();"><svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" class="svg-icon lucide-arrow-right"><line x1="5" y1="12" x2="19" y2="12"></line><polyline points="12 5 19 12 12 19"></polyline></svg></button>
        </div>
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


let currentPage = 0;
let maxPage = Infinity;
let currentPath = "";
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
    currentPath = filePath;
    document.getElementById("view").innerHTML = readerHTML;
    const status = GetTranslatedText(filePath, 0); // Load the initial content
    // Start background processing for the rest of the content
    processInBackground(filePath);
}

function processInBackground(filePath) {
    // Use -1 to indicate background processing
    const chunksProcessed = GetTranslatedText(filePath, -1);
    // console.log("Chunks: ");
    // console.log(chunksProcessed);
    if (chunksProcessed > 0) {
        setTimeout(() => processInBackground(filePath), 1);
    }
}


function changePage(direction) {
    if (direction === "next") {
        currentPage++;
    } else if (direction === "prev" && currentPage > 0) { // Ensure currentPage doesn't go negative
        currentPage--;
    }

    updatePageNumber();
    const status = GetTranslatedText(currentPath, 0); // Load the initial content

    // fetchTranslatedText();
}

function fetchTranslatedText() {
    const value = GetTranslatedText(currentPath, currentPage);
    if (value === "loading") {
        setTimeout(fetchTranslatedText, 1000);  // Wait for 1 second (1000 milliseconds) before trying again
    } 
    // if (value === "end"){
    //     maxPage = currentPage    }
}

// You can call changePage with the desired direction
// changePage("next");


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
function nextPage() {
    changePage("next");
}

function prevPage() {
    changePage("prev");
}

function updatePageNumber() {
    document.getElementById("pageNumber").textContent = currentPage;
}


document.addEventListener('DOMContentLoaded', () => {
    openRecent();
});
updatePageNumber();
