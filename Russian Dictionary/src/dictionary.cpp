#include "dictionary.h"

#include "xml_node.h"
#include "unicode_io.h"
#include "noun.h"
#include "verb.h"

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
	XMLNodeUTF8 dictionary_node;
	dictionary_node.read_from_file(filename);

	if (dictionary_node.children.at(0).tag != L"dictionary") {
		throw std::exception("The given file does not contain a dictionary");
	}

	for (const XMLNodeUTF8 word : dictionary_node.children.at(0).children) {
		if (word.tag == L"noun") {
			Noun* entry = new Noun{ word };
			if (entry == nullptr) throw std::exception("Unable to create new noun");
			words.push_back(entry);
		}
		else if (word.tag == L"verb") {
			Verb* entry = new Verb{ word };
			if (entry == nullptr) throw std::exception("Unable to create new verb");
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