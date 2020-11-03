#include <iostream>
#include <string>
#include <stdexcept>
#include "unicode_io.h"
#include "dictionary.h"
#include "menu.h"
#include "noun.h"
#include "verb.h"

void present_main_menu(Dictionary& d);

void present_search_menu(Dictionary& d);
void present_add_word_menu(Dictionary& d);
void present_edit_word_menu(Dictionary& d);
void present_remove_word_menu(Dictionary& d);

void present_add_noun_menu(Dictionary& d);
void present_add_verb_menu(Dictionary& d);

void update_noun(Noun& n);
void update_verb(Verb& v);

void update_noun_gender(Noun& n);
void update_noun_case(Noun::Case& c);

void update_verb_infinitive(Verb& v);
void update_verb_tense(Verb::Tense& t);

Word* get_word_from_user(Dictionary& d);

int main() {
	initialize_io();

	Dictionary d;
	d.load_from_file("dictionary.txt");

	present_main_menu(d);

	d.save_to_file("dictionary.txt");

	return 0;
}

void present_main_menu(Dictionary& d) {
	MenuUTF8 main_menu;
	main_menu.add_option(L's', L"to search for a word");
	main_menu.add_option(L'a', L"to add a word");
	main_menu.add_option(L'u', L"to update a word");
	main_menu.add_option(L'r', L"to remove a word");
	main_menu.add_option(L'e', L"to exit");

	while (true) {
		switch (main_menu.get_user_choice()) {
		case L's':
			present_search_menu(d);
			break;
		case L'a':
			present_add_word_menu(d);
			break;
		case L'u':
			present_edit_word_menu(d);
			break;
		case L'r':
			present_remove_word_menu(d);
			break;
		case L'e':
			return;
		}
	}
}

void present_search_menu(Dictionary& d) {
	std::wcout << L"What word would you like to see?" << std::endl;

	Word* word = get_word_from_user(d);
	if (word == nullptr) return;

	word->print();
	std::wcout << std::endl;

	return;
}
void present_add_word_menu(Dictionary& d) {
	std::wcout << L"What is the word's part of speech?" << std::endl;

	MenuUTF8 menu;
	menu.add_option(L'n', L"to add a noun");
	menu.add_option(L'v', L"to add a verb");
	menu.add_option(L'e', L"to return to the main menu");
	
	switch (menu.get_user_choice()) {
	case L'n':
		present_add_noun_menu(d);
		break;
	case L'v':
		present_add_verb_menu(d);
		break;
	case L'e':
		return;
	}
}
void present_edit_word_menu(Dictionary& d) {
	std::wcout << L"What word would you like to edit?" << std::endl;

	Word* word = get_word_from_user(d);
	if (word == nullptr) return;

	switch (word->part_of_speech) {
	case Word::PartOfSpeech::noun:
		update_noun(*static_cast<Noun*>(word));
		break;
	case Word::PartOfSpeech::verb:
		update_verb(*static_cast<Verb*>(word));
		break;
	}

	return;
}
void present_remove_word_menu(Dictionary& d) {
	std::wcout << L"What word would you like to remove?" << std::endl;

	Word* word = get_word_from_user(d);
	if (word == nullptr) return;

	d.remove_word(word);

	std::wcout << L"Successfully removed" << std::endl;
	std::wcout << std::endl;

	return;
}

void present_add_noun_menu(Dictionary& d) {
	std::wstring dictionary_form, translation;

	std::wcout << "Dictionary form: ";
	std::getline(std::wcin, dictionary_form);
	std::wcout << "Translation: ";
	std::getline(std::wcin, translation);
	std::wcout << std::endl;

	Noun& noun = d.add_noun(dictionary_form, translation);
	std::wcout << "Added ";
	noun.print_preview();
	std::wcout << std::endl;

	update_noun(noun);

	return;
}
void present_add_verb_menu(Dictionary& d) {
	std::wstring dictionary_form, translation;

	std::wcout << "Dictionary form: ";
	std::getline(std::wcin, dictionary_form);
	std::wcout << "Translation: ";
	std::getline(std::wcin, translation);
	std::wcout << std::endl;

	Verb& verb = d.add_verb(dictionary_form, translation);
	std::wcout << "Added ";
	verb.print_preview();
	std::wcout << std::endl;

	update_verb(verb);

	return;
}

void update_noun(Noun& n) {
	MenuUTF8 menu;
	menu.add_option(L'g', L"to update the gender");
	menu.add_option(L'n', L"to update the nominative case");
	menu.add_option(L'a', L"to update the accusative case");
	menu.add_option(L'e', L"to exit");

	while (true) {
		switch (menu.get_user_choice()) {
		case L'g':
			update_noun_gender(n);
			break;
		case L'n':
			update_noun_case(n.nominative);
			break;
		case L'a':
			update_noun_case(n.accusative);
			break;
		case L'e':
			return;
		}
	}
}
void update_verb(Verb& v) {
	MenuUTF8 menu;
	menu.add_option(L'i', L"to update the infinitive");
	menu.add_option(L'r', L"to update the present tense");
	menu.add_option(L'e', L"to exit");

	while (true) {
		switch (menu.get_user_choice()) {
		case L'i':
			update_verb_infinitive(v);
			break;
		case L'r':
			update_verb_tense(v.present);
			break;
		case L'e':
			return;
		}
	}
}

