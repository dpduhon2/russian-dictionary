#include "noun.h"

#include <iostream>
#include <iomanip>
#include "xml_node.h"

Noun::Noun() :
	Word{ PartOfSpeech::noun },
	gender{ Gender::neuter },
	nominative{ L"nominative" },
	accusative{ L"accusative" },
	genitive{ L"genitive" },
	prepositional{ L"prepositional" }
{}

void Noun::print() const {
	std::wcout << std::left << std::setw(24) << std::setfill(L'-') << dictionary_form << std::setfill(L' ') << std::endl;

	std::wcout << L'\t' << translation << std::endl;
	std::wcout << std::endl;

	int default_width = 6;

	int singular_column_width = default_width;
	if (nominative.singular.length()    + 2 > singular_column_width) singular_column_width = nominative.singular.length()    + 2;
	if (accusative.singular.length()    + 2 > singular_column_width) singular_column_width = accusative.singular.length()    + 2;
	if (genitive.singular.length()      + 2 > singular_column_width) singular_column_width = genitive.singular.length()      + 2;
	if (prepositional.singular.length() + 2 > singular_column_width) singular_column_width = prepositional.singular.length() + 2;

	int plural_column_width = default_width;
	if (nominative.plural.length()    + 2 > singular_column_width) singular_column_width = nominative.plural.length()    + 2;
	if (accusative.plural.length()    + 2 > singular_column_width) singular_column_width = accusative.plural.length()    + 2;
	if (genitive.plural.length()      + 2 > singular_column_width) singular_column_width = genitive.plural.length()      + 2;
	if (prepositional.plural.length() + 2 > singular_column_width) singular_column_width = prepositional.plural.length() + 2;

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

	if (!(nominative.is_empty() && accusative.is_empty() && genitive.is_empty() && prepositional.is_empty())) {
		std::wcout << L'\t' << std::setw(8) << L' ';
		std::wcout << std::left << std::setw(singular_column_width) << L"sing.";
		std::wcout << std::left << std::setw(plural_column_width) << L"plur." << std::endl;
	}
	
	if (!nominative.is_empty()) {
		std::wcout << L'\t';
		std::wcout << std::right << std::setw(8) << L"nom.  ";
		std::wcout << std::left << std::setw(singular_column_width) << nominative.singular;
		std::wcout << std::left << std::setw(plural_column_width) << nominative.plural << std::endl;
	}

	if (!accusative.is_empty()) {
		std::wcout << L'\t';
		std::wcout << std::right << std::setw(8) << L"acc.  ";
		std::wcout << std::left << std::setw(singular_column_width) << accusative.singular;
		std::wcout << std::left << std::setw(plural_column_width) << accusative.plural << std::endl;
	}
	
	if (!genitive.is_empty()) {
		std::wcout << L'\t';
		std::wcout << std::right << std::setw(8) << L"gen.  ";
		std::wcout << std::left << std::setw(singular_column_width) << genitive.singular;
		std::wcout << std::left << std::setw(plural_column_width) << genitive.plural << std::endl;
	}
	
	if (!prepositional.is_empty()) {
		std::wcout << L'\t';
		std::wcout << std::right << std::setw(8) << L"prep.  ";
		std::wcout << std::left << std::setw(singular_column_width) << prepositional.singular;
		std::wcout << std::left << std::setw(plural_column_width) << prepositional.plural << std::endl;
	}

	return;
}
bool Noun::contains(const std::wstring& word) const {
	return has_form(word) || translation_contains(word);
}
bool Noun::has_form(const std::wstring& word) const {
	return dictionary_form == word
		|| nominative.contains(word)
		|| accusative.contains(word)
		|| genitive.contains(word)
		|| prepositional.contains(word);
}

void Noun::update_from_xml_node(const XMLNodeUTF8& node) {
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
			nominative.update_from_xml_node(attribute);
		else if (attribute.tag == L"accusative")
			accusative.update_from_xml_node(attribute);
		else if (attribute.tag == L"genitive")
			genitive.update_from_xml_node(attribute);
		else if (attribute.tag == L"prepositional")
			prepositional.update_from_xml_node(attribute);
	}
}
XMLNodeUTF8 Noun::create_xml_node() const {
	XMLNodeUTF8 entry;

	entry.tag = L"noun";

	if (dictionary_form != L"") {
		XMLNodeUTF8 dictionary_form_node;
		dictionary_form_node.tag = L"dictionary-form";
		dictionary_form_node.contents = dictionary_form;
		entry.children.push_back(dictionary_form_node);
	}
	if (translation != L"") {
		XMLNodeUTF8 translation_node;
		translation_node.tag = L"translation";
		translation_node.contents = translation;
		entry.children.push_back(translation_node);
	}

	XMLNodeUTF8 gender_node;
	gender_node.tag = L"gender";
	switch (gender) {
	case Gender::masculine:
		gender_node.contents = L"masculine";
		break;
	case Gender::feminine:
		gender_node.contents = L"feminine";
		break;
	case Gender::neuter:
		gender_node.contents = L"neuter";
		break;
	}
	entry.children.push_back(gender_node);

	if (!nominative.is_empty())
		entry.children.push_back(nominative.create_xml_node());
	if (!accusative.is_empty())
		entry.children.push_back(accusative.create_xml_node());
	if (!genitive.is_empty())
		entry.children.push_back(genitive.create_xml_node());
	if (!prepositional.is_empty())
		entry.children.push_back(prepositional.create_xml_node());

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

void Noun::Case::update_from_xml_node(const XMLNodeUTF8& node) {
	for (const XMLNodeUTF8& form : node.children) {
		if (form.tag == L"singular")
			singular = form.contents;
		else if (form.tag == L"plural")
			plural = form.contents;
	}
	return;
}
XMLNodeUTF8 Noun::Case::create_xml_node() const {
	XMLNodeUTF8 case_node;

	case_node.tag = name;
	if (singular != L"") {
		XMLNodeUTF8 singular_node;
		singular_node.tag = L"singular";
		singular_node.contents = singular;
		case_node.children.push_back(singular_node);
	}
	if (plural != L"") {
		XMLNodeUTF8 plural_node;
		plural_node.tag = L"plural";
		plural_node.contents = plural;
		case_node.children.push_back(plural_node);
	}

	return case_node;
}