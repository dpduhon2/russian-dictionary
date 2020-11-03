#ifndef WORDS_H
#define WORDS_H

#include <string>

class XMLNodeUTF8;

class Word {
public:
	enum class PartOfSpeech {
		noun,
		verb
	};

	Word(PartOfSpeech pos);

	const PartOfSpeech part_of_speech;
	std::wstring dictionary_form;
	std::wstring translation;

	void print_preview() const;

	virtual bool contains(const std::wstring& word) const = 0;
	virtual bool has_form(const std::wstring& word) const = 0;
	virtual bool translation_contains(const std::wstring& word) const = 0;

	virtual void print() const = 0;
	virtual void edit_entry() = 0;
	virtual XMLNodeUTF8 create_xml_node() const = 0;
};
bool operator<(const Word& w_1, const Word& w_2);

#endif