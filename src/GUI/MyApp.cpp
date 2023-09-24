#include "MyApp.h"
#include "logging.h"
#include "translator.h"
#include <Ultralight/Ultralight.h>
#include <algorithm>
#include <cerrno>
#include <fstream>
#include <functional>
#include <iostream>
#include <set>
#include <sstream>
#include <stdio.h>
#include <string>
#ifdef _WIN32
#include <tchar.h>
#include <windows.h>
#endif

#define WINDOW_WIDTH 1440
#define WINDOW_HEIGHT 900
#define BUFSIZE MAX_PATH

MyApp::MyApp()
{
    ///
    ///
    /// Create our main App instance.
    ///
    app_ = App::Create();

    ///
    /// Create a resizable window by passing by OR'ing our window flags with
    /// kWindowFlags_Resizable.
    ///
    window_ = Window::Create(app_->main_monitor(), WINDOW_WIDTH, WINDOW_HEIGHT,
        false, kWindowFlags_Titled | kWindowFlags_Resizable);

    ///
    /// Create our HTML overlay-- we don't care about its initial size and
    /// position because it'll be calculated when we call OnResize() below.
    ///
    overlay_ = Overlay::Create(window_, 1, 1, 0, 0);

    ///
    /// Force a call to OnResize to perform size/layout of our overlay.
    ///
    OnResize(window_.get(), window_->width(), window_->height());

    ///
    /// Load a page into our overlay's View
    ///
    overlay_->view()->LoadURL("file:///library.html");

    ///
    /// Register our MyApp instance as an AppListener so we can handle the
    /// App's OnUpdate event below.
    ///
    app_->set_listener(this);

    ///
    /// Register our MyApp instance as a WindowListener so we can handle the
    /// Window's OnResize event below.
    ///
    window_->set_listener(this);

    ///
    /// Register our MyApp instance as a LoadListener so we can handle the
    /// View's OnFinishLoading and OnDOMReady events below.
    ///
    overlay_->view()->set_load_listener(this);

    ///
    /// Register our MyApp instance as a ViewListener so we can handle the
    /// View's OnChangeCursor and OnChangeTitle events below.
    ///
    overlay_->view()->set_view_listener(this);
}

MyApp::~MyApp() { }

void MyApp::Run() { app_->Run(); }

#ifdef _WIN32
/*!
 * @brief Use a dialog to get a file path
 * @details Using windows GetOpenFileName file dialog to open a file
 *					 and returns the file path
 * @returns The file path as a JSValue
 */
