#include <string>
#include "nbttag.h"

using namespace std;

nbttag::nbttag (char typei){
	type = typei;
	named = false;
}

nbttag::nbttag (char typei, string namei){
	type = typei;
	named = true;
	name = namei;
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
			return "Short";
		break;
	}
	return "Invalid";
}

ostream& operator<<(ostream& os, nbttag it){
	os << "Tag type " << it.friendly_type();
	if(it.named)
		os << ", \"" << it.name << "\"";
	return os;
}

nbttag_parent::nbttag_parent (char typei, int lengthi) : nbttag(typei) {
	length = lengthi;
}

nbttag_parent::nbttag_parent (char typei, string namei, int lengthi) : nbttag(typei, namei) {
	length = lengthi;
}

void nbttag_parent::add_child (nbttag child){
	contents.push_back(child);
}