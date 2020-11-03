#include "noun.h"

#include <iostream>
#include <iomanip>
#include "xml_node.h"

Noun::Noun() :
	Word{ PartOfSpeech::noun },
	gender{ Gender::neuter }
{}
Noun::Noun(const XMLNodeUTF8& node) : Noun{} {
	for (const XMLNodeUTF8& attribute : node.children) {
		if (attribute.tag == L"dictionary-form")
			dictionary_form = attribute.contents;
		else if (attribute.tag == L"translation")
			translation = attribute.contents;
		else if (attribute.tag == L"gender") {
			if (attribute.contents == L"masculine")
				gender = Noun::Gender::masculine;
			else if (attribute.contents == L"feminine")
				gender = Noun::Gender::feminine;
			else if (attribute.contents == L"neuter")
				gender = Noun::Gender::neuter;
		}
		else if (attribute.tag == L"nominative")
			update_case_from_xml_node(attribute, nominative);
		else if (attribute.tag == L"accusative")
			update_case_from_xml_node(attribute, accusative);
	}
}

bool Noun::contains(const std::wstring& word) const {
	return has_form(word) || translation_contains(word);
}
bool Noun::has_form(const std::wstring& word) const {
	return dictionary_form == word || case_contains(nominative, word) || case_contains(accusative, word);
}
bool Noun::translation_contains(const std::wstring& word) const {
	return translation.find(word) != std::wstring::npos;
}

void Noun::print() const {
	std::wcout << std::left << std::setw(24) << std::setfill(L'-') << dictionary_form << std::setfill(L' ') << std::endl;

	std::wcout << L'\t' << translation << std::endl;
	std::wcout << std::endl;

	int default_width = 6;

	int singular_column_width = default_width;
	if (nominative.singular.length() + 2 > singular_column_width) singular_column_width = nominative.singular.length() + 2;
	if (accusative.singular.length() + 2 > singular_column_width) singular_column_width = accusative.singular.length() + 2;

	int plural_column_width = default_width;
	if (nominative.plural.length() + 2 > singular_column_width) singular_column_width = nominative.plural.length() + 2;
	if (accusative.plural.length() + 2 > singular_column_width) singular_column_width = accusative.plural.length() + 2;

	std::wcout << L'\t';
	switch (gender) {
	case Gender::masculine:
		std::wcout << L"masculine";
		break;
	case Gender::feminine:
		std::wcout << L"feminine";
		break;
	case Gender::neuter:
		std::wcout << L"neuter";
		break;
	}
	std::wcout << std::endl;
	std::wcout << std::endl;

	std::wcout << L'\t' << std::setw(6) << L' ';
	std::wcout << std::left << std::setw(singular_column_width) << L"sing.";
	std::wcout << std::left << std::setw(plural_column_width) << L"plur." << std::endl;

	std::wcout << L'\t';
	std::wcout << std::right << std::setw(6) << L"nom.  ";
	std::wcout << std::left << std::setw(singular_column_width) << nominative.singular;
	std::wcout << std::left << std::setw(plural_column_width) << nominative.plural << std::endl;

	std::wcout << L'\t';
	std::wcout << std::right << std::setw(6) << L"acc.  ";
	std::wcout << std::left << std::setw(singular_column_width) << accusative.singular;
	std::wcout << std::left << std::setw(plural_column_width) << accusative.plural << std::endl;

	return;
}
void Noun::edit_entry() {
	return;
}
XMLNodeUTF8 Noun::create_xml_node() const {
	XMLNodeUTF8 entry{ L"noun" };

	if (dictionary_form != L"") {
		XMLNodeUTF8 d{ L"dictionary-form" };
		d.contents = dictionary_form;
		entry.children.push_back(d);
	}
	if (translation != L"") {
		XMLNodeUTF8 t{ L"translation" };
		t.contents = translation;
		entry.children.push_back(t);
	}

	XMLNodeUTF8 g{ L"gender" };
	switch (gender) {
	case Gender::masculine:
		g.contents = L"masculine";
		break;
	case Gender::feminine:
		g.contents = L"feminine";
		break;
	case Gender::neuter:
		g.contents = L"neuter";
		break;
	}
	entry.children.push_back(g);

	XMLNodeUTF8 n{ create_case_node(nominative, L"nominative") };
	if (n.children.size() != 0) entry.children.push_back(n);
	XMLNodeUTF8 a{ create_case_node(accusative, L"accusative") };
	if (a.children.size() != 0) entry.children.push_back(a);

	return entry;
}

bool Noun::case_contains(const Case& c, const std::wstring& word) const {
	return c.singular == word || c.plural == word;
}

void Noun::update_case_from_xml_node(const XMLNodeUTF8& case_node, Case& noun_case) {
	for (const XMLNodeUTF8& form : case_node.children) {
		if (form.tag == L"singular")
			noun_case.singular = form.contents;
		else if (form.tag == L"plural")
			noun_case.plural = form.contents;
	}
	return;
}
XMLNodeUTF8 Noun::create_case_node(const Case& case_, const std::wstring& name) const {
	XMLNodeUTF8 c{ name };

	if (case_.singular != L"") {
		XMLNodeUTF8 s{ L"singular" };
		s.contents = case_.singular;
		c.children.push_back(s);
	}
	if (case_.plural != L"") {
		XMLNodeUTF8 p{ L"plural" };
		p.contents = case_.plural;
		c.children.push_back(p);
	}

	return c;
}