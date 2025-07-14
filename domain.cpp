#include "domain.h"
#include <string>

using namespace std;

/// @brief Текущее состояние программы. 
/// (Задаётся при старте программы через первый аргумент.)
State currentState;

/// @brief Задает состояние программы (значение currentState).
/// @param newValue Новое значение состояния currentState.
/// @return Является ли операция успешной.
bool SetState(string newValue)
{
    if(newValue != "Normal" && newValue != "Failed")
        return false;
    currentState.general = newValue;
    return true;
}

/// @brief Получает заданное при старте программы состояние.
/// @return Текущее значение currentState.
State GetState()
{
    return currentState;
}