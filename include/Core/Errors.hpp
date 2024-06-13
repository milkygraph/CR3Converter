#pragma once
#include <iostream>
#include <string>

struct Error {
	void operator()(const std::string& message) {
		std::cerr << message << std::endl;
	}
};

struct WindowError : Error {
	explicit WindowError(const std::string& message) {
		Error::operator()("Window Error: " + message);
	}
};