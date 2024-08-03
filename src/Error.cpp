#include "Error.h"

#include <iostream>

auto printError(const char* file, unsigned int line, const char* function,
                const char* what, const char* details) noexcept -> void {
	std::cerr << "ERROR: \'" << what << "\'\n";
	std::cerr << "\tIn " << function << '\n';
	std::cerr << '\t' << file << ", line " << line << '\n';

	if (details != nullptr) {
		std::cerr << details << '\n';
	}
}

