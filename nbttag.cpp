#include <string>
#include "nbttag.h"

using namespace std;

nbttag::nbttag (){
	type = NBT_ID_END;
	named = false;
	length = 0;
}

nbttag::nbttag (char typei){
	type = typei;
	named = false;
	length = 0;
}

nbttag::nbttag (char typei, string namei){
	type = typei;
	named = true;
	name = namei;
	length = 0;
}

void nbttag::add_child(nbttag* child){
	contents.push_back(child);
	length++;
}

nbttag* nbttag::get_child(size_t pos){
	return contents.at(pos);
}

nbttag* nbttag::get_last_child(){
	return contents.back();
}

int nbttag::get_child_index(nbttag* child){
	for(int i = 0; i < contents.size(); i++){
		if(contents.at(i) == child){
			return i;
		}
	}
	return -1;
}

void nbttag::set_parent(nbttag* parenti){
	parent = parenti;
}

nbttag* nbttag::get_parent(){
	return parent;
}

int nbttag::length_recurse(){
	int out = length;
	if(contents.size() > 0){
		for(nbttag* i : contents){
			out+=i->length_recurse();
		}
	}
	return out;
}

string nbttag::payload_str() const {
	if(!payload_exists)
		return "No Payload";
	switch(type){
		case NBT_ID_BYTE:
			return to_string(*(get_if<signed char>(&payload)));
		break;
		case NBT_ID_BYTE_ARR:
			{
			string tmpstr = "";
			for(auto i : *(get_if<vector<signed char>>(&payload))){
				tmpstr += to_string(i) + " ";
			}
			return tmpstr;
			}
		break;
		case NBT_ID_DOUBLE:
			return to_string(*(get_if<double>(&payload)));
		break;
		case NBT_ID_FLOAT:
			return to_string(*(get_if<float>(&payload)));
		break;
		case NBT_ID_INT:
			return to_string(*(get_if<int>(&payload)));
		break;
		case NBT_ID_INT_ARR:
			{
			string tmpstr = "";
			for(auto i : *(get_if<vector<signed int>>(&payload))){
				tmpstr += to_string(i) + " ";
			}
			return tmpstr;
			}
		break;
		case NBT_ID_LONG:
			return to_string(*(get_if<long>(&payload)));
		break;
		case NBT_ID_LONG_ARR:
			{
			string tmpstr = "";
			for(auto i : *(get_if<vector<signed long>>(&payload))){
				tmpstr += to_string(i) + " ";
			}
			return tmpstr;
			}
		break;
		case NBT_ID_SHORT:
			return to_string(*(get_if<short>(&payload)));
		break;
		case NBT_ID_STRING:
			return *(get_if<string>(&payload));
		break;
	}
	return "Invalid type";
}

nbt_payload nbttag::get_payload(){
	return payload;
}

ostream& operator<<(ostream& os, const nbttag &it){
	os << "Tag type " << nbt_friendly_type(it.type);
	if(it.named)
		os << ", \"" << it.name << "\"";
	if(it.payload_exists)
		os << " : " << it.payload_str();
	if(it.contents.size() > 0){
		os << " {\n";
		for (auto i : it.contents)
			os << *i << "\n";
		os << "}";
		if(it.named)
			os << " (" << it.name << ")";
	}
	return os;
}

