#ifndef NOUN_H
#define NOUN_H

#include <string>
#include "word.h"

class XMLNodeUTF8;

class Noun : public Word {
public:
	enum class Gender {
		masculine,
		feminine,
		neuter
	};
	struct Case {
		std::wstring singular;
		std::wstring plural;
	};

	Noun();
	Noun(const XMLNodeUTF8& node);

	Gender gender;
	Case nominative;
	Case accusative;

	bool contains(const std::wstring& word) const;
	bool has_form(const std::wstring& word) const;
	bool translation_contains(const std::wstring& word) const;

	void print() const;
	void edit_entry();
	XMLNodeUTF8 create_xml_node() const;

private:
	bool case_contains(const Case& c, const std::wstring& word) const;

	void update_case_from_xml_node(const XMLNodeUTF8& case_node, Case& noun_case);
	XMLNodeUTF8 create_case_node(const Case& c, const std::wstring& name) const;
};

#endif