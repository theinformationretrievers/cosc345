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

// Fuzzy search min function. see https://github.com/farzher/fuzzysort/tree/master. for use.
((r,e)=>{"function"==typeof define&&define.amd?define([],e):"object"==typeof module&&module.exports?module.exports=e():r.fuzzysort=e()})(this,r=>{"use strict";var i,o,e,a,f=r=>{var e=v(r="string"!=typeof r?"":r);return{target:r,t:e.i,o:e.v,u:N,l:e.g,score:N,_:[0],obj:N}},t=r=>{r=(r="string"!=typeof r?"":r).trim();var e=v(r),a=[];if(e.p)for(var f,t=r.split(/\s+/),t=[...new Set(t)],n=0;n<t.length;n++)""!==t[n]&&(f=v(t[n]),a.push({v:f.v,i:t[n].toLowerCase(),p:!1}));return{v:e.v,g:e.g,p:e.p,i:e.i,h:a}},M=r=>{var e;return 999<r.length?f(r):(void 0===(e=n.get(r))&&(e=f(r),n.set(r,e)),e)},q=r=>{var e;return 999<r.length?t(r):(void 0===(e=s.get(r))&&(e=t(r),s.set(r,e)),e)},D=(r,e,a=!1)=>{if(!1===a&&r.p)return j(r,e);for(var a=r.i,f=r.v,t=f[0],n=e.o,i=f.length,o=n.length,v=0,s=0,u=0;;){if(t===n[s]){if(C[u++]=s,++v===i)break;t=f[v]}if(o<=++s)return N}var v=0,l=!1,g=0,d=e.u,c=(d===N&&(d=e.u=k(e.target)),s=0===C[0]?0:d[C[0]-1],0);if(s!==o)for(;;)if(o<=s){if(v<=0)break;if(200<++c)break;--v;s=d[L[--g]]}else if(f[v]===n[s]){if(L[g++]=s,++v===i){l=!0;break}++s}else s=d[s];var w=e.t.indexOf(a,C[0]),r=~w;if(r&&!l)for(var _=0;_<u;++_)C[_]=w+_;a=!1;r&&(a=e.u[w-1]===w);p=l?(b=L,g):(b=C,u);for(var b,p,x=0,h=0,_=1;_<i;++_)b[_]-b[_-1]!=1&&(x-=b[_],++h);if(x-=(12+(b[i-1]-b[0]-(i-1)))*h,0!==b[0]&&(x-=b[0]*b[0]*.2),l){for(var y=1,_=d[0];_<o;_=d[_])++y;24<y&&(x*=10*(y-24))}else x*=1e3;r&&(x/=1+i*i*1),a&&(x/=1+i*i*1),e.score=x-=o-i;for(_=0;_<p;++_)e._[_]=b[_];return e._.j=p,e},j=(r,e)=>{for(var a=new Set,f=0,t=N,n=0,i=r.h,o=0;o<i.length;++o){var v=i[o];if((t=D(v,e))===N)return N;f+=t.score,t._[0]<n&&(f-=n-t._[0]);for(var n=t._[0],s=0;s<t._.j;++s)a.add(t._[s])}r=D(r,e,!0);if(r!==N&&r.score>f)return r;t.score=f;var u,o=0;for(u of a)t._[o++]=u;return t._.j=o,t},v=r=>{for(var e=r.length,a=r.toLowerCase(),f=[],t=0,n=!1,i=0;i<e;++i){var o=f[i]=a.charCodeAt(i);32===o?n=!0:t|=1<<(97<=o&&o<=122?o-97:48<=o&&o<=57?26:o<=127?30:31)}return{v:f,g:t,p:n,i:a}},k=r=>{for(var e=r.length,a=(r=>{for(var e=r.length,a=[],f=0,t=!1,n=!1,i=0;i<e;++i){var o=r.charCodeAt(i),v=65<=o&&o<=90,o=v||97<=o&&o<=122||48<=o&&o<=57,s=v&&!t||!n||!o,t=v,n=o;s&&(a[f++]=i)}return a})(r),f=[],t=a[0],n=0,i=0;i<e;++i)i<t?f[i]=t:(t=a[++n],f[i]=void 0===t?e:t);return f},n=new Map,s=new Map,C=[],L=[],E=r=>{for(var e=J,a=r.length,f=0;f<a;++f){var t=r[f];t!==N&&e<(t=t.score)&&(e=t)}return e===J?N:e},F=(r,e)=>{var a=r[e];if(void 0!==a)return a;for(var f=e,t=(f=Array.isArray(e)?f:e.split(".")).length,n=-1;r&&++n<t;)r=r[f[n]];return r},G=r=>"object"==typeof r,H=1/0,J=-H,K=[],N=null,O=(i=[],o=K.total=0,a=r=>{for(var e=i[t=0],a=1;a<o;){var f=a+1,t=a;f<o&&i[f].score<i[a].score&&(t=f),i[t-1>>1]=i[t],a=1+(t<<1)}for(var n=t-1>>1;0<t&&e.score<i[n].score;n=(t=n)-1>>1)i[t]=i[n];i[t]=e},(e={}).add=r=>{var e=o;i[o++]=r;for(var a=e-1>>1;0<e&&r.score<i[a].score;a=(e=a)-1>>1)i[e]=i[a];i[e]=r},e.k=r=>{var e;if(0!==o)return e=i[0],i[0]=i[--o],a(),e},e.C=r=>{if(0!==o)return i[0]},e.L=r=>{i[0]=r,a()},e);return{single:(r,e)=>{var a;return"farzher"==r?{target:"farzher was here (^-^*)/",score:0,_:[0]}:!r||!e||(r=q(r),G(e)||(e=M(e)),((a=r.g)&e.l)!==a)?N:D(r,e)},go:(r,e,a)=>{if("farzher"==r)return[{target:"farzher was here (^-^*)/",score:0,_:[0],obj:e?e[0]:N}];if(!r)if(a&&a.all){var f=e;var t=a;var n=[],i=(n.total=f.length,t&&t.limit||H);if(t&&t.key)for(var o=0;o<f.length;o++){var v=f[o];var s=F(v,t.key);if(!s)continue;if(!G(s))s=M(s);s.score=J;s._.j=0;var u=s;u={target:u.target,t:"",o:N,u:N,l:0,score:s.score,_:N,obj:v};n.push(u);if(n.length>=i)return n}else if(t&&t.keys)for(o=0;o<f.length;o++){v=f[o];var l=new Array(t.keys.length);for(var g=t.keys.length-1;g>=0;--g){s=F(v,t.keys[g]);if(!s){l[g]=N;continue}if(!G(s))s=M(s);s.score=J;s._.j=0;l[g]=s}l.obj=v;l.score=J;n.push(l);if(n.length>=i)return n}else for(o=0;o<f.length;o++){s=f[o];if(!s)continue;if(!G(s))s=M(s);s.score=J;s._.j=0;n.push(s);if(n.length>=i)return n}return n;return}else return K;var d=q(r),c=d.g,w=(d.p,a&&a.threshold||J),_=a&&a.limit||H,b=0,p=0,x=e.length;if(a&&a.key)for(var h=a.key,y=0;y<x;++y){var j=e[y];!(m=F(j,h))||(c&(m=G(m)?m:M(m)).l)!==c||(B=D(d,m))===N||B.score<w||(B={target:B.target,t:"",o:N,u:N,l:0,score:B.score,_:B._,obj:j},b<_?(O.add(B),++b):(++p,B.score>O.C().score&&O.L(B)))}else if(a&&a.keys)for(var k=a.scoreFn||E,C=a.keys,L=C.length,y=0;y<x;++y){for(var j=e[y],S=new Array(L),z=0;z<L;++z){h=C[z];(m=F(j,h))?(c&(m=G(m)?m:M(m)).l)!==c?S[z]=N:S[z]=D(d,m):S[z]=N}S.obj=j;var A=k(S);A===N||A<w||(S.score=A,b<_?(O.add(S),++b):(++p,A>O.C().score&&O.L(S)))}else for(var m,B,y=0;y<x;++y)!(m=e[y])||(c&(m=G(m)?m:M(m)).l)!==c||(B=D(d,m))===N||B.score<w||(b<_?(O.add(B),++b):(++p,B.score>O.C().score&&O.L(B)));if(0===b)return K;for(var I=new Array(b),y=b-1;0<=y;--y)I[y]=O.k();return I.total=b+p,I},highlight:(r,e,a)=>{if("function"==typeof e){var f=e;if((l=r)===N)return N;for(var t=l.target,n=t.length,i=(i=l._).slice(0,i.j).sort((r,e)=>r-e),o="",v=0,s=0,u=!1,l=[],g=0;g<n;++g){var d=t[g];if(i[s]===g){if(++s,u||(u=!0,l.push(o),o=""),s===i.length){o+=d,l.push(f(o,v++)),o="",l.push(t.substr(g+1));break}}else u&&(u=!1,l.push(f(o,v++)),o="");o+=d}return l}if(r===N)return N;void 0===e&&(e="<b>"),void 0===a&&(a="</b>");for(var c="",w=0,_=!1,b=r.target,p=b.length,x=(x=r._).slice(0,x.j).sort((r,e)=>r-e),h=0;h<p;++h){var y=b[h];if(x[w]===h){if(_||(_=!0,c+=e),++w===x.length){c+=y+a+b.substr(h+1);break}}else _&&(_=!1,c+=a);c+=y}return c},prepare:f,indexes:r=>r._.slice(0,r._.j).sort((r,e)=>r-e),cleanup:()=>{n.clear(),s.clear(),C=[],L=[]}}});