void nbttag::write_data(ofstream& os){
	if(named){
		os.put(type);
		unsigned short tmp = htobe16(static_cast<unsigned short>(name.length()));
		os.write(reinterpret_cast<char*>(&tmp), 2);
		for (char i : name)
			os.put(i);
	}
	if(payload_exists){
		switch(type){
			case NBT_ID_BYTE:
				os.put(*get_if<signed char>(&payload));
			break;
			case NBT_ID_BYTE_ARR:{
				unsigned int tmp = htobe32(static_cast<unsigned int>((*get_if<vector<signed char>>(&payload)).size()));
				os.write(reinterpret_cast<char*>(&tmp), 4);
				for(signed char i : *(get_if<vector<signed char>>(&payload)))
					os.put(i);
			}break;
			case NBT_ID_DOUBLE:{
				unsigned long tmp = htobe64(*reinterpret_cast<unsigned long*>(get_if<double>(&payload)));
				os.write(reinterpret_cast<char*>(&tmp), 8);
			}break;
			case NBT_ID_FLOAT:{
				unsigned int tmp = htobe32(*reinterpret_cast<unsigned int*>(get_if<float>(&payload)));
				os.write(reinterpret_cast<char*>(&tmp), 4);
			}break;
			case NBT_ID_INT:{
				unsigned int tmp = htobe32(*reinterpret_cast<unsigned int*>(get_if<int>(&payload)));
				os.write(reinterpret_cast<char*>(&tmp), 4);
			}break;
			case NBT_ID_INT_ARR:{
				unsigned int tmp = htobe32(static_cast<unsigned int>((*get_if<vector<int>>(&payload)).size()));
				os.write(reinterpret_cast<char*>(&tmp), 4);
				for(signed int i : *(get_if<vector<signed int>>(&payload))){
					unsigned int tmp = htobe32(*reinterpret_cast<unsigned int*>(&i));
					os.write(reinterpret_cast<char*>(&tmp), 4);
				}
			}break;
			case NBT_ID_LONG:{
				unsigned long tmp = htobe64(*reinterpret_cast<unsigned long*>(get_if<long>(&payload)));
				os.write(reinterpret_cast<char*>(&tmp), 8);
			}break;
			case NBT_ID_LONG_ARR:{
				unsigned int tmp = htobe32(static_cast<unsigned int>((*get_if<vector<long>>(&payload)).size()));
				os.write(reinterpret_cast<char*>(&tmp), 4);
				for(long i : *(get_if<vector<long>>(&payload))){
					unsigned long tmp = htobe64(*reinterpret_cast<unsigned long*>(&i));
					os.write(reinterpret_cast<char*>(&tmp), 8);
				}
			}break;
			case NBT_ID_SHORT:{
				unsigned short tmp = htobe16(*reinterpret_cast<unsigned short*>(get_if<short>(&payload)));
				os.write(reinterpret_cast<char*>(&tmp), 2);
			}break;
			case NBT_ID_STRING:{
				string tmpstr = *get_if<string>(&payload);
				unsigned short tmp = htobe16(static_cast<unsigned short>(tmpstr.length()));
				os.write(reinterpret_cast<char*>(&tmp), 2);
				for (char i : tmpstr)
					os.put(i);
			}break;
		}
	}
	if(type == NBT_ID_LIST || type == NBT_ID_COMPOUND){
		if(type == NBT_ID_LIST){
			os.put(*get_if<signed char>(&payload));
			unsigned int tmp = htobe32(*reinterpret_cast<unsigned int*>(&length));
			os.write(reinterpret_cast<char*>(&tmp), 4);
		}
		for(nbttag* i : contents){
			i->write_data(os);
		}
		if(type == NBT_ID_COMPOUND){
			os.put('\x00'); //ending TAG_END
		}
	}
}

std::string nbt_friendly_type(char type){
	switch(type){
		case NBT_ID_BYTE:
			return "Byte";
		break;
		case NBT_ID_BYTE_ARR:
			return "Byte Array";
		break;
		case NBT_ID_COMPOUND:
			return "Compound";
		break;
		case NBT_ID_DOUBLE:
			return "Double";
		break;
		case NBT_ID_END:
			return "END";
		break;
		case NBT_ID_FLOAT:
			return "Float";
		break;
		case NBT_ID_INT:
			return "Int";
		break;
		case NBT_ID_INT_ARR:
			return "Int Array";
		break;
		case NBT_ID_LIST:
			return "List";
		break;
		case NBT_ID_LONG:
			return "Long";
		break;
		case NBT_ID_LONG_ARR:
			return "Long Array";
		break;
		case NBT_ID_SHORT:
			return "Short";
		break;
		case NBT_ID_STRING:
			return "String";
		break;
	}
	return "Invalid";
}