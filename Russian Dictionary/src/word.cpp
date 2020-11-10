#include "word.h"

#include <iostream>

Word::Word(PartOfSpeech pos) :
	part_of_speech{ pos }
{}

void Word::print_preview() const {
	std::wcout << dictionary_form << L" (";
	switch (part_of_speech) {
	case PartOfSpeech::noun:
		std::wcout << L"noun";
		break;
	case PartOfSpeech::verb:
		std::wcout << L"verb";
		break;
	case PartOfSpeech::adjective:
		std::wcout << L"adjective";
		break;
	}
	std::wcout << L") - " << translation << std::endl;

	return;
}
bool Word::translation_contains(const std::wstring& word) const {
	auto index = translation.find(word);
	return (index != std::wstring::npos)
		&& (index == 0 || translation.at(index - 1) == L' ')
		&& (index == translation.size() - word.size() || translation.at(index + word.size()) == L' ');
}

bool operator<(const Word& w_1, const Word& w_2) {
	return w_1.dictionary_form < w_2.dictionary_form;
}