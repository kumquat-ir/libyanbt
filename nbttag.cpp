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

string nbttag::friendly_type(){
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

ostream& operator<<(ostream& os, nbttag it){
	os << "Tag type " << it.friendly_type();
	if(it.named)
		os << ", \"" << it.name << "\"";
	if(it.payload_exists)
		os << "\n" << *static_cast<string*>(get_if<string>(&it.payload));
	if(it.contents.size() > 0){
		os << "\nChildren: \n\t";
		for (auto i : it.contents)
			os << *i << "\n\t";
	}
	return os;
}