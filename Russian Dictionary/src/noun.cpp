#include "noun.h"

#include <iostream>
#include <iomanip>
#include "xml_node.h"

Noun::Noun() :
	Word{ PartOfSpeech::noun },
	gender{ Gender::neuter },
	nominative{ L"nominative" },
	accusative{ L"accusative" },
	genitive{ L"genitive" }
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
			nominative.update_from_node(attribute);
		else if (attribute.tag == L"accusative")
			accusative.update_from_node(attribute);
	}
}

bool Noun::contains(const std::wstring& word) const {
	return has_form(word) || translation_contains(word);
}
bool Noun::has_form(const std::wstring& word) const {
	return dictionary_form == word || nominative.contains(word) || accusative.contains(word);
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

	if (!(nominative.is_empty() && accusative.is_empty())) {
		std::wcout << L'\t' << std::setw(6) << L' ';
		std::wcout << std::left << std::setw(singular_column_width) << L"sing.";
		std::wcout << std::left << std::setw(plural_column_width) << L"plur." << std::endl;
	}
	
	if (!nominative.is_empty()) {
		std::wcout << L'\t';
		std::wcout << std::right << std::setw(6) << L"nom.  ";
		std::wcout << std::left << std::setw(singular_column_width) << nominative.singular;
		std::wcout << std::left << std::setw(plural_column_width) << nominative.plural << std::endl;
	}

	if (!accusative.is_empty()) {
		std::wcout << L'\t';
		std::wcout << std::right << std::setw(6) << L"acc.  ";
		std::wcout << std::left << std::setw(singular_column_width) << accusative.singular;
		std::wcout << std::left << std::setw(plural_column_width) << accusative.plural << std::endl;
	}
	
	if (!genitive.is_empty()) {
		std::wcout << L'\t';
		std::wcout << std::right << std::setw(6) << L"gen.  ";
		std::wcout << std::left << std::setw(singular_column_width) << genitive.singular;
		std::wcout << std::left << std::setw(plural_column_width) << genitive.plural << std::endl;
	}

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

	XMLNodeUTF8 n{ nominative.create_node() };
	if (n.children.size() != 0) entry.children.push_back(n);
	XMLNodeUTF8 a{ accusative.create_node() };
	if (a.children.size() != 0) entry.children.push_back(a);
	XMLNodeUTF8 g{ genitive.create_node() };
	if (g.children.size() != 0) entry.children.push_back(a);

	return entry;
}

Noun::Case::Case(const std::wstring& name_) :
	name{ name_ }
{}

void Noun::Case::print() const {
	return;
}
bool Noun::Case::is_empty() const {
	return singular == L"" && plural == L"";
}
bool Noun::Case::contains(const std::wstring& word) const {
	return singular == word || plural == word;
}

void Noun::Case::update_from_node(const XMLNodeUTF8& node) {
	for (const XMLNodeUTF8& form : node.children) {
		if (form.tag == L"singular")
			singular = form.contents;
		else if (form.tag == L"plural")
			plural = form.contents;
	}
	return;
}
XMLNodeUTF8 Noun::Case::create_node() const {
	XMLNodeUTF8 c{ name };

	if (singular != L"") {
		XMLNodeUTF8 s{ L"singular" };
		s.contents = singular;
		c.children.push_back(s);
	}
	if (plural != L"") {
		XMLNodeUTF8 p{ L"plural" };
		p.contents = plural;
		c.children.push_back(p);
	}

	return c;
}