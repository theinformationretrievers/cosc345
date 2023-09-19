#include "MyApp.h"
#include "logging.h"
#include <Ultralight/Ultralight.h>

#include <iostream>


std::string ToUTF8(const ultralight::String& str)
{
    String8 utf8 = str.utf8();
    return std::string(utf8.data(), utf8.length());
}

const char* Stringify(ultralight::MessageSource source)
{
    switch (source) {
    case kMessageSource_XML:
        return "XML";
    case kMessageSource_JS:
        return "JS";
    case kMessageSource_Network:
        return "Network";
    case kMessageSource_ConsoleAPI:
        return "ConsoleAPI";
    case kMessageSource_Storage:
        return "Storage";
    case kMessageSource_AppCache:
        return "AppCache";
    case kMessageSource_Rendering:
        return "Rendering";
    case kMessageSource_CSS:
        return "CSS";
    case kMessageSource_Security:
        return "Security";
    case kMessageSource_ContentBlocker:
        return "ContentBlocker";
    case kMessageSource_Other:
        return "Other";
    default:
        return "";
    }
}

const char* Stringify(ultralight::MessageLevel level)
{
    switch (level) {
    case kMessageLevel_Log:
        return "Log";
    case kMessageLevel_Warning:
        return "Warning";
    case kMessageLevel_Error:
        return "Error";
    case kMessageLevel_Debug:
        return "Debug";
    case kMessageLevel_Info:
        return "Info";
    default:
        return "";
    }
}

//
// Inherited from ViewListener::OnAddConsoleMessage
//
// Make sure that you bind 'MyApp' to 'View::set_view_listener'
// to receive this event.
//