void update_noun_gender(Noun& n) {
	std::wcout << n.dictionary_form << L": ";
	switch (n.gender) {
	case Noun::Gender::masculine:
		std::wcout << L"masculine";
		break;
	case Noun::Gender::feminine:
		std::wcout << L"feminine";
		break;
	case Noun::Gender::neuter:
		std::wcout << L"neuter";
		break;
	}
	std::wcout << std::endl << std::endl;

	MenuUTF8 input;
	input.add_option(L'm', L"for masculine");
	input.add_option(L'f', L"for feminine");
	input.add_option(L'n', L"for neuter");

	switch (input.get_user_choice()) {
	case L'm':
		n.gender = Noun::Gender::masculine;
		break;
	case L'f':
		n.gender = Noun::Gender::feminine;
		break;
	case L'n':
		n.gender = Noun::Gender::neuter;
		break;
	}

	return;
}
void update_noun_case(Noun::Case& c) {
	std::wstring singular, plural;

	std::wcout << "Singular: ";
	std::getline(std::wcin, singular);
	std::wcout << "Plural: ";
	std::getline(std::wcin, plural);
	std::wcout << std::endl;

	c.singular = (singular == L"") ? c.singular : singular;
	c.plural   = (plural   == L"") ? c.plural   : plural;

	return;
}

void update_verb_infinitive(Verb& v) {
	std::wstring infinitive;

	std::wcout << "Infinitive: ";
	std::getline(std::wcin, infinitive);
	std::wcout << std::endl;

	v.infinitive = (infinitive == L"") ? v.infinitive : infinitive;

	return;
}
void update_verb_tense(Verb::Tense& t) {
	std::wstring first_person_singular, first_person_plural;
	std::wstring second_person_singular, second_person_plural;
	std::wstring third_person_singular, third_person_plural;

	std::wcout << "First-person singular: ";
	std::getline(std::wcin, first_person_singular);
	std::wcout << "Second-person singular: ";
	std::getline(std::wcin, second_person_singular);
	std::wcout << "Third-person singular: ";
	std::getline(std::wcin, third_person_singular);
	std::wcout << "First-person plural: ";
	std::getline(std::wcin, first_person_plural);
	std::wcout << "Second-person plural: ";
	std::getline(std::wcin, second_person_plural);
	std::wcout << "Third-person plural: ";
	std::getline(std::wcin, third_person_plural);
	std::wcout << std::endl;

	t.first_person_singular = (first_person_singular == L"") ? t.first_person_singular : first_person_singular;
	t.second_person_singular = (second_person_singular == L"") ? t.second_person_singular : second_person_singular;
	t.third_person_singular = (third_person_singular == L"") ? t.third_person_singular : third_person_singular;
	t.first_person_plural = (first_person_plural == L"") ? t.first_person_plural : first_person_plural;
	t.second_person_plural = (second_person_plural == L"") ? t.second_person_plural : second_person_plural;
	t.third_person_plural = (third_person_plural == L"") ? t.third_person_plural : third_person_plural;

	return;
}

Word* get_word_from_user(Dictionary& d) {
	std::wstring word;
	std::wcout << L">> ";
	std::getline(std::wcin, word);
	std::wcout << std::endl;

	if (word == L"") return nullptr;

	std::vector<Word*> words{ d.find_words_containing(word) };

	if (words.size() == 0) {
		std::wcout << L"No matches found" << std::endl;
		std::wcout << std::endl;
		return nullptr;
	}
	else if (words.size() == 1) {
		std::wcout << L"One match found:" << std::endl;
		words.at(0)->print_preview();
		std::wcout << std::endl;
		
		std::wcout << L"Is this the entry you meant? ";
		Word* result = get_yes_or_no() ? words.at(0) : nullptr;
		std::wcout << std::endl;

		return result;
	}
	else {
		std::wcout << L"Multiple potential matches:" << std::endl;
		for (std::size_t i = 0; i < words.size(); ++i) {
			std::wcout << i + 1 << L". ";
			words.at(i)->print_preview();
		}
		std::wcout << std::endl;

		std::wstring input;
		int choice;
		std::wcout << L"Which entry did you mean?" << std::endl;
		while (true) {
			std::wcout << L">> ";
			std::getline(std::wcin, input);

			if (input == L"") {
				std::wcout << std::endl;
				return nullptr;
			}

			try {
				choice = std::stoi(input);
				if (choice >= 1 && choice <= words.size()) {
					std::wcout << std::endl;
					return words.at(choice - 1);
				}
			}
			catch (const std::exception& e) {}

			if (words.size() == 2)
				std::wcout << L"Enter 1 or 2" << std::endl;
			else
				std::wcout << L"Enter an integer from 1 to " << words.size() << std::endl;
		}
	}
}