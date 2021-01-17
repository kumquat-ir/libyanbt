#ifndef NBTFILE_H
#define NBTFILE_H

#include <string>
#include "nbttag.h"

class nbtfile {
public:
	std::string filename;
	nbttag_parent root_tag;
};

#endif /* NBTFILE_H */