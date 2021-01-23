#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/copy.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "nbtfile.h"

using namespace std;
namespace io = boost::iostreams;

nbtfile::nbtfile (string namei){
	filename = namei;
	nbt_parent_init = false;
}

nbtfile::nbtfile (string namei, nbttag rooti){
	filename = namei;
	nbt_parent_init = true;
	root_tag = rooti;
}

void nbtfile::init_root_tag(nbttag rooti){
	nbt_parent_init = true;
	root_tag = rooti;
}

void nbtfile::write_file(string ofname){
	if(compress_type == NBT_COMPRESS_GZIP){
		ofstream tmpos(filesystem::temp_directory_path() / "yanbt_tmp", ios::out | ios::binary);
		root_tag.write_data(tmpos);
		tmpos.close();
		io::filtering_istream fos;
		fos.push(io::gzip_compressor());
		fos.push(io::file_source(filesystem::temp_directory_path() / "yanbt_tmp"), ios::binary);
		ofstream os(ofname, ios::out | ios::binary);
		io::copy(fos, os);
		io::close(fos);
		os.close();
	} else {
		ofstream os(ofname, ios::out | ios::binary);
		root_tag.write_data(os);
		os.close();
	}
}

ostream& operator<<(ostream& os, const nbtfile& it){
	os << "NBT file " << it.filename << ":\n";
	os << it.root_tag;
	return os;
}