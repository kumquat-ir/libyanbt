#ifndef NBTFILE_H
#define NBTFILE_H

#include <string>
#include <iostream>
#include "nbttag.h"

class nbtfile {
private:
	bool nbt_parent_init = false;
public:
	std::string filename;
	nbttag root_tag;
	nbtfile(std::string);
	nbtfile(std::string, nbttag);
	void init_root_tag(nbttag);
	friend std::ostream& operator<<(std::ostream& os, nbtfile& it);
};

#endif /* NBTFILE_H */