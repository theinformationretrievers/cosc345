function readAndCreateBooks() {
    fetch('books.json')
        .then(response => response.json())
        .then(data => {
            const libraryContainer = document.querySelector('.library-grid'); // Replace with your actual class name

            Object.keys(data).forEach(bookKey => {
                const book = data[bookKey];

                const button = document.createElement('button');
                button.className = 'library-grid-item';
                button.onclick = function () {
                    // Add your custom functionality here when the button is clicked
                    // You can access book properties like book.Title, book.Author, etc.
                    console.log("Button clicked for:", book.Title);
                };

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

// Call the function to read and create buttons when your page loads
document.addEventListener('DOMContentLoaded', readAndCreateBooks);