const recentGridHTML = `
<div id="recent-grid" class="book-grid">
    <button onclick="addBook();" class="book-grid-item" id="btn-add-book">
        <h2>+ Add Book</h2>
        <p id="addbtntxt">Import a new file from your desktop</p>
    </button>
</div>`;

const libraryGridHTML = `
<div class="search-bar">
    <input id="search-input" type="text" placeholder="Search..">
</div>
<div id="library-grid" class="book-grid">
</div>` ;

const settingsHTML = `
<h1>Settings</h1>
<section id="general" class="settings-section">
    <form>
        <!-- Checkbox settings -->
        <label>
            <input type="checkbox" name="option1"> Option 1
        </label>
        <label>
            <input type="checkbox" name="option2"> Option 2
        </label>

        <label for="input-fontsize">FontSize:</label>
        <input type="number" id="input-fontsize" name="input-fontsize" min="0" max="50">

        <label for="dropdown-align">Choose text alignment:</label>
        <select id="dropdown-align" name="dropdown">
            <option value="left">Left Align</option>
            <option value="center">Center Align</option>
            <option value="right">Right Align</option>
        </select>

        <!-- Dropdown selection -->
        <label for="dropdown-theme">Choose a theme:</label>
        <select id="dropdown-theme" name="dropdown">
            <option value="theme-dark">Dark Mode</option>
            <option value="theme-light">Light Mode</option>
        </select>

        <!-- Large text box for newline-separated strings -->
        <label for="newlineStrings">Input newline-separated strings to blacklist:</label>
        <textarea id="newlineStrings" name="newlineStrings" rows="6"></textarea>
        
        <!-- Add more settings options as needed -->
        
        <button onclick="saveSettings();" type="button">Save</button>
        <!-- <button type="button">Cancel</button> -->
        <!-- <button type="button">Reset to Defaults</button> -->
    </form>
</section>
`;


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


