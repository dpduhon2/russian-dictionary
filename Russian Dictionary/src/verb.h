#ifndef VERB_H
#define VERB_H

#include <string>
#include "word.h"

class XMLNodeUTF8;

class Verb : public Word {
public:
	struct Tense {
		std::wstring first_person_singular;
		std::wstring second_person_singular;
		std::wstring third_person_singular;
		std::wstring first_person_plural;
		std::wstring second_person_plural;
		std::wstring third_person_plural;
	};

	Verb();
	Verb(const XMLNodeUTF8& node);

	std::wstring infinitive;
	Tense present;

	bool contains(const std::wstring& word) const;
	bool has_form(const std::wstring& word) const;
	bool translation_contains(const std::wstring& word) const;

	void print() const;
	void edit_entry();
	XMLNodeUTF8 create_xml_node() const;

private:
	bool tense_contains(const Tense& tense, const std::wstring& word) const;

	void print_tense(const Tense& tense) const;
	void update_tense_from_xml_node(const XMLNodeUTF8& tense_node, Tense& tense);
	XMLNodeUTF8 create_tense_node(const Tense& tense, const std::wstring& name) const;
};

#endif