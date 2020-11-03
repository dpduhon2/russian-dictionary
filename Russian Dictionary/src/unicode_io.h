#ifndef UNICODE_IO
#define UNICODE_IO

#pragma warning(disable:4996)

#include <stdio.h>
#include <string>

void initialize_io();
bool get_yes_or_no();

class ifstreamUTF8 {
public:
	ifstreamUTF8(const std::string& filename);
	~ifstreamUTF8();

	void set_buffer_size(int size);

	bool is_valid();
	void close();  // will close automatically at destruction time

	ifstreamUTF8& operator>>(std::wstring& s);

	std::wstring getline();
	wchar_t getc();

private:
	FILE* file;
	int buffer_size;
};

class ofstreamUTF8 {
public:
	enum class mode {
		replace,
		append
	};

	ofstreamUTF8(const std::string& filename, mode iomode_);
	~ofstreamUTF8();

	void set_buffer_size(int size);

	bool is_valid();
	void close();  // will close automatically at destruction time

	ofstreamUTF8& operator<<(const std::wstring& s);
	ofstreamUTF8& operator<<(wchar_t c);
	ofstreamUTF8& operator<<(int i);
	ofstreamUTF8& operator<<(double d);

private:
	FILE* file;
	mode iomode;
	int buffer_size;
};

#endif