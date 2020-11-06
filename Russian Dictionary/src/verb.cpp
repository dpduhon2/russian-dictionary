#include "verb.h"

#include <iostream>
#include <iomanip>
#include "xml_node.h"

Verb::Verb() :
	Word{ PartOfSpeech::verb },
	present{ L"present" }
{}

void Verb::print() const {
	std::wcout << std::left << std::setw(24) << std::setfill(L'-') << dictionary_form << std::setfill(L' ') << std::endl;

	std::wcout << L'\t' << translation << std::endl;
	std::wcout << std::endl;

	std::wcout << L"\tpresent" << std::endl;
	present.print();

	return;
}
bool Verb::contains(const std::wstring& word) const {
	return has_form(word) || translation_contains(word);
}
bool Verb::has_form(const std::wstring& word) const {
	return dictionary_form == word || infinitive == word || present.contains(word);
}

void Verb::update_from_xml_node(const XMLNodeUTF8& node) {
	for (const XMLNodeUTF8& attribute : node.children) {
		if (attribute.tag == L"dictionary-form")
			dictionary_form = attribute.contents;
		else if (attribute.tag == L"translation")
			translation = attribute.contents;
		else if (attribute.tag == L"infinitive")
			infinitive = attribute.contents;
		else if (attribute.tag == L"present")
			present.update_from_xml_node(attribute);
	}
}
XMLNodeUTF8 Verb::create_xml_node() const {
	XMLNodeUTF8 entry;
	
	entry.tag = L"verb";

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

	if (infinitive != L"") {
		XMLNodeUTF8 infinitive_node;
		infinitive_node.tag = L"infinitive";
		infinitive_node.contents = infinitive;
		entry.children.push_back(infinitive_node);
	}

	if (!present.is_empty())
		entry.children.push_back(present.create_xml_node());

	return entry;
}

Verb::Tense::Tense(const std::wstring& name_) :
	name{ name_ }
{}

void Verb::Tense::print() const {
	int default_width = 6;

	int singular_column_width = default_width;
	if (first_person_singular.length()  + 2 > singular_column_width) singular_column_width = first_person_singular.length()  + 2;
	if (second_person_singular.length() + 2 > singular_column_width) singular_column_width = second_person_singular.length() + 2;
	if (third_person_singular.length()  + 2 > singular_column_width) singular_column_width = third_person_singular.length()  + 2;

	int plural_column_width = default_width;
	if (first_person_plural.length()  + 2 > singular_column_width) singular_column_width = first_person_plural.length()  + 2;
	if (second_person_plural.length() + 2 > singular_column_width) singular_column_width = second_person_plural.length() + 2;
	if (third_person_plural.length()  + 2 > singular_column_width) singular_column_width = third_person_plural.length()  + 2;

	std::wcout << L'\t' << std::setw(4) << L' ';
	std::wcout << std::left << std::setw(singular_column_width) << L"sing.";
	std::wcout << std::left << std::setw(plural_column_width) << L"plur." << std::endl;

	std::wcout << L'\t';
	std::wcout << std::right << std::setw(4) << L"1p  ";
	std::wcout << std::left << std::setw(singular_column_width) << first_person_singular;
	std::wcout << std::left << std::setw(plural_column_width)   << first_person_plural << std::endl;

	std::wcout << L'\t';
	std::wcout << std::right << std::setw(4) << L"2p  ";
	std::wcout << std::left << std::setw(singular_column_width) << second_person_singular;
	std::wcout << std::left << std::setw(plural_column_width)   << second_person_plural << std::endl;

	std::wcout << L'\t';
	std::wcout << std::right << std::setw(4) << L"3p  ";
	std::wcout << std::left << std::setw(singular_column_width) << third_person_singular;
	std::wcout << std::left << std::setw(plural_column_width)   << third_person_plural << std::endl;

	return;
}
bool Verb::Tense::is_empty() const {
	return first_person_singular  == L""
		&& second_person_singular == L""
		&& third_person_singular  == L""
		&& first_person_plural    == L""
		&& second_person_plural   == L""
		&& third_person_plural    == L"";
}
bool Verb::Tense::contains(const std::wstring& word) const {
	return first_person_singular  == word
		|| second_person_singular == word
		|| third_person_singular  == word
		|| first_person_plural    == word
		|| second_person_plural   == word
		|| third_person_plural    == word;
}

void Verb::Tense::update_from_xml_node(const XMLNodeUTF8& node) {
	for (const XMLNodeUTF8& form : node.children) {
		if (form.tag == L"first-person-singular")
			first_person_singular = form.contents;
		else if (form.tag == L"second-person-singular")
			second_person_singular = form.contents;
		else if (form.tag == L"third-person-singular")
			third_person_singular = form.contents;
		else if (form.tag == L"first-person-plural")
			first_person_plural = form.contents;
		else if (form.tag == L"second-person-plural")
			second_person_plural = form.contents;
		else if (form.tag == L"third-person-plural")
			third_person_plural = form.contents;
	}
	return;
}
XMLNodeUTF8 Verb::Tense::create_xml_node() const  {
	XMLNodeUTF8 tense_node;

	tense_node.tag = name;
	if (first_person_singular != L"") {
		XMLNodeUTF8 first_person_singular_node;
		first_person_singular_node.tag = L"first-person-singular";
		first_person_singular_node.contents = first_person_singular;
		tense_node.children.push_back(first_person_singular_node);
	}
	if (second_person_singular != L"") {
		XMLNodeUTF8 second_person_singular_node;
		second_person_singular_node.tag = L"second-person-singular";
		second_person_singular_node.contents = second_person_singular;
		tense_node.children.push_back(second_person_singular_node);
	}
	if (third_person_singular != L"") {
		XMLNodeUTF8 third_person_singular_node;
		third_person_singular_node.tag = L"third-person-singular";
		third_person_singular_node.contents = third_person_singular;
		tense_node.children.push_back(third_person_singular_node);
	}
	if (first_person_plural != L"") {
		XMLNodeUTF8 first_person_plural_node;
		first_person_plural_node.tag = L"first-person-plural";
		first_person_plural_node.contents = first_person_plural;
		tense_node.children.push_back(first_person_plural_node);
	}
	if (second_person_plural != L"") {
		XMLNodeUTF8 second_person_plural_node;
		second_person_plural_node.tag = L"second-person-plural";
		second_person_plural_node.contents = second_person_plural;
		tense_node.children.push_back(second_person_plural_node);
	}
	if (third_person_plural != L"") {
		XMLNodeUTF8 third_person_plural_node;
		third_person_plural_node.tag = L"third-person-plural";
		third_person_plural_node.contents = third_person_plural;
		tense_node.children.push_back(third_person_plural_node);
	}

	return tense_node;
}