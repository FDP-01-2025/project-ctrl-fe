#pragma once

#include <sstream>
#include <string>
#include <type_traits>

// Función template para convertir cualquier tipo numérico a wstring
template<typename T>
std::wstring ToWString(const T& value) {
    // Verifica que el tipo sea numérico (entero o flotante)
    static_assert(std::is_arithmetic<T>::value, "ToWString requires a numeric type");

    std::wostringstream wss;
    wss << value;  // Convierte el valor a wide string
    return wss.str();
}

// Especialización para std::string (opcional)
std::wstring ToWString(const std::string& str) {
    return std::wstring(str.begin(), str.end());
}