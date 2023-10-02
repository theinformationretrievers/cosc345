#include "logging.h"


std::string ToUTF8(const ultralight::String& str)
{
    ultralight::String8 utf8 = str.utf8();
    return std::string(utf8.data(), utf8.length());
}

const char* Stringify(ultralight::MessageSource source)
{
    switch (source) {
    case ultralight::kMessageSource_XML:
        return "XML";
    case ultralight::kMessageSource_JS:
        return "JS";
    case ultralight::kMessageSource_Network:
        return "Network";
    case ultralight::kMessageSource_ConsoleAPI:
        return "ConsoleAPI";
    case ultralight::kMessageSource_Storage:
        return "Storage";
    case ultralight::kMessageSource_AppCache:
        return "AppCache";
    case ultralight::kMessageSource_Rendering:
        return "Rendering";
    case ultralight::kMessageSource_CSS:
        return "CSS";
    case ultralight::kMessageSource_Security:
        return "Security";
    case ultralight::kMessageSource_ContentBlocker:
        return "ContentBlocker";
    case ultralight::kMessageSource_Other:
        return "Other";
    default:
        return "";
    }
}

const char* Stringify(ultralight::MessageLevel level)
{
    switch (level) {
    case ultralight::kMessageLevel_Log:
        return "Log";
    case ultralight::kMessageLevel_Warning:
        return "Warning";
    case ultralight::kMessageLevel_Error:
        return "Error";
    case ultralight::kMessageLevel_Debug:
        return "Debug";
    case ultralight::kMessageLevel_Info:
        return "Info";
    default:
        return "";
    }
}
