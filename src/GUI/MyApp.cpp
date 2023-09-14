#include "MyApp.h"

#include <stdio.h>

#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

#include "translator.h"
#include <cerrno>
#ifdef _WIN32
#include <tchar.h>
#include <windows.h>
#endif

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 400
#define BUFSIZE MAX_PATH

MyApp::MyApp() {
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
  overlay_->view()->LoadURL("file:///reader.html");

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

MyApp::~MyApp() {}

void MyApp::Run() { app_->Run(); }

#ifdef _WIN32
/*!
* @brief Open get the contents of a file
* @details Using windows GetOpenFileName file dialog to open a file
*					 then converts it to a std::fstream and
reads the contents
* @returns The contents of the opened file as a JSValue string or the
           error as a JSValue string
*
*/
JSValue MyApp::GetFileWindows(const JSObject& thisObject, const JSArgs& args) {
  HWND hwnd = (HWND)window_->native_handle();
  TCHAR szFilter[] = TEXT("Text Files (*.TXT)\0*.txt\0");

  OPENFILENAME ofn;  // common dialog box structure
  char szFile[260];  // buffer for file name
  HANDLE hf;         // file handle

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
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

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
  DWORD dwRet =
      GetFinalPathNameByHandle(hf, Path, BUFSIZE, FILE_NAME_NORMALIZED);

  // Close the file handle
  CloseHandle(hf);

  // Create an fstream with the file path and read the text
  // std::fstream fs(Path, std::fstream::in | std::fstream::out);
  std::ifstream file(Path);

  // std::streampos initialPos = file.tellg();
  // std::vector<translation> translated_words  = translate(file);
  // file.seekg(initialPos);

  std::string fileContent = translate_and_replace(file);

  if (file.is_open()) {
    // std::istreambuf_iterator<char> it(file);
    // std::istreambuf_iterator<char> end;

    // fileContent.append(it, end);

    // fileContent.append("</pre>");

    // Close the file stream
    file.close();
  } else {
    std::cerr << "Error opening file" << std::endl;
    std::string err("unable to open file:'");
    err.append(Path).append("'");
    return JSValue(err.c_str());
  }

  // Close the file stream
  file.close();
  return JSValue(fileContent.c_str());

  // TODO Call Subsituter here
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
JSValue MyApp::GetFileLinux(const JSObject& thisObject, const JSArgs& args) {
  std::cout << "Hello, world!" << std::endl;

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

  // std::cout << result << std::endl;
  
  // std::cout << (result == "/home/cody/testfile.txt") << std::endl;

  // auto returnCode = pclose(pipe);

  // if (returnCode == EXIT_SUCCESS) {
  //   std::cout << "Selected file: " << result << std::endl;
  // } else {
  //   std::cerr << "Error during file selection." << std::endl;
  // }
  // result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
  // std::ifstream file(result);

  // if (!file.is_open()) {
  //   perror("Error opening file");
  //   // std::cerr << "Failed to open file: " << result << std::endl;
  //   return "";
  // }
  while (fgets(buffer.data(), 128, pipe) != nullptr) {
    result += buffer.data();
  }

  result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());

  auto returnCode = pclose(pipe);

  if (returnCode == EXIT_SUCCESS) {
      std::cout << "Selected file: " << result << std::endl;
  } else {
      std::cerr << "Error during file selection." << std::endl;
  }

  std::ifstream file(result);
  if (!file) {
      std::cerr << "Failed to open the file: " << result << std::endl;
  }
  std::string fileContents;
  std::string line;
  while (std::getline(file, line)) {
    fileContents += line + "\n";  // Add newline character if needed
  }

  file.close();
  // You may need to return an ultralight::JSValue here, depending on your code
  // logic
  return JSValue(fileContents.c_str());
}
#endif

void MyApp::OnUpdate() {
  ///
  /// This is called repeatedly from the application's update loop.
  ///
  /// You should update any app logic here.
  ///
}

void MyApp::OnClose(ultralight::Window* window) { app_->Quit(); }

void MyApp::OnResize(ultralight::Window* window, uint32_t width,
                     uint32_t height) {
  ///
  /// This is called whenever the window changes size (values in pixels).
  ///
  /// We resize our overlay here to take up the entire window.
  ///
  overlay_->Resize(width, height);
}

void MyApp::OnFinishLoading(ultralight::View* caller, uint64_t frame_id,
                            bool is_main_frame, const String& url) {
  ///
  /// This is called when a frame finishes loading on the page.
  ///
}

void MyApp::OnDOMReady(ultralight::View* caller, uint64_t frame_id,
                       bool is_main_frame, const String& url) {
  ///
  /// This is called when a frame's DOM has finished loading on the page.
  ///
  /// This is the best time to setup any JavaScript bindings.
  ///
  RefPtr<JSContext> context = caller->LockJSContext();
  SetJSContext(context->ctx());

  JSObject global = JSGlobalObject();

#ifdef _WIN32
  global["GetFile"] = BindJSCallbackWithRetval(&MyApp::GetFileWindows);
#else
  global["GetFile"] = BindJSCallbackWithRetval(&MyApp::GetFileLinux);
#endif
}

void MyApp::OnChangeCursor(ultralight::View* caller, Cursor cursor) {
  ///
  /// This is called whenever the page requests to change the cursor.
  ///
  /// We update the main window's cursor here.
  ///
  window_->SetCursor(cursor);
}

void MyApp::OnChangeTitle(ultralight::View* caller, const String& title) {
  ///
  /// This is called whenever the page requests to change the title.
  ///
  /// We update the main window's title here.
  ///
  window_->SetTitle(title.utf8().data());
}
