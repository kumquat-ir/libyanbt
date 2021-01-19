#include <string>
#include <iostream>
#include "nbtfile.h"

using namespace std;

nbtfile::nbtfile (string namei){
	filename = namei;
	nbt_parent_init = false;
}

nbtfile::nbtfile (string namei, nbttag* rooti){
	filename = namei;
	nbt_parent_init = true;
	root_tag = rooti;
}

ostream& operator<<(ostream& os, nbtfile it){
	os << "NBT file " << it.filename << ":\n";
	os << *(it.root_tag);
	return os;
}