#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/copy.hpp>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <endian.h>
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

int checkgz(string ifname){
	/* use magic number 1f8b to check if file ifname is a gzip */
	ifstream file (ifname, ios::binary);
	if(!file.good() || !file.is_open()){ // TODO learn how to throw an error
		cerr << "file cannot be opened!" << endl;
		file.close();
		return -1;
	}
	char magic[2];
	int out = 0;
	file.read(magic, 2);
	if(magic[0]=='\x1f' && magic[1]=='\x8b')
		out = 1;
	file.close();
	return out;
}

int main(int argc, char** argv){
	if(argc <= 1){
		cerr << "no file provided" << endl;
		return 1;
	}
	string ifname = argv[1];
	unique_ptr<char[]> filemem;
	int fsize;
	int cgz = checkgz(ifname);
	if(cgz == -1){
		return 1;
	} else if (cgz == 1){/* gzip detected */
		io::filtering_istream gzfile;
		string nifname = filesystem::temp_directory_path() / "yanbt_tmp";
		ofstream tmpfile (nifname);
		gzfile.push(io::gzip_decompressor());
		cout << "decompressing gzip" << endl;
		gzfile.push(io::file_source(ifname), ios::binary);
		io::copy(gzfile, tmpfile);
		io::close(gzfile);
		ifname = nifname;
	}
	ifstream filein (ifname, ios::binary | ios::ate);
	if(!filein.good() || !filein.is_open()){
		cerr << "file cannot be opened!" << endl;
		filein.close();
		return 1;
	}
	fsize = filein.tellg();
	filemem.reset(new char[fsize]);
	filein.seekg(ios::beg);
	filein.read(filemem.get(), fsize);
	
	cout << "size: " << fsize << endl;
	//print_hex(filemem, fsize, 0);

	nbtfile nbtdata = nbtfile("default filename");
	parse(filein, nbtdata);
	cout << nbtdata << endl;

	filein.close();
	return 0;
}