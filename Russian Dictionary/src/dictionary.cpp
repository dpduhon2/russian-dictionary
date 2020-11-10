#include "dictionary.h"

#include "xml_node.h"
#include "unicode_io.h"
#include "noun.h"
#include "verb.h"
#include "adjective.h"

Dictionary::~Dictionary() {
	for (Word* w : words) {
		delete w;
	}
}

Noun& Dictionary::add_noun(const std::wstring& dictionary_form, const std::wstring& translation) {
	Noun* noun = new Noun;
	noun->dictionary_form = dictionary_form;
	noun->translation = translation;

	words.push_back(noun);

	return *noun;
}
Verb& Dictionary::add_verb(const std::wstring& dictionary_form, const std::wstring& translation) {
	Verb* verb = new Verb;
	verb->dictionary_form = dictionary_form;
	verb->translation = translation;

	words.push_back(verb);

	return *verb;
}
Adjective& Dictionary::add_adjective(const std::wstring& dictionary_form, const std::wstring& translation) {
	Adjective* adjective = new Adjective;
	adjective->dictionary_form = dictionary_form;
	adjective->translation = translation;

	words.push_back(adjective);

	return *adjective;
}

std::vector<Word*> Dictionary::find_words_containing(const std::wstring& word) {
	std::vector<Word*> matching_words;
	for (Word* w : words) {
		if (w->contains(word)) {
			matching_words.push_back(w);
		}
	}
	return matching_words;
}

void Dictionary::remove_word(Word* word) {
	for (auto i = words.begin(); i != words.end(); ++i) {
		if (*i == word) {
			delete *i;
			words.erase(i);
			return;
		}
	}
	return;
}

void Dictionary::print_all() const {
	for (Word* w : words) {
		w->print();
	}
	return;
}
void Dictionary::print_words_containing(const std::wstring& word) const {
	for (Word* w : words) {
		if (w->contains(word)) {
			w->print_preview();
		}
	}
	return;
}
void Dictionary::print(Word::PartOfSpeech part_of_speech) const  {
	for (Word* w : words) {
		if (w->part_of_speech == part_of_speech) {
			w->print();
		}
	}
	return;
}

void Dictionary::load_from_file(const std::string& filename) {
	XMLNodeUTF8 file_node;
	file_node.read_from_file(filename);

	XMLNodeUTF8 dictionary_node;
	if (file_node.children.size() != 0) dictionary_node = file_node.children.at(0);

	if (dictionary_node.tag != L"dictionary") {
		throw std::exception("The given file does not contain a dictionary");
	}

	for (const XMLNodeUTF8 word : dictionary_node.children) {
		if (word.tag == L"noun") {
			Noun* entry = new Noun;
			if (entry == nullptr) throw std::exception("Unable to create new noun");

			entry->update_from_xml_node(word);
			words.push_back(entry);
		}
		else if (word.tag == L"verb") {
			Verb* entry = new Verb;
			if (entry == nullptr) throw std::exception("Unable to create new verb");

			entry->update_from_xml_node(word);
			words.push_back(entry);
		}
		else if (word.tag == L"adjective") {
			Adjective* entry = new Adjective;
			if (entry == nullptr) throw std::exception("Unable to create new adjective");

			entry->update_from_xml_node(word);
			words.push_back(entry);
		}
		else throw std::exception("Unrecognized part of speech");
	}

	return;
}
void Dictionary::save_to_file(const std::string& filename) {
	XMLNodeUTF8 dictionary_node{ L"dictionary" };
	for (Word* w : words) {
		dictionary_node.children.push_back(w->create_xml_node());
	}
	dictionary_node.write_to_file(filename);
	return;
}