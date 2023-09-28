#pragma once
#include "logging.h"
#include <AppCore/AppCore.h>
#include <Ultralight/Ultralight.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

using namespace ultralight;

class MyApp : public AppListener,
              public WindowListener,
              public LoadListener,
              public ViewListener {
public:
    MyApp();

    virtual ~MyApp();

    // Start the run loop.
    virtual void Run();

    // Open a File

#ifdef _WIN32
    virtual JSValue GetFileWindows(const JSObject& thisObject, const JSArgs& args);
#else
    // Open a File
    virtual JSValue GetFileLinux(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args);
#endif

    virtual JSValue GetTranslatedText(const JSObject& thisObject, const JSArgs& args);

    virtual JSValue writeLocalBook(const JSObject& thisObject, const JSArgs& args);

    virtual JSValue getPreviousLocalFiles(const JSObject& thisObject, const JSArgs& args);

    // This is called continuously from the app's main loop.
    virtual void OnUpdate() override;

    // This is called when the window is closing.
    virtual void OnClose(ultralight::Window* window) override;

    // This is called whenever the window resizes.
    virtual void OnResize(ultralight::Window* window, uint32_t width, uint32_t height) override;

    // This is called when the page finishes a load in one of its frames.
    virtual void OnFinishLoading(ultralight::View* caller,
        uint64_t frame_id,
        bool is_main_frame,
        const String& url) override;

    // This is called when the DOM has loaded in one of its frames.
    virtual void OnDOMReady(ultralight::View* caller,
        uint64_t frame_id,
        bool is_main_frame,
        const String& url) override;

    // This is called when the page requests to change the Cursor.
    virtual void OnChangeCursor(ultralight::View* caller,
        Cursor cursor) override;

    virtual void OnChangeTitle(ultralight::View* caller,
        const String& title) override;

    virtual void OnAddConsoleMessage(ultralight::View* caller,
        ultralight::MessageSource source,
        ultralight::MessageLevel level,
        const ultralight::String& message,
        uint32_t line_number,
        uint32_t column_number,
        const ultralight::String& source_id);
    // int chunkFileIntoWords(const std::string& filePath, int startChunk, int maxChunksToProcess, int chunkSize);
    void updateReaderContent(const double page);
    void translateNextChunk();
    void diagnoseFileStream(const std::string& context);
protected:
    RefPtr<App> app_;
    RefPtr<Window> window_;
    RefPtr<Overlay> overlay_;

private:
    std::streampos startPosition = 0;
    std::streampos endPosition = 0;
    std::streampos fileSize;
    std::ifstream fileStream;

    int maxChunk = 0;
    std::string currentPath = "default";
    int currentChunk = 0;
    size_t chunkSize = 4000;
    std::vector<std::string> pages;
    size_t numChunks = 1;
    bool endPage = false;
};
