#include "Error.h"

#include <iostream>

auto PrintError(const char* file, unsigned int line, const char* function, const char* what, const char* extra_print) -> void {
	std::cerr << "ERROR: \'" << what << "\'\n";
	std::cerr << "\tIn " << function << '\n';
	std::cerr << '\t' << file << ", line " << line << '\n';

	if (extra_print != nullptr) {
		std::cerr << extra_print << '\n';
	}
}

