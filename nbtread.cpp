#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/copy.hpp>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <endian.h>
#include "nbtfile.h"
#include "nbtparse.h"
#include "nbtread.h"

using namespace std;
namespace io = boost::iostreams;

void print_hex(unique_ptr<char[]> &toPrint, int amt, int start){
	cout << hex << setfill('0');
	for(int i = start; i < amt; i++){
		cout << setw(2) << static_cast<unsigned> (toPrint[i] & 255) << " ";
	}
	cout << endl << dec;
}

char* checkmagic(filesystem::path ifpath){
	ifstream file (ifpath, ios::binary);
	if(!file.good() || !file.is_open()){
		cerr << "file cannot be opened!" << endl;
		file.close();
		throw "File cannot be opened!";
	}
	char* magic = new char[2];
	file.read(magic, 2);
	file.close();
	return magic;
}

nbtfile read_nbt(filesystem::path ifpath){
	char compression = NBT_COMPRESS_NONE;
	char* magic = checkmagic(ifpath);
	string ifname = ifpath.filename();
	if (magic[0] == '\x1f' && magic[1] == '\x8b'){/* gzip detected */
		io::filtering_istream gzfile;
		string tmpf_path = filesystem::temp_directory_path() / "yanbt_tmp";
		ofstream tmpfile (tmpf_path, ios::binary);
		gzfile.push(io::gzip_decompressor());
		cout << "decompressing gzip" << endl;
		gzfile.push(io::file_source(ifpath), ios::binary);
		io::copy(gzfile, tmpfile);
		io::close(gzfile);
		tmpfile.close();
		ifpath = tmpf_path;
		compression = NBT_COMPRESS_GZIP;
	}
	ifstream filein (ifpath, ios::binary);
	if(!filein.good() || !filein.is_open()){
		cerr << "file cannot be opened!" << endl;
		filein.close();
		throw "File cannot be opened!";
	}

	nbtfile nbtdata = nbtfile(ifname);
	nbtdata.set_compression(compression);
	parse(filein, nbtdata);

	filein.close();

	return nbtdata;
}