#include "adjective.h"

#include <iostream>
#include <iomanip>
#include "xml_node.h"

Adjective::Adjective() :
	Word{ Word::PartOfSpeech::adjective },
	nominative{ L"nominative" },
	accusative{ L"accusative" }
{}

void Adjective::print() const {
	std::wcout << std::left << std::setw(24) << std::setfill(L'-') << dictionary_form << std::setfill(L' ') << std::endl;

	std::wcout << L'\t' << translation << std::endl;
	std::wcout << std::endl;

	int default_width = 6;

	int masculine_column_width = default_width;
	if (nominative.masculine.length() + 2 > masculine_column_width) masculine_column_width = nominative.masculine.length() + 2;
	if (accusative.masculine.length() + 2 > masculine_column_width) masculine_column_width = accusative.masculine.length() + 2;

	int feminine_column_width = default_width;
	if (nominative.feminine.length() + 2 > feminine_column_width) feminine_column_width = nominative.feminine.length() + 2;
	if (accusative.feminine.length() + 2 > feminine_column_width) feminine_column_width = accusative.feminine.length() + 2;

	int neuter_column_width = default_width;
	if (nominative.neuter.length() + 2 > neuter_column_width) neuter_column_width = nominative.neuter.length() + 2;
	if (accusative.neuter.length() + 2 > neuter_column_width) neuter_column_width = accusative.neuter.length() + 2;

	int plural_column_width = default_width;
	if (nominative.plural.length() + 2 > plural_column_width) plural_column_width = nominative.plural.length() + 2;
	if (accusative.plural.length() + 2 > plural_column_width) plural_column_width = accusative.plural.length() + 2;

	if (!(nominative.is_empty() && accusative.is_empty())) {
		std::wcout << L'\t' << std::setw(8) << L' ';
		std::wcout << std::left << std::setw(masculine_column_width) << L"masc.";
		std::wcout << std::left << std::setw(feminine_column_width) << L"fem.";
		std::wcout << std::left << std::setw(neuter_column_width) << L"neut.";
		std::wcout << std::left << std::setw(plural_column_width) << L"plur." << std::endl;
	}

	if (!nominative.is_empty()) {
		std::wcout << L'\t';
		std::wcout << std::right << std::setw(8) << L"nom.  ";
		std::wcout << std::left << std::setw(masculine_column_width) << nominative.masculine;
		std::wcout << std::left << std::setw(feminine_column_width)  << nominative.feminine;
		std::wcout << std::left << std::setw(neuter_column_width)    << nominative.neuter;
		std::wcout << std::left << std::setw(plural_column_width)    << nominative.plural << std::endl;
	}

	if (!accusative.is_empty()) {
		std::wcout << L'\t';
		std::wcout << std::right << std::setw(8) << L"acc.  ";
		std::wcout << std::left << std::setw(masculine_column_width) << accusative.masculine;
		std::wcout << std::left << std::setw(feminine_column_width)  << accusative.feminine;
		std::wcout << std::left << std::setw(neuter_column_width)    << accusative.neuter;
		std::wcout << std::left << std::setw(plural_column_width)    << accusative.plural << std::endl;
	}

	return;
}
bool Adjective::contains(const std::wstring& word) const {
	return has_form(word) || translation_contains(word);
}
bool Adjective::has_form(const std::wstring& word) const {
	return dictionary_form == word
		|| nominative.contains(word)
		|| accusative.contains(word);
}

void Adjective::update_from_xml_node(const XMLNodeUTF8& node) {
	for (const XMLNodeUTF8& attribute : node.children) {
		if (attribute.tag == L"dictionary-form")
			dictionary_form = attribute.contents;
		else if (attribute.tag == L"translation")
			translation = attribute.contents;
		else if (attribute.tag == L"nominative")
			nominative.update_from_xml_node(attribute);
		else if (attribute.tag == L"accusative")
			accusative.update_from_xml_node(attribute);
	}
	return;
}
XMLNodeUTF8 Adjective::create_xml_node() const {
	XMLNodeUTF8 entry;

	entry.tag = L"adjective";

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

	if (!nominative.is_empty())
		entry.children.push_back(nominative.create_xml_node());
	if (!accusative.is_empty())
		entry.children.push_back(accusative.create_xml_node());

	return entry;
}

Adjective::Case::Case(const std::wstring& name_) :
	name{ name_ }
{}

void Adjective::Case::print() const {
	std::wcout << name << std::endl;
	std::wcout << '\t' << L"masculine: " << masculine << std::endl;
	std::wcout << '\t' << L"feminine: "  << feminine  << std::endl;
	std::wcout << '\t' << L"neuter: "    << neuter    << std::endl;
	std::wcout << '\t' << L"plural: "    << plural    << std::endl;

	return;
}
bool Adjective::Case::is_empty() const {
	return masculine == L""
		&& feminine == L""
		&& neuter == L""
		&& plural == L"";
}
bool Adjective::Case::contains(const std::wstring& word) const {
	return masculine == word
		|| feminine == word
		|| neuter == word
		|| plural == word;
}

void Adjective::Case::update_from_xml_node(const XMLNodeUTF8& node) {
	for (const XMLNodeUTF8& form : node.children) {
		if (form.tag == L"masculine")
			masculine = form.contents;
		else if (form.tag == L"feminine")
			feminine = form.contents;
		else if (form.tag == L"neuter")
			neuter = form.contents;
		else if (form.tag == L"plural")
			plural = form.contents;
	}
	return;
}
XMLNodeUTF8 Adjective::Case::create_xml_node() const {
	XMLNodeUTF8 case_node;

	case_node.tag = name;
	if (masculine != L"") {
		XMLNodeUTF8 masculine_node;
		masculine_node.tag = L"masculine";
		masculine_node.contents = masculine;
		case_node.children.push_back(masculine_node);
	}
	if (feminine != L"") {
		XMLNodeUTF8 feminine_node;
		feminine_node.tag = L"feminine";
		feminine_node.contents = feminine;
		case_node.children.push_back(feminine_node);
	}
	if (neuter != L"") {
		XMLNodeUTF8 neuter_node;
		neuter_node.tag = L"neuter";
		neuter_node.contents = neuter;
		case_node.children.push_back(neuter_node);
	}
	if (plural != L"") {
		XMLNodeUTF8 plural_node;
		plural_node.tag = L"plural";
		plural_node.contents = plural;
		case_node.children.push_back(plural_node);
	}

	return case_node;
}