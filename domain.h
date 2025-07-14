#pragma once

#include <string>

struct State
{
    std::string general;
};

bool SetState(std::string newValue);

State GetState();