function saveSettings() {
    // THEME SWITCHING
    let selectTheme = document.getElementById("dropdown-theme");
    let selectedIndex = selectTheme.selectedIndex;
    let selectedOption = selectTheme.options[selectedIndex];
    let className = document.body.className
    let selectedValue = selectedOption.value;
    document.body.classList.remove(className);
    document.body.classList.add(selectedValue);

    rootStyle = getComputedStyle(document.documentElement);

    //ALIGNMENT SWITCHING
    // let selectAlign = document.getElementById("dropdown-align");
    // let selectedAlignIndex = selectAlign.selectedIndex;
    // let selectedAlignOption = selectAlign.options[selectedAlignIndex];
    // let selectedAlignValue = selectedAlignOption.value;
    // console.log(selectedAlignValue)
    // document.documentElement.style.setProperty('--text-alignment', selectedAlignValue)

    // console.log(document.documentElement.style.getPropertyValue('--text-alignment'));


    // FONT SIZE SWITCHING
    let inputFont = document.getElementById("input-fontsize").value; // Corrected ID attribute
    document.documentElement.style.setProperty('--reader-font-size', (inputFont + "px"))

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

function openSettings(){
    document.getElementById("view").innerHTML = settingsHTML;
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


/**
 * @brief Check if a string contains a partial match of a sub-string (case-insensitive).
 * @param {string} text - The text to search within.
 * @param {string} searchTerm - The partial search term.
 * @returns {boolean} - True if the text contains a partial match of the search term, false otherwise.
 */
function containsPartialMatch(text, searchTerm) {
    text = text.toLowerCase();
    searchTerm = searchTerm.toLowerCase();

    return text.includes(searchTerm);
}

function performSearch() {
    const searchTerm = document.getElementById("search-input").value;

    document.getElementById("view").innerHTML = libraryGridHTML;

    if (searchTerm === "") {
        readAndCreateBooks();
    } else {
        customFuzzySearchAndCreateBooks(searchTerm);
        document.getElementById("search-input").value = searchTerm;
    }
}


/**
 * @brief Loads built-in books to the library based on a custom fuzzy search term.
 * @details Book metadata stored in a JSON file is read,
 * and for each book matching the search term (title or author), a grid item is created in the library.
 * @param {string} searchTerm - The search term to match books against.
 */
function customFuzzySearchAndCreateBooks(searchTerm) {
    fetch('books.json')
        .then(response => response.json())
        .then(data => {
            const libraryContainer = document.getElementById('library-grid'); // Replace with your actual class name

            // Use fuzzysort to perform the custom fuzzy search
            const results = fuzzysort.go(searchTerm, Object.values(data), {
                keys: ['Title', 'Author'],
                // limit: 10,
                threshold: -9999,

                // Create a custom combined score to sort by
                scoreFn: a => Math.max(a[0]?a[0].score:-10000, a[1]?a[1].score:-10000)
            });

            results.forEach(result => {
                const book = result.obj;

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


document.addEventListener('keydown', function (event) {
    if (event.key === "Enter") {
        performSearch()
    } else if (event.key === "ArrowLeft") {
        document.getElementById("prevButton").click();
    } else if (event.key === "ArrowRight") {
        document.getElementById("nextButton").click();
    }
});

document.addEventListener('DOMContentLoaded', () => {
    openRecent();
});
updatePageNumber();