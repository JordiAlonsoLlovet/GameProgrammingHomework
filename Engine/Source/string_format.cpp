#pragma once
#include "Globals.h"
#include <memory>
#include <string>
#include <stdexcept>

template<typename ... Args>
char* string_format(const char* format, Args ... args)
{
    int size_s = std::snprintf(nullptr, 0, format, args ...) + 1; // Extra space for '\0'
    if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
    auto size = static_cast<size_t>(size_s);
    std::unique_ptr<char[]> buf(new char[size]);
    std::snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1).c_str(); // We don't want the '\0' inside
}