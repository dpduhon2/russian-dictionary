#ifndef MENU_UTF8_H
#define MENU_UTF8_H

#include <string>
#include <vector>

class MenuUTF8 {
public:
	void add_option(wchar_t option, const std::wstring& description);
	char get_user_choice() const;
private:
	std::vector<std::pair<wchar_t, std::wstring>> options;
};

#endif