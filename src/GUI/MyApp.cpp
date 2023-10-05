#include "MyApp.h"
#include "logging.h"
#include "translator.h"
#include <Ultralight/Ultralight.h>
#include <algorithm>
#include <array>
#include <cerrno>
#include <fstream>
#include <functional>
#include <iostream>
#include <set>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

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
JSValue MyApp::getFileWindows(const JSObject& thisObject, const JSArgs& args)
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
JSValue MyApp::getFileLinux(const JSObject& thisObject, const JSArgs& args)
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

JSValue MyApp::getTranslatedText(const JSObject& thisObject, const JSArgs& args)
{
    if (!args[0].IsString()) {
        return JSValue("Invalid string");
    }
    double page = args[1].ToNumber();
    JSValueRef exception = NULL;

    // Extracting the filePath
    JSStringRef jsPathString = JSValueToStringCopy(thisObject.context(), args[0], &exception);
    size_t pathLength = JSStringGetMaximumUTF8CStringSize(jsPathString);
    char* filePath = new char[pathLength];
    JSStringGetUTF8CString(jsPathString, filePath, pathLength);
    std::string strPath = filePath;

    // Extracting the status
    delete[] filePath;
    if (strPath != currentPath || currentPath == "default") {
        currentPath = strPath;
        fileStream.close();
        pages = {};
        endPage = false;
        startPosition = 0;
        endPosition = 0;
        fileSize = 1000000;
        page = 0;
        fileStream = std::ifstream(strPath);
        fileStream.seekg(0, std::ios::end); // Move to the end of the file
        fileSize = fileStream.tellg(); // Get the current position (which is the end)
        fileStream.seekg(0, std::ios::beg); // Move back to the beginning or to the original position

        if (!fileStream.is_open()) {
            std::cerr << "Failed to open the fileStream: " << strPath << std::endl;
            diagnoseFileStream("MyApp::getTranslatedText - After trying to open file");
            return JSValue("Failed to open the fileStream");
        }

        translateNextChunk();
    }
    updateReaderContent(page);
    if (endPage) {
        return JSValue(pages.size());
    }
    return JSValue("normal");
}
void MyApp::diagnoseFileStream(const std::string& context)
{
    std::cerr << "Diagnostics from: " << context << std::endl;
    if (!fileStream.is_open()) {
        std::cerr << "File stream is not open." << std::endl;
    } else {
        std::cerr << "File stream is open." << std::endl;
    }

    if (fileStream.eof()) {
        std::cerr << "End of file reached." << std::endl;
    }

    if (fileStream.fail()) {
        std::cerr << "File stream has encountered an error." << std::endl;
    }

    if (fileStream.bad()) {
        std::cerr << "File stream has encountered a non-recoverable error." << std::endl;
    }

    std::cerr << "Current file stream position: " << fileStream.tellg() << std::endl;
}


JSValue MyApp::saveBlacklist(const JSObject& thisObject, const JSArgs& args) {
    if (args.size() >= 1 && args[0].IsString()) {
        // Convert JSArg to a string
        JSValueRef exception = NULL;

        // Extracting the filePath
        JSStringRef jsStr = JSValueToStringCopy(thisObject.context(), args[0], &exception);
        size_t jsLength = JSStringGetMaximumUTF8CStringSize(jsStr);
        char* blacklistStrJS = new char[jsLength];
        JSStringGetUTF8CString(jsStr, blacklistStrJS, jsLength);

        std::string blacklistStr = blacklistStrJS;

        // Create a vector to store the individual words
        std::vector<std::string> words;

        // Use stringstream to split the string by spaces
        std::istringstream iss(blacklistStr);
        std::string word;

        while (iss >> word) {
            words.push_back(word);
        }

        // Now 'words' contains individual words from the space-separated string
        for (const std::string& w : words) {
            std::cout << w << std::endl;
        }

        update_blacklist_word(words);


    } else {
        std::cerr << "Invalid arguments passed to SavePreferences!" << std::endl;
    }

    return JSValue(" "); // Return undefined to JS
}

void MyApp::updateReaderContent(const double page)
{
    if (pages.size() <= page) {
        translateNextChunk();
    }
    std::string jsCode = "document.getElementById('reader-content').innerHTML = `<pre>" + pages[page] + "</pre>`; document.getElementById('view').scrollTop = 0";

    overlay_->view()->EvaluateScript(jsCode.c_str());
}

