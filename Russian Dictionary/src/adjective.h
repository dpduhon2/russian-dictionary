#ifndef ADJECTIVE_H
#define ADJECTIVE_H

#include <string>
#include "word.h"

class XMLNodeUTF8;

class Adjective : public Word {
public:
	class Case {
	public:
		Case(const std::wstring& name_);

		std::wstring masculine;
		std::wstring feminine;
		std::wstring neuter;
		std::wstring plural;

		void print() const;
		bool is_empty() const;
		bool contains(const std::wstring& word) const;

		void update_from_xml_node(const XMLNodeUTF8& node);
		XMLNodeUTF8 create_xml_node() const;

	private:
		std::wstring name;
	};

	Adjective();

	Case nominative;
	Case accusative;

	void print() const;
	bool contains(const std::wstring& word) const;
	bool has_form(const std::wstring& word) const;

	void update_from_xml_node(const XMLNodeUTF8& node);
	XMLNodeUTF8 create_xml_node() const;
};

#endif