JSValue MyApp::GetFileWindows(const JSObject& thisObject, const JSArgs& args)
{
    HWND hwnd = (HWND)window_->native_handle();
    TCHAR szFilter[] = TEXT("Text Files (*.TXT)\0*.txt\0");

    OPENFILENAME ofn; // common dialog box structure
    char szFile[260]; // buffer for file name
    HANDLE hf; // file handle

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    // Set lpstrFile[0] to '\0' so that GetOpenFileName does not
    // use the contents of szFile to initialize itself.
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    // Display the Open dialog box.
    if (GetOpenFileName(&ofn) == TRUE) {
        hf = CreateFile(ofn.lpstrFile, GENERIC_READ, 0, (LPSECURITY_ATTRIBUTES)NULL,
            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
    } else {
        std::cerr << "Error opening file" << std::endl;
        std::string err("unable to open file");
        return JSValue(err.c_str());
    }

    // Get The File Path
    TCHAR Path[BUFSIZE];
    DWORD dwRet = GetFinalPathNameByHandle(hf, Path, BUFSIZE, FILE_NAME_NORMALIZED);

    // Close the file handle
    CloseHandle(hf);
    return JSValue(Path);
}

#else
/*!
* @brief Open get the contents of a file
* @details Using windows GetOpenFileName file dialog to open a file
*					 then converts it to a std::fstream and
reads the contents
* @returns The contents of the opened file as a JSValue string or the
           error as a JSValue string
*
*/
JSValue MyApp::GetFileLinux(const JSObject& thisObject, const JSArgs& args)
{
    std::array<char, 128> buffer;
    std::string result = "";

    FILE* pipe = popen("./file_chooser", "r");
    if (!pipe) {
        std::cerr << "Couldn't start command." << std::endl;
        return 0;
    }

    while (fgets(buffer.data(), 128, pipe) != nullptr) {
        result += buffer.data();
    }

    pclose(pipe);
    result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
    return JSValue(result.c_str());
}
#endif

/*!
* @brief Open get the contents of a file
* @details Using windows GetOpenFileName file dialog to open a file
*					 then converts it to a std::fstream and reads the contents
* @returns The contents of the opened file as a JSValue string or the
           error as a JSValue string
*/
// JSValue MyApp::GetTranslatedText(const JSObject& thisObject, const JSArgs& args) {
//   if (!args[0].IsString()) {
//     return JSValue("Invalid string");
//   }
//   JSValueRef exception = NULL;
//   JSStringRef jsPathString = JSValueToStringCopy(thisObject.context(), args[0], &exception);
//   size_t pathLength = JSStringGetMaximumUTF8CStringSize(jsPathString);
//   char* filePath = new char[pathLength];
//   JSStringGetUTF8CString(jsPathString, filePath, pathLength);
//   std::ifstream file(filePath);
//   if (!file.is_open()) {
//     std::cerr << "Failed to open the file: " << filePath << std::endl;
//     return JSValue("Failed to open the file");
//   }
//   delete[] filePath;
//   std::string fileContent = translate_and_replace(file, 42);
//   file.close();
//   return JSValue(fileContent.c_str());
// }
JSValue MyApp::GetTranslatedText(const JSObject& thisObject, const JSArgs& args)
{
    if (!args[0].IsString()) {
        return JSValue("Invalid string");
    }
    JSValueRef exception = NULL;
    double page = args[1].ToNumber();
    JSStringRef jsPathString = JSValueToStringCopy(thisObject.context(), args[0], &exception);
    size_t pathLength = JSStringGetMaximumUTF8CStringSize(jsPathString);
    char* filePath = new char[pathLength];
    JSStringGetUTF8CString(jsPathString, filePath, pathLength);
    std::string strPath = filePath;

    if (strPath != currentPath && strPath != "default") {
        currentPath = strPath;
        chunks = {};
        end = false;
        startPosition = 0;
        endPosition = 0;
        currentChunk = 0; // Reset the current chunk counter
        chunkFileIntoWords(strPath, currentChunk, numChunks); // Process the first chunk
        currentChunk += numChunks;
        delete[] filePath;
        updateReaderContent(chunks[0]); // Update with the first chunk

        return JSValue("Initialized");
    }

    if (page == -1) {
        // Process the next set of chunks in the background
        int chunksProcessed = chunkFileIntoWords(strPath, currentChunk, numChunks);
        currentChunk += chunksProcessed;
        delete[] filePath;
        return JSValue(std::to_string(chunksProcessed).c_str()); // Return the number of chunks processed
    }
    if(end == true){
        updateReaderContent(chunks[maxChunk-1]);
        return JSValue("end");
    }
    if (page >= maxChunk) {
        updateReaderContent("loading...");
        return JSValue("loading");

    }
    updateReaderContent(chunks[page]);
    delete[] filePath;
    return JSValue("Updated");
}

std::string MyApp::escapeSpecialCharacters(const std::string& fileContent)
{
    std::string escapedContent = fileContent;

    // Escape backslashes first
    size_t pos = 0;
    while ((pos = escapedContent.find("\\", pos)) != std::string::npos) {
        escapedContent.replace(pos, 1, "\\\\");
        pos += 2; // Move past the replaced content
    }

    // Escape double quotes
    pos = 0;
    while ((pos = escapedContent.find("\"", pos)) != std::string::npos) {
        escapedContent.replace(pos, 1, "\\\"");
        pos += 2; // Move past the replaced content
    }

    // Escape newlines
    pos = 0;
    while ((pos = escapedContent.find("\n", pos)) != std::string::npos) {
        escapedContent.replace(pos, 1, "\\n");
        pos += 2; // Move past the replaced content
    }

    // Optionally, escape other special characters like \r, \t, etc. if needed

    return escapedContent;
}

void MyApp::updateReaderContent(const std::string& escapedContent)
{

    std::string jsCode = "document.getElementById('reader-content').innerHTML = \" <pre>" + escapedContent + "</pre> \";";

    std::cout << jsCode << std::endl;
    overlay_->view()->EvaluateScript(jsCode.c_str());
}

int MyApp::chunkFileIntoWords(const std::string& filePath, int startChunk, int maxChunksToProcess)
{
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open the file");
    }

    // Move to the starting position
    // file.seekg(chunkSize * startChunk, std::ios::beg);

    int chunksProcessed = 0;
    while (chunksProcessed < maxChunksToProcess) {
        file.seekg(endPosition);
        startPosition = file.tellg();
        file.seekg(chunkSize, std::ios::cur); // Move forward by chunkSize
        endPosition = file.tellg();

        char ch;
        bool foundEndOfChunk = false;
        while (file.get(ch)) {
            if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
                // This is a letter, so do nothing
            } else {
                // This is not a letter, so it's the end of the word
                endPosition = file.tellg();
                foundEndOfChunk = true;
                break; // Exit the inner while loop
            }
        }
        if (!foundEndOfChunk && file.eof()) {
            end = true;
            break;
        }
        std::streamoff chunkLength = endPosition - startPosition;

        // Set the get position to startPosition
        file.seekg(startPosition);

        // Read the chunk into a buffer
        std::vector<char> buffer(chunkLength);
        file.read(buffer.data(), chunkLength);

        // Convert the buffer directly to a std::istringstream
        std::istringstream chunkStream(std::string(buffer.begin(), buffer.end()));

        // Process the chunk and store the result
        chunks.push_back(escapeSpecialCharacters(translate_and_replace(chunkStream, 42)));
        maxChunk = chunks.size();
        // std::cout << chunks.back() << std::endl;
        chunksProcessed++;
        if (file.eof()) {
            end = true;
            break;
        }
    }

    return chunksProcessed;
}

