const readerHTML = `
<div class="reader">
    <header>
        <div class="header-menu">
        </div>
    </header>
    <div id="reader-content">
        <div id="top-sentinel" class="sentinel"></div>
        <div id="middle-content" class="content"></div>
        <div id="bottom-sentinel" class="sentinel"></div>
    </div>
    <footer>
        
    </footer>
</div>
`


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

const options = {
    root: null, // Use the viewport as the root
    rootMargin: '0px',
    threshold: 0.1 // Trigger callback when at least 10% of the sentinel is visible
};
let isLoadingContent = false;
let currentPath = ""
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
    const view = document.getElementById("view")
    view.innerHTML = readerHTML;
    GetTranslatedText(filePath, "current"); // Load the initial content
    view.addEventListener('scroll', debounce(function () {
        if (isLoadingContent) {
            return; // Exit if content is currently being loaded
        }
        const bottomSentinel = document.getElementById('bottom-sentinel');
        const topSentinel = document.getElementById('top-sentinel');

        const bottomSentinelPosition = bottomSentinel.getBoundingClientRect();
        const topSentinelPosition = topSentinel.getBoundingClientRect();
        // Check if bottom sentinel is in view
        if (topSentinelPosition.top <= window.innerHeight && topSentinelPosition.bottom >= 0) {
            GetTranslatedText(currentPath, "prev");
            
            isLoadingContent = false;
        }
        if (bottomSentinelPosition.top <= window.innerHeight && bottomSentinelPosition.bottom >= 0) {
            GetTranslatedText(currentPath, "next");
            isLoadingContent = false;
        }

        // Check if top sentinel is in view
    },0));
}





function nextPage() {
    changePage("next");
}

function prevPage() {
    changePage("prev");
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


function debounce(func, wait) {
    let timeout;
    return function() {
        const context = this, args = arguments;
        clearTimeout(timeout);
        timeout = setTimeout(function() {
            func.apply(context, args);
        }, wait);
    };
}




document.addEventListener('DOMContentLoaded', () => {
    openRecent();
});
