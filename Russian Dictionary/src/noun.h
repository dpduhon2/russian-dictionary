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
	class Case {
	public:
		Case(const std::wstring& name_);

		std::wstring singular;
		std::wstring plural;

		void print() const;
		bool is_empty() const;
		bool contains(const std::wstring& word) const;

		void update_from_node(const XMLNodeUTF8& node);
		XMLNodeUTF8 create_node() const;

	private:
		std::wstring name;
	};

	Noun();
	Noun(const XMLNodeUTF8& node);

	Gender gender;
	Case nominative;
	Case accusative;
	Case genitive;

	bool contains(const std::wstring& word) const;
	bool has_form(const std::wstring& word) const;

	void print() const;
	XMLNodeUTF8 create_xml_node() const;
};

#endif