// return chunks;

/*!
* @brief Open get the contents of a file
* @details Using windows GetOpenFileName file dialog to open a file
*					 then converts it to a std::fstream and reads the contents
* @returns The contents of the opened file as a JSValue string or the
           error as a JSValue string
*/
JSValue MyApp::writeLocalBook(const JSObject& thisObject, const JSArgs& args)
{
    if (!args[0].IsString()) {
        return JSValue("Invalid string");
    }

    JSValueRef exception = NULL;
    JSStringRef jsPathString = JSValueToStringCopy(thisObject.context(), args[0], &exception);
    size_t pathLength2 = JSStringGetMaximumUTF8CStringSize(jsPathString);
    char* filePath2 = new char[pathLength2];
    JSStringGetUTF8CString(jsPathString, filePath2, pathLength2);

    std::string writeFile = "./local_files.txt";

    // Add each line to the set to ensure uniqueness
    std::ifstream inputFile(writeFile);
    std::set<std::string> uniqueLines;
    std::string line;

    while (std::getline(inputFile, line)) {
        uniqueLines.insert(line);
    }

    // If the new filepath is not already listed
    if (uniqueLines.find(filePath2) == uniqueLines.end()) {
        // Open the file in append mode
        std::ofstream file(writeFile, std::ios_base::app);
        if (file.is_open() && filePath2 != "unable to open file") {
            // Write the content followed by a newline character
            file << filePath2 << "\n";
            file.close();
            std::cout << "Content '" << filePath2 << "' written to file '" << writeFile << "'." << std::endl;
        } else {
            std::cerr << "Error: Unable to open file '" << writeFile << "' for writing." << std::endl;
        }
    } else {
        return JSValue("Duplicate");
    }

    return JSValue("Success");
}
/*!
* @brief Open get the contents of a file
* @details Using windows GetOpenFileName file dialog to open a file
*					 then converts it to a std::fstream and reads the contents
* @returns The contents of the opened file as a JSValue string or the
           error as a JSValue string
*/
JSValue MyApp::getPreviousLocalFiles(const JSObject& thisObject, const JSArgs& args)
{
    std::string writeFile = "./local_files.txt";

    // Add each line to the set to ensure uniqueness
    std::ifstream inputFile(writeFile);
    std::set<std::string> uniqueLines;
    std::string line;

    while (std::getline(inputFile, line)) {
        uniqueLines.insert(line);
    }

    ultralight::JSArray jsArray;

    for (const std::string& str : uniqueLines) {
        // Convert each string to a JSValue and add it to the JSArray
        ultralight::JSValue jsString = ultralight::JSValue(str.c_str());
        jsArray.push(jsString);
    }

    return JSValue(jsArray);
}

