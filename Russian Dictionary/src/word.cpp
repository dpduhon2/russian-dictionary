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
	}
	std::wcout << L") - " << translation << std::endl;

	return;
}

bool operator<(const Word& w_1, const Word& w_2) {
	return w_1.dictionary_form < w_2.dictionary_form;
}