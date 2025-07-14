#pragma once

#include <string>
#include <iostream>
#include <cstring>

using namespace std;

/// @brief Вывод сообщения об ошибке с описанием.
/// @param message Текст сообщения.
/// @param exitProgram Необходимо ли закрыть программу.
void handleError(const string& message, bool exitProgram = true) 
{
    cerr << "Error: " << message << " (" << strerror(errno) << ")" << endl;
    if (exitProgram) 
        exit(EXIT_FAILURE);
}