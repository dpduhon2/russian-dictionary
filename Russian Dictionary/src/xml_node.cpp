#include "xml_node.h"

#include <stdexcept>
#include "unicode_io.h"

static std::wstring trim_whitespace(const std::wstring& s) {
	const std::wstring WHITESPACE = L" \n\r\t\f\v";
	std::wstring trimmed;

	size_t start = s.find_first_not_of(WHITESPACE);
	trimmed = (start == std::wstring::npos) ? L"" : s.substr(start);
	
	size_t end = s.find_last_not_of(WHITESPACE);
	trimmed = (end == std::wstring::npos) ? L"" : s.substr(0, end + 1);
	
	return trimmed;
}

XMLNodeUTF8::XMLNodeUTF8(const std::wstring& tag_) :
	tag{ tag_ }
{}

XMLNodeUTF8* XMLNodeUTF8::get_child(const std::vector<int>& index) {
	XMLNodeUTF8* current_node = this;
	for (int i : index) {
		if (i < 0 || i >= current_node->children.size()) return nullptr;
		else {
			current_node = &(current_node->children.at(i));
		}
	}
	return current_node;
}

void XMLNodeUTF8::read_from_file(const std::string& filename) {
	ifstreamUTF8 file{ filename };

	tag = L"file";

	bool reading_tag = false;
	bool closing_tag = false;
	std::wstring current_tag;
	std::vector<int> current_node_indices;
	while (file.is_valid()) {
		XMLNodeUTF8& current_node = *get_child(current_node_indices);
		wchar_t c = file.getc();
		switch (c) {
		case L'<':
			reading_tag = true;
			break;
		case L'>':
			if (closing_tag) {
				if (current_node.tag == current_tag) {
					current_node.contents = trim_whitespace(current_node.contents);
					current_node_indices.pop_back();
				}
				else {
					throw std::exception("Ill-formatted file");
				}
				closing_tag = false;
			}
			else {
				current_node.children.push_back(XMLNodeUTF8{ current_tag });
				current_node_indices.push_back(current_node.children.size() - 1);
			}
			reading_tag = false;
			current_tag.clear();
			break;
		case L'/':
			if (reading_tag && current_tag == L"") {
				closing_tag = true;
			}
			break;
		default:
			if (reading_tag) {
				current_tag.push_back(c);
			}
			else {
				current_node.contents.push_back(c);
			}
			break;
		}
	}
	if (reading_tag || closing_tag || current_node_indices.size() != 0) {
		throw std::exception("Ill-formatted file");
	}

	if (children.size() != 1) throw std::exception("Ill-formatted file");
	
	return;
}
void XMLNodeUTF8::write_to_file(const std::string& filename) const {
	ofstreamUTF8 file{ filename, ofstreamUTF8::mode::replace };
	if (!file.is_valid()) throw std::exception("Unable to write to file");

	write_xml_node(file, *this, 0u);

	return;
}

XMLNodeUTF8& XMLNodeUTF8::operator=(const XMLNodeUTF8& node) {
	tag = node.tag;
	contents = node.contents;
	children.clear();
	for (const XMLNodeUTF8& child : node.children) {
		children.push_back(child);
	}
	return *this;
}

void XMLNodeUTF8::write_xml_node(ofstreamUTF8& file, const XMLNodeUTF8& root, unsigned level) const {
	std::wstring indentation(level, L'\t');
	if (root.children.size() == 0) {
		file << indentation << L"<" << root.tag << L">";
		file << root.contents;
		file << L"</" << root.tag << L">\n";
	}
	else {
		file << indentation << L"<" << root.tag << L">\n";
		if (root.contents != L"") file << indentation << L'\t' << root.contents << L'\n';
		for (const XMLNodeUTF8& child : root.children) {
			write_xml_node(file, child, level + 1);
		}
		file << indentation << L"</" << root.tag << L">\n";
	}
	return;
}