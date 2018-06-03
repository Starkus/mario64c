#pragma once

#include <vector>

std::vector<std::string> splitString(const char* input, char splitter) {
	std::vector<std::string> words;

	std::string word;
	int bufferPos = 0;

	char c;
	for (int i = 0;; ++i) {
		c = input[i];

		if (c == splitter) {
			words.push_back(word);
			word.clear();
		}
		else if (c == 0) {
			words.push_back(word);
			break;
		}
		else {
			word += c;
		}
	}

	return words;
}