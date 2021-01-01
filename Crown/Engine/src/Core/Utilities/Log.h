#pragma once

#include <iostream>

// Engine log macros
#define LOG_ENGINE(...)	std::cout << __VA_ARGS__ << std::endl;
#define LOG_ENGINE_WARNING(...)	std::cout << __VA_ARGS__ << std::endl;
#define LOG_ENGINE_ERROR(...)	std::cout << __VA_ARGS__ << std::endl;

// Application log macros
#define LOG(...)	std::cout << __VA_ARGS__ << std::endl;
#define LOG_WARNING(...)	std::cout << __VA_ARGS__ << std::endl;
#define LOG_ERROR(...)	std::cout << __VA_ARGS__ << std::endl;