#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/copy.hpp>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <filesystem>

using namespace std;
namespace io = boost::iostreams;

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
	streampos size;
	unique_ptr<char[]> filemem;
	int fsize;
	switch(checkgz(ifname)){
		case -1: /* error */
			return 1;
			break;
		case 1: {/* gzip detected */
			io::filtering_istream gzfile;
			string nifname = filesystem::temp_directory_path() / "yanbt_tmp";
			ofstream tmpfile (nifname);
			gzfile.push(io::gzip_decompressor());
			cout << "decompressing gzip" << endl;
			gzfile.push(io::file_source(ifname), ios::binary);
			io::copy(gzfile, tmpfile);
			io::close(gzfile);
			ifname = nifname;
			} // for scope
			/* FALL THROUGH */
		case 0: {/* no gzip */
			ifstream filein (ifname, ios::binary | ios::ate);
			if(!filein.good() || !filein.is_open()){
				cerr << "file cannot be opened!" << endl;
				filein.close();
				return 1;
			}
			size = filein.tellg();
			filemem.reset(new char[size]);
			filein.seekg(ios::beg);
			filein.read(filemem.get(), size);
			filein.close();
			fsize = size;
			} // for scope
			break;
		default:
			cout << "checkgz returned something odd, aborting" << endl;
			return 1;
	}
	
	cout << "size: " << fsize << endl;
	cout << hex << setfill('0');
	for(int i = 0; i < fsize; i++){
		cout << setw(2) << static_cast<unsigned> (filemem[i] & 255) << " ";
	}
	cout << endl << dec;

	return 0;
}