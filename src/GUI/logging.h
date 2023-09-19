#include <Ultralight/Ultralight.h>

#include <iostream>

std::string ToUTF8(const ultralight::String& str);
const char* Stringify(ultralight::MessageSource source);
const char* Stringify(ultralight::MessageLevel level);