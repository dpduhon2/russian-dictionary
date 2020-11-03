#include "unicode_io.h"

#include <iostream>
#include <cwctype>
#include <fcntl.h> // _O_U16TEXT
#include <io.h>    // _setmode()
#include <wchar.h>

void initialize_io() {
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin),  _O_U16TEXT);

	return;
}
bool get_yes_or_no() {
	while (true) {
		std::wstring input;
		std::getline(std::wcin, input);
		for (wchar_t& c : input) c = std::towlower(c);

		if (input == L"yes" || input == L"y" || input == L"\u0434" || input == L"\u0434\u0430")
			return true;
		else if (input == L"" || input == L"no" || input == L"n" || input == L"\u043d" || input == L"\u043d\u0435\u0442")
			return false;
		
		std::wcout << L"Enter yes or no. ";
	}
}

ifstreamUTF8::ifstreamUTF8(const std::string& filename) :
	file{ nullptr },
	buffer_size{ 1024 }
{
	file = std::fopen(filename.c_str(), "r");
	if (file != nullptr) _setmode(_fileno(file), _O_U8TEXT);
}
ifstreamUTF8::~ifstreamUTF8() {
	close();
}

void ifstreamUTF8::set_buffer_size(int size) {
	if (size > 1) buffer_size = size;
	return;
}

bool ifstreamUTF8::is_valid() {
	return (file != nullptr) && !(std::feof(file)) && !(std::ferror(file));
}
void ifstreamUTF8::close() {
	std::fclose(file);
	return;
}

ifstreamUTF8& ifstreamUTF8::operator>>(std::wstring& w) {
	wchar_t* s = (wchar_t*)std::malloc(buffer_size * sizeof(wchar_t));
	if (s == nullptr) return *this;

	std::fwscanf(file, L"%ls", s);

	if (s != nullptr) {
		w = std::wstring{ s };
		std::free(s);
	}

	return *this;
}

std::wstring ifstreamUTF8::getline() {
	wchar_t* s = (wchar_t*)std::malloc(buffer_size * sizeof(wchar_t));
	if (s == nullptr) return std::wstring{};

	std::fgetws(s, buffer_size, file);

	std::wstring w;
	if (s != nullptr) {
		w = std::wstring{ s };
		std::free(s);
	}

	return w;
}
wchar_t ifstreamUTF8::getc() {
	return std::fgetwc(file);
}

ofstreamUTF8::ofstreamUTF8(const std::string& filename, mode iomode_) :
	file{ nullptr },
	iomode{ iomode_ },
	buffer_size{ 1024 }
{
	switch (iomode) {
	case mode::replace:
		file = std::fopen(filename.c_str(), "w");
		break;
	case mode::append:
		file = std::fopen(filename.c_str(), "a");
		break;
	}
	if (file != nullptr) _setmode(_fileno(file), _O_U8TEXT);
}
ofstreamUTF8::~ofstreamUTF8() {
	close();
}

void ofstreamUTF8::set_buffer_size(int size) {
	if (size > 1) buffer_size = size;
	return;
}

bool ofstreamUTF8::is_valid() {
	return (file != nullptr) && !(std::feof(file)) && !(std::ferror(file));
}
void ofstreamUTF8::close() {
	std::fclose(file);
	return;
}

ofstreamUTF8& ofstreamUTF8::operator<<(const std::wstring& s) {
	fwprintf(file, L"%s", s.c_str());
	return *this;
}
ofstreamUTF8& ofstreamUTF8::operator<<(wchar_t c) {
	fwprintf(file, L"%c", c);
	return *this;
}
ofstreamUTF8& ofstreamUTF8::operator<<(int i) {
	fwprintf(file, L"%d", i);
	return *this;
}
ofstreamUTF8& ofstreamUTF8::operator<<(double d) {
	fwprintf(file, L"%lf", d);
	return *this;
}