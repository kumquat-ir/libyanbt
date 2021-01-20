#ifndef NBTTAG_H
#define NBTTAG_H

#include <string>
#include <vector>
#include <iostream>
#include <memory>
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

typedef std::variant<signed char, short, int, long, float, double, std::shared_ptr<signed char>,\
		std::shared_ptr<int>, std::shared_ptr<long>, std::string> nbt_payload;

class nbttag {
protected:
	std::vector<nbttag*> contents;
public:
	nbt_payload payload;
	bool payload_exists = false;
	char type;
	bool named;
	std::string name;
	int length;
	nbttag();
	nbttag(char);
	nbttag(char, std::string);
	std::string friendly_type();
	void add_child(nbttag*);
	nbttag* get_child(size_t);
	nbttag* get_last_child();
	template < class T >
	void init_payload(T payloadi){
		payload_exists = true;
		payload = payloadi;
	}
	std::string payload_str();
	friend std::ostream& operator<<(std::ostream& os, nbttag it);
};

#endif /* NBTTAG_H */