void MyApp::OnUpdate()
{
    ///
    /// This is called repeatedly from the application's update loop.
    ///
    /// You should update any app logic here.
    ///
}

void MyApp::OnClose(ultralight::Window* window) { app_->Quit(); }

void MyApp::OnResize(ultralight::Window* window, uint32_t width,
    uint32_t height)
{
    ///
    /// This is called whenever the window changes size (values in pixels).
    ///
    /// We resize our overlay here to take up the entire window.
    ///
    overlay_->Resize(width, height);
}

void MyApp::OnFinishLoading(ultralight::View* caller, uint64_t frame_id,
    bool is_main_frame, const String& url)
{
    ///
    /// This is called when a frame finishes loading on the page.
    ///
}

void MyApp::OnDOMReady(ultralight::View* caller, uint64_t frame_id,
    bool is_main_frame, const String& url)
{
    ///
    /// This is called when a frame's DOM has finished loading on the page.
    ///
    /// This is the best time to setup any JavaScript bindings.
    ///
    RefPtr<JSContext> context = caller->LockJSContext();
    SetJSContext(context->ctx());

    JSObject global = JSGlobalObject();

#ifdef _WIN32
    global["GetFilePath"] = BindJSCallbackWithRetval(&MyApp::GetFileWindows);
#else
    global["GetFilePath"] = BindJSCallbackWithRetval(&MyApp::GetFileLinux);
#endif
    global["GetTranslatedText"] = BindJSCallbackWithRetval(&MyApp::GetTranslatedText);
    global["writeLocalBookJS"] = BindJSCallbackWithRetval(&MyApp::writeLocalBook);
    global["getPreviousLocalFilesJS"] = BindJSCallbackWithRetval(&MyApp::getPreviousLocalFiles);

    // Call the openRecent() JavaScript function when the DOM is ready
    // this is for when the app first loads, as it doesn't seem to work when placed inside app.js
    ultralight::String* x = NULL;
    caller->EvaluateScript("openRecent();", x);
}

void MyApp::OnChangeCursor(ultralight::View* caller, Cursor cursor)
{
    ///
    /// This is called whenever the page requests to change the cursor.
    ///
    /// We update the main window's cursor here.
    ///
    window_->SetCursor(cursor);
}

void MyApp::OnChangeTitle(ultralight::View* caller, const String& title)
{
    ///
    /// This is called whenever the page requests to change the title.
    ///
    /// We update the main window's title here.
    ///
    window_->SetTitle(title.utf8().data());
}
void MyApp::OnAddConsoleMessage(ultralight::View* caller,
    ultralight::MessageSource source,
    ultralight::MessageLevel level,
    const ultralight::String& message,
    uint32_t line_number,
    uint32_t column_number,
    const ultralight::String& source_id)
{

    std::cout << "[Console]: [" << Stringify(source) << "] ["
              << Stringify(level) << "] " << ToUTF8(message);

    if (source == kMessageSource_JS) {
        std::cout << " (" << ToUTF8(source_id) << " @ line " << line_number
                  << ", col " << column_number << ")";
    }

    std::cout << std::endl;
}