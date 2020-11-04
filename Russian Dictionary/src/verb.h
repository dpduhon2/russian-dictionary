#ifndef VERB_H
#define VERB_H

#include <string>
#include "word.h"

class XMLNodeUTF8;

class Verb : public Word {
public:
	class Tense {
	public:
		Tense(const std::wstring& name_);

		std::wstring first_person_singular;
		std::wstring second_person_singular;
		std::wstring third_person_singular;
		std::wstring first_person_plural;
		std::wstring second_person_plural;
		std::wstring third_person_plural;

		void print() const;
		bool is_empty() const;
		bool contains(const std::wstring& word) const;

		void update_from_node(const XMLNodeUTF8& node);
		XMLNodeUTF8 create_node() const;

	private:
		std::wstring name;
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
};

#endif