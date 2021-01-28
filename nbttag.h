#ifndef NBTTAG_H
#define NBTTAG_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <variant>

constexpr char NBT_ID_END = '\x00';
constexpr char NBT_ID_BYTE = '\x01';
constexpr char NBT_ID_SHORT = '\x02';
constexpr char NBT_ID_INT = '\x03';
constexpr char NBT_ID_LONG = '\x04';
constexpr char NBT_ID_FLOAT = '\x05';
constexpr char NBT_ID_DOUBLE = '\x06';
constexpr char NBT_ID_BYTE_ARR = '\x07';
constexpr char NBT_ID_STRING = '\x08';
constexpr char NBT_ID_LIST = '\x09';
constexpr char NBT_ID_COMPOUND = '\x0a';
constexpr char NBT_ID_INT_ARR = '\x0b';
constexpr char NBT_ID_LONG_ARR = '\x0c';

typedef std::variant<signed char, short, int, long, float, double, std::vector<signed char>,\
		std::vector<int>, std::vector<long>, std::string> nbt_payload;

class nbttag {
protected:
	std::vector<nbttag*> contents;
	nbt_payload payload;
	nbttag* parent = nullptr;
public:
	bool payload_exists = false;
	char type;
	bool named;
	std::string name;
	int length;
	nbttag();
	nbttag(char);
	nbttag(char, std::string);
	int length_recurse();
	void add_child(nbttag*);
	nbttag* get_child(size_t);
	nbttag* get_last_child();
	void set_parent(nbttag*);
	nbttag* get_parent();
	template < class T >
	void init_payload(T payloadi){
		payload_exists = true;
		payload = payloadi;
	}
	template < class T >
	void set_payload(T payloadi){
		payload = payloadi;
	}
	nbt_payload get_payload();
	std::string payload_str() const;
	void write_data(std::ofstream&);
	friend std::ostream& operator<<(std::ostream& os, const nbttag &it);
};

std::string nbt_friendly_type(char);

#endif /* NBTTAG_H */