#include "verb.h"

#include <iostream>
#include <iomanip>
#include "xml_node.h"

Verb::Verb() :
	Word{ PartOfSpeech::verb }
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
			update_tense_from_xml_node(attribute, present);
	}
}

bool Verb::contains(const std::wstring& word) const {
	return has_form(word) || translation_contains(word);
}
bool Verb::has_form(const std::wstring& word) const {
	return dictionary_form == word || infinitive == word || tense_contains(present, word);
}
bool Verb::translation_contains(const std::wstring& word) const {
	return translation.find(word) != std::wstring::npos;
}

void Verb::print() const {
	std::wcout << std::left << std::setw(24) << std::setfill(L'-') << dictionary_form << std::setfill(L' ') << std::endl;

	std::wcout << L'\t' << translation << std::endl;
	std::wcout << std::endl;

	std::wcout << L"\tpresent" << std::endl;
	print_tense(present);

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
	XMLNodeUTF8 present_node{ create_tense_node(present, L"present-tense") };
	if (present_node.children.size() != 0) entry.children.push_back(present_node);

	return entry;
}

bool Verb::tense_contains(const Tense& tense, const std::wstring& word) const {
	return tense.first_person_singular == word
		|| tense.second_person_singular == word
		|| tense.third_person_singular == word
		|| tense.first_person_plural == word
		|| tense.second_person_plural == word
		|| tense.third_person_plural == word;
}

void Verb::print_tense(const Tense& tense) const {
	int default_width = 6;

	int singular_column_width = default_width;
	if (tense.first_person_singular.length() + 2 > singular_column_width) singular_column_width = tense.first_person_singular.length() + 2;
	if (tense.second_person_singular.length() + 2 > singular_column_width) singular_column_width = tense.second_person_singular.length() + 2;
	if (tense.third_person_singular.length() + 2 > singular_column_width) singular_column_width = tense.third_person_singular.length() + 2;

	int plural_column_width = default_width;
	if (tense.first_person_plural.length() + 2 > singular_column_width) singular_column_width = tense.first_person_plural.length() + 2;
	if (tense.second_person_plural.length() + 2 > singular_column_width) singular_column_width = tense.second_person_plural.length() + 2;
	if (tense.third_person_plural.length() + 2 > singular_column_width) singular_column_width = tense.third_person_plural.length() + 2;

	std::wcout << L'\t' << std::setw(4) << L' ';
	std::wcout << std::left << std::setw(singular_column_width) << L"sing.";
	std::wcout << std::left << std::setw(plural_column_width) << L"plur." << std::endl;

	std::wcout << L'\t';
	std::wcout << std::right << std::setw(4) << L"1p  ";
	std::wcout << std::left << std::setw(singular_column_width) << present.first_person_singular;
	std::wcout << std::left << std::setw(plural_column_width) << present.first_person_plural << std::endl;

	std::wcout << L'\t';
	std::wcout << std::right << std::setw(4) << L"2p  ";
	std::wcout << std::left << std::setw(singular_column_width) << present.second_person_singular;
	std::wcout << std::left << std::setw(plural_column_width) << present.second_person_plural << std::endl;

	std::wcout << L'\t';
	std::wcout << std::right << std::setw(4) << L"3p  ";
	std::wcout << std::left << std::setw(singular_column_width) << present.third_person_singular;
	std::wcout << std::left << std::setw(plural_column_width) << present.third_person_plural << std::endl;

	/* \u044f
	 * \u0442\u044b
	 * \u043e\u043d\u043e
	 * \u043c\u044b
	 * \u0432\u044b
	 * \u043e\u043d\u0438
	 */

	return;
}
XMLNodeUTF8 Verb::create_tense_node(const Tense& tense, const std::wstring& name) const {
	XMLNodeUTF8 t{ name };

	if (tense.first_person_singular != L"") {
		XMLNodeUTF8 f{ L"first-person-singular" };
		f.contents = tense.first_person_singular;
		t.children.push_back(f);
	}
	if (tense.second_person_singular != L"") {
		XMLNodeUTF8 f{ L"second-person-singular" };
		f.contents = tense.second_person_singular;
		t.children.push_back(f);
	}
	if (tense.third_person_singular != L"") {
		XMLNodeUTF8 f{ L"third-person-singular" };
		f.contents = tense.third_person_singular;
		t.children.push_back(f);
	}
	if (tense.first_person_plural != L"") {
		XMLNodeUTF8 f{ L"first-person-plural" };
		f.contents = tense.first_person_plural;
		t.children.push_back(f);
	}
	if (tense.second_person_plural != L"") {
		XMLNodeUTF8 f{ L"second-person-plural" };
		f.contents = tense.second_person_plural;
		t.children.push_back(f);
	}
	if (tense.third_person_plural != L"") {
		XMLNodeUTF8 f{ L"third-person-plural" };
		f.contents = tense.third_person_plural;
		t.children.push_back(f);
	}

	return t;
}
void Verb::update_tense_from_xml_node(const XMLNodeUTF8& tense_node, Tense& tense) {
	for (const XMLNodeUTF8& form : tense_node.children) {
		if (form.tag == L"first-person-singular")
			tense.first_person_singular = form.contents;
		else if (form.tag == L"second-person-singular")
			tense.second_person_singular = form.contents;
		else if (form.tag == L"third-person-singular")
			tense.third_person_singular = form.contents;
		else if (form.tag == L"first-person-plural")
			tense.first_person_plural = form.contents;
		else if (form.tag == L"second-person-plural")
			tense.second_person_plural = form.contents;
		else if (form.tag == L"third-person-plural")
			tense.third_person_plural = form.contents;
	}
	return;
}