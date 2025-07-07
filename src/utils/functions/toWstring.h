#pragma once

#include <sstream>
#include <string>
#include <type_traits>

// Template function to convert any numeric type to std::wstring
template <typename T>
std::wstring ToWString(const T &value)
{
    // Ensure the type is numeric (integral or floating point)
    static_assert(std::is_arithmetic<T>::value, "ToWString requires a numeric type");

    std::wostringstream wss;
    wss << value; // Convert the value to wide string
    return wss.str();
}

// Optional specialization for std::string
std::wstring ToWString(const std::string &str)
{
    // Convert std::string to std::wstring by copying characters
    return std::wstring(str.begin(), str.end());
}