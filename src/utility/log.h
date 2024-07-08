#pragma once

#include <iostream>
#include <string>

#include "nameof.h"

void Log(const char* msg);
void Log(const std::string msg);
void Log(const int& msg);
void Log(const long& msg);
void Log(const float& msg);
void Log(const double& msg);
