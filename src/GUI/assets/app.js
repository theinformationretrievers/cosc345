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
            <button id="firstPageButton" class="clickable-icon" onclick="goToFirstPage();">
            <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 15 15" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" class="svg-icon lucide-arrow-left">
            x<path fill-rule="evenodd" clip-rule="evenodd" d="M6.85355 3.85355C7.04882 3.65829 7.04882 3.34171 6.85355 3.14645C6.65829 2.95118 6.34171 2.95118 6.14645 3.14645L2.14645 7.14645C1.95118 7.34171 1.95118 7.65829 2.14645 7.85355L6.14645 11.8536C6.34171 12.0488 6.65829 12.0488 6.85355 11.8536C7.04882 11.6583 7.04882 11.3417 6.85355 11.1464L3.20711 7.5L6.85355 3.85355ZM12.8536 3.85355C13.0488 3.65829 13.0488 3.34171 12.8536 3.14645C12.6583 2.95118 12.3417 2.95118 12.1464 3.14645L8.14645 7.14645C7.95118 7.34171 7.95118 7.65829 8.14645 7.85355L12.1464 11.8536C12.3417 12.0488 12.6583 12.0488 12.8536 11.8536C13.0488 11.6583 13.0488 11.3417 12.8536 11.1464L9.20711 7.5L12.8536 3.85355Z" fill="#000000"/>
</svg>
</button>

            <button id="prevButton" class="clickable-icon" onclick="prevPage();">
                <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" class="svg-icon lucide-arrow-left">
                    <line x1="19" y1="12" x2="5" y2="12"></line>
                    <polyline points="12 19 5 12 12 5"></polyline></svg></button>
            <span id="pageNumber">0</span>
            <button id="nextButton" class="clickable-icon" onclick="nextPage();"><svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round" class="svg-icon lucide-arrow-right"><line x1="5" y1="12" x2="19" y2="12"></line><polyline points="12 5 19 12 12 19"></polyline></svg></button>
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
let max = false;
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
    openBook(filePath);
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
    console.log(filePath);
    currentPath = filePath;
    currentPage = 0;
    maxPage = Infinity;
    max = false;
    document.getElementById("view").innerHTML = readerHTML;
    const status = GetTranslatedText(currentPath, currentPage); // Load the initial content
    if (status != "normal" && max == false) {
        maxPage = currentPage;
        max = true;
    }
    // Start background processing for the rest of the content
}




function changePage(direction) {
    if (direction === "next" && currentPage < maxPage) {
        currentPage++;
    } else if (direction === "prev" && currentPage > 0) { // Ensure currentPage doesn't go negative
        currentPage--;
    }
    updatePageNumber();
    const status = GetTranslatedText(currentPath, currentPage);
    if (status != "normal" && max == false) {
        maxPage = currentPage;
        max = true;
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
function nextPage() {
    changePage("next");
}

function prevPage() {
    changePage("prev");
}

function goToFirstPage() {
    currentPage = 0;
    let content = document.getElementById("reader-content");
    content.style.animation = "fadeOut 0.5s forwards";

    setTimeout(() => {

        changePage("");
        content.style.animation = "fadeIn 0.5s forwards";
    }, 500); // 500ms matches the duration of the fade-out animation
}


function updatePageNumber() {
    if (currentPage == maxPage) {
        document.getElementById("pageNumber").textContent = "Final page";
    } else {
        document.getElementById("pageNumber").textContent = currentPage;
    }
}

document.addEventListener('keydown', function (event) {
    if (event.key === "ArrowLeft") {
        document.getElementById("prevButton").click();
    } else if (event.key === "ArrowRight") {
        document.getElementById("nextButton").click();
    }
});
document.addEventListener('DOMContentLoaded', () => {
    openRecent();
});
updatePageNumber();