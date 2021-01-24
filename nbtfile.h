#ifndef NBTFILE_H
#define NBTFILE_H

#include <string>
#include <iostream>
#include <fstream>
#include "nbttag.h"

constexpr char NBT_COMPRESS_NONE = '\x00';
constexpr char NBT_COMPRESS_GZIP = '\x01';
constexpr char NBT_COMPRESS_ZLIB = '\x02';

class nbtfile {
private:
	bool nbt_parent_init = false;
public:
	std::string filename;
	char compress_type = NBT_COMPRESS_NONE;
	nbttag root_tag;
	nbtfile(std::string);
	nbtfile(std::string, nbttag);
	void init_root_tag(nbttag);
	void write_file(std::string);
	void set_compression(char);
	friend std::ostream& operator<<(std::ostream& os, const nbtfile& it);
};

#endif /* NBTFILE_H */