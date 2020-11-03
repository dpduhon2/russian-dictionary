#include "menu.h"

#include <iostream>
#include <stdexcept>
#include "unicode_io.h"

void MenuUTF8::add_option(wchar_t option, const std::wstring& description) {
	if (!isalnum(option)) throw std::exception("Options must be alphanumeric");

	for (const auto& p : options) {
		if (p.first == option) {
			throw std::exception("Cannot add the same option multiple times");
		}
	}
	options.emplace_back(option, description);
	
	return;
}
char MenuUTF8::get_user_choice() const {
	if (options.size() < 2) throw std::exception("Too few options to present to user");

	std::wcout << L"Enter";
	for (const auto& option : options) {
		std::wcout << L'\t' << option.first << L' ' << option.second << std::endl;
	}

	wchar_t user_choice;
	while (true) {
		std::wstring input;
		std::wcout << ">> ";
		std::getline(std::wcin, input);
		user_choice = (input.size() != 1) ? L' ' : input.at(0);

		for (const auto& option : options) {
			if (user_choice == option.first) {
				std::wcout << std::endl;
				return user_choice;
			}
		};

		std::wcout << L"Enter ";
		if (options.size() == 2) {
			std::wcout << options.at(0).first << L" or " << options.at(1).second << std::endl;
		}
		else {
			for (auto option = options.begin(); option != options.end() - 1; ++option)
				std::wcout << option->first << L", ";
			std::wcout << L"or " << options.back().first << std::endl;
		}
	}
}