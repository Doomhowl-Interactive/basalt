#include <iostream>
#include <cassert>
#include <cstdarg>

#include <iostream>
#include <optional>
#include <vector>

#include "basalt_math.hpp"
#include "basalt_utils.hpp"
#include "basalt_console.hpp"

using namespace std;

// adapted from Raylib
string FormatText(const char* text, ...)
{
    constexpr int MAX_TEXT_LEN = 1024;
    constexpr int TEXT_BUFFER_COUNT = 16;

    assert(text);

    char buffer[MAX_TEXT_LEN];

    va_list args;
    va_start(args, text);
    vsnprintf(buffer, MAX_TEXT_LEN, text, args);

    return buffer;
}

string& TrimText(string buffer)
{
    buffer = buffer.substr(0, buffer.find_last_not_of(" \n\r\t") + 1);
    return buffer;
}

string& StripText(string buffer)
{
    string result = "";
    for (char c : buffer) {
        if (c != ' ' && c != '\n' && c != '\r' && c != '\t') {
            result += c;
        }
    }
    return result;
}

string& PadStringRight(string text, char symbol, size_t length)
{
    while (text.length() < length) {
        text += symbol;
    }
    return text;
}

string& PadStringLeft(string text, char symbol, size_t length)
{
    while (text.length() < length) {
        text = symbol + text;
    }
    return text;
}

string ConcatStrings(const vector<string> lines, char sep)
{
    string result;
    for (const auto& line : lines) {
        result += line + "\n";
    }
    result.pop_back();
    return result;
}
