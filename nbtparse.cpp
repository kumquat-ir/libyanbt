#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <endian.h>
#include "nbtparse.h"

using namespace std;

int parse(ifstream &nbtfile){
	nbtfile.seekg(ios::beg);

	unique_ptr<char[]> buf;
	unsigned short stringl;
	vector<char> containers = {};

	while(!nbtfile.eof()){
		buf.reset(new char[1]);
		nbtfile.read(buf.get(), 1);
		switch(buf[0]){
			case NBT_ID_COMPOUND:
				cout << "Compound: ";
				buf.reset(new char[2]);
				nbtfile.read(buf.get(), 2);
				stringl = htobe16(*reinterpret_cast<unsigned short*>(buf.get())); //this is probably the wrong way to do this
				cout << stringl << endl;
				buf.reset(new char[stringl]);
				nbtfile.read(buf.get(), stringl);
				cout << buf << endl;
				containers.push_back(NBT_ID_COMPOUND);
				continue;
			break;
			case NBT_ID_STRING:
				cout << "String: ";
				buf.reset(new char[2]);
				nbtfile.read(buf.get(), 2);
				stringl = htobe16(*reinterpret_cast<unsigned short*>(buf.get()));
				buf.reset(new char[stringl]);
				nbtfile.read(buf.get(), stringl);
				cout << buf << endl << "\t";
				buf.reset(new char[2]);
				nbtfile.read(buf.get(), 2);
				stringl = htobe16(*reinterpret_cast<unsigned short*>(buf.get()));
				buf.reset(new char[stringl]);
				nbtfile.read(buf.get(), stringl);
				cout << buf << endl;
			break;
			case NBT_ID_END:
				cout << "End" << endl;
			break;
		}
	}

	return 0;
}