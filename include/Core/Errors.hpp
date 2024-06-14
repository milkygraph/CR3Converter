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

struct LibRawError : Error {
	explicit LibRawError(const std::string& message) {
		Error::operator()("LibRaw Error: " + message);
	}
};

struct GUIError : Error {
	explicit GUIError(const std::string& message) {
		Error::operator()("GUI Error: " + message);
	}
};