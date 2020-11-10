#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <vector>
#include <list>
#include "word.h"

class XMLNodeUTF8;
class Noun;
class Verb;
class Adjective;

class Dictionary {
public:
	~Dictionary();

	void print(Word::PartOfSpeech part_of_speech) const;
	void print_words_containing(const std::wstring& word) const;
	void print_all() const;

	Noun& add_noun(const std::wstring& dictionary_form, const std::wstring& translation);
	Verb& add_verb(const std::wstring& dictionary_form, const std::wstring& translation);
	Adjective& add_adjective(const std::wstring& dictionary_form, const std::wstring& translation);

	std::vector<Word*> find_words_containing(const std::wstring& word);

	void remove_word(Word* w);

	void load_from_file(const std::string& filename);
	void save_to_file(const std::string& filename);

private:
	std::vector<Word*> words;
};

#endif