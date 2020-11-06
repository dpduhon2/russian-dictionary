#ifndef XML_NODE_H
#define XML_NODE_H

#include <string>
#include <vector>

class ofstreamUTF8;

class XMLNodeUTF8 {
public:
	std::wstring tag;
	std::wstring contents;
	std::vector<XMLNodeUTF8> children;

	XMLNodeUTF8* get_child(const std::vector<int>& index);

	void read_from_file(const std::string& filename);
	void write_to_file(const std::string& filename) const;

	XMLNodeUTF8& operator=(const XMLNodeUTF8& node);

private:
	void write_xml_node(ofstreamUTF8& file, const XMLNodeUTF8& root, unsigned level) const;
};

#endif