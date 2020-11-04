#include "verb.h"

#include <iostream>
#include <iomanip>
#include "xml_node.h"

Verb::Verb() :
	Word{ PartOfSpeech::verb },
	present{ L"present" }
{}
Verb::Verb(const XMLNodeUTF8& node) : Verb{} {
	for (const XMLNodeUTF8& attribute : node.children) {
		if (attribute.tag == L"dictionary-form")
			dictionary_form = attribute.contents;
		else if (attribute.tag == L"translation")
			translation = attribute.contents;
		else if (attribute.tag == L"infinitive")
			infinitive = attribute.contents;
		else if (attribute.tag == L"present-tense")
			present.update_from_node(attribute);
	}
}

bool Verb::contains(const std::wstring& word) const {
	return has_form(word) || translation_contains(word);
}
bool Verb::has_form(const std::wstring& word) const {
	return dictionary_form == word || infinitive == word || present.contains(word);
}
bool Verb::translation_contains(const std::wstring& word) const {
	return translation.find(word) != std::wstring::npos;
}

void Verb::print() const {
	std::wcout << std::left << std::setw(24) << std::setfill(L'-') << dictionary_form << std::setfill(L' ') << std::endl;

	std::wcout << L'\t' << translation << std::endl;
	std::wcout << std::endl;

	std::wcout << L"\tpresent" << std::endl;
	present.print();

	return;
}
void Verb::edit_entry() {
	return;
}
XMLNodeUTF8 Verb::create_xml_node() const {
	XMLNodeUTF8 entry{ L"verb" };

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
	if (infinitive != L"") {
		XMLNodeUTF8 i{ L"infinitive" };
		i.contents = infinitive;
		entry.children.push_back(i);
	}
	XMLNodeUTF8 present_node{ present.create_node() };
	if (present_node.children.size() != 0) entry.children.push_back(present_node);

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

void Verb::Tense::update_from_node(const XMLNodeUTF8& node) {
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
XMLNodeUTF8 Verb::Tense::create_node() const  {
	XMLNodeUTF8 t{ name };

	if (first_person_singular != L"") {
		XMLNodeUTF8 f{ L"first-person-singular" };
		f.contents = first_person_singular;
		t.children.push_back(f);
	}
	if (second_person_singular != L"") {
		XMLNodeUTF8 f{ L"second-person-singular" };
		f.contents = second_person_singular;
		t.children.push_back(f);
	}
	if (third_person_singular != L"") {
		XMLNodeUTF8 f{ L"third-person-singular" };
		f.contents = third_person_singular;
		t.children.push_back(f);
	}
	if (first_person_plural != L"") {
		XMLNodeUTF8 f{ L"first-person-plural" };
		f.contents = first_person_plural;
		t.children.push_back(f);
	}
	if (second_person_plural != L"") {
		XMLNodeUTF8 f{ L"second-person-plural" };
		f.contents = second_person_plural;
		t.children.push_back(f);
	}
	if (third_person_plural != L"") {
		XMLNodeUTF8 f{ L"third-person-plural" };
		f.contents = third_person_plural;
		t.children.push_back(f);
	}

	return t;
}