void MyApp::translateNextChunk()
{
    startPosition = fileStream.tellg();

    if ((chunkSize + startPosition) > fileSize) {
        std::streamoff chunkLength = fileSize - startPosition;
        std::vector<char> buffer(chunkLength);
        fileStream.read(buffer.data(), chunkLength);
        std::istringstream chunkStream(std::string(buffer.begin(), buffer.end()));
        std::string result = translate_and_replace(chunkStream, 42);
        pages.push_back(result);
        fileStream.seekg(endPosition);

        endPage = true;
    } else {

        fileStream.seekg(chunkSize, std::ios::cur); // Move forward by chunkSize
        endPosition = fileStream.tellg();

        char ch;
        bool foundEndOfChunk = false;
        while (fileStream.get(ch)) {
            if ((ch >= 41 && ch <= 90) || (ch >= 97 && ch <= 122)) {
            } else {
                // This is not a letter, so it's the end of the word
                endPosition = fileStream.tellg();
                foundEndOfChunk = true;
                break;
            }
        }
        if (!foundEndOfChunk && fileStream.eof()) {
            endPosition = fileStream.tellg();
        }
        std::streamoff chunkLength = endPosition - startPosition;

        // Set the get position to startPosition
        fileStream.seekg(startPosition);
        if (!fileStream.good()) {
            std::cerr << "File stream is not in a good state before reading the chunk." << std::endl;
            diagnoseFileStream("translateNextChunk - Before reading chunk");
        }
        // Read the chunk into a buffer
        std::vector<char> buffer(chunkLength);
        fileStream.read(buffer.data(), chunkLength);
        if (fileStream.fail()) {
            std::cerr << "Failed to read the chunk from the file." << std::endl;
            diagnoseFileStream("translateNextChunk - After reading chunk");
        }
        // Convert the buffer directly to a std::istringstream
        std::istringstream chunkStream(std::string(buffer.begin(), buffer.end()));
        std::string result = translate_and_replace(chunkStream, 42);
        pages.push_back(result);

        fileStream.seekg(endPosition);
    }
}

/*
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

JSValue MyApp::savePreferences(const JSObject& thisObject, const JSArgs& args)
{
    if (args.size() >= 1 && args[0].IsString()) {
        // Convert JSArg to a string
        JSValueRef exception = NULL;

        // Extracting the filePath
        JSStringRef jsJson = JSValueToStringCopy(thisObject.context(), args[0], &exception);
        size_t jsLength = JSStringGetMaximumUTF8CStringSize(jsJson);
        char* json = new char[jsLength];
        JSStringGetUTF8CString(jsJson, json, jsLength);
        ultralight::String jsonStr = json;
        // std::cout << jsonStr << std::endl;
        // Save the string to a file
        std::ofstream outFile("userPreferences.json");
        if (outFile.is_open()) {
            outFile << jsonStr.utf8().data();
            outFile.close();
            std::cout << "Preferences saved successfully!" << std::endl;
        } else {
            std::cerr << "Unable to open file for writing!" << std::endl;
        }
    } else {
        std::cerr << "Invalid arguments passed to SavePreferences!" << std::endl;
    }

    return JSValue(" "); // Return undefined to JS
}

JSValue MyApp::loadPreferences(const JSObject& thisObject, const JSArgs& args)
{
    // Load the string from a file
    std::ifstream inFile("userPreferences.json");
    if (inFile.is_open()) {
        std::stringstream buffer;
        buffer << inFile.rdbuf();
        inFile.close();
        std::string result = buffer.str();
        if (!result.empty()) {
            std::cout << "Preferences loaded successfully!" << std::endl;
            return result.c_str();
        } else {
            std::cerr << "Failed to parse JSON!" << std::endl;
        }
    } else {
        std::cerr << "Unable to open file for reading!" << std::endl;
    }

    return JSValue(" "); // Return undefined to JS in case of error
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
    global["getFilePath"] = BindJSCallbackWithRetval(&MyApp::getFileWindows);
#else
    global["getFilePath"] = BindJSCallbackWithRetval(&MyApp::getFileLinux);
#endif
    global["getTranslatedText"] = BindJSCallbackWithRetval(&MyApp::getTranslatedText);
    global["writeLocalBookJS"] = BindJSCallbackWithRetval(&MyApp::writeLocalBook);
    global["getPreviousLocalFilesJS"] = BindJSCallbackWithRetval(&MyApp::getPreviousLocalFiles);
    global["savePreferences"] = BindJSCallbackWithRetval(&MyApp::savePreferences);
    global["loadPreferences"] = BindJSCallbackWithRetval(&MyApp::loadPreferences);
    // Call he openRecent() JavaScript function when the DOM is ready
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