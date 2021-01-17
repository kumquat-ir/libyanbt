#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <endian.h>
#include "nbtparse.h"
#include "nbttag.h"

using namespace std;

/* im using these specific snippets a lot this just makes it so much easier 
   dont use these outside of the parse() loop */
#define readbuf( amt ) { buf.reset(new char[amt]); nbtfile.read(buf.get(), amt); }
#define readbuf_ushort( res ) { buf.reset(new char[2]); nbtfile.read(buf.get(), 2); res = htobe16(*reinterpret_cast<unsigned short*>(buf.get())); }
#define readbuf_str( res ) { readbuf_ushort( res ); if(res > 0) {\
	buf.reset(new char[res + 1]); nbtfile.read(buf.get(), res);\
	buf[res] = '\x00';\
} }

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
				readbuf_str(stringl);
				cout << stringl << endl;
				cout << buf << endl;
				containers.push_back(NBT_ID_COMPOUND);
			break;
			case NBT_ID_STRING:
				cout << "String: ";
				readbuf_str(stringl);
				cout << buf << endl << "\t";
				readbuf_str(stringl);
				cout << buf << endl;
			break;
			case NBT_ID_END:
				cout << "End" << endl;
			break;
			case NBT_ID_BYTE:
				cout << "Byte: ";
				readbuf_str(stringl);
				cout << buf << endl;
				readbuf(1);
				cout << static_cast<unsigned char>(buf[0]) << endl;
			break;
			case NBT_ID_INT:
				cout << "Int: ";
				readbuf_str(stringl);
				cout << buf << endl;
				readbuf(4);
				cout << htobe32(*reinterpret_cast<int*>(buf.get())) << endl;
			break;
			case NBT_ID_LONG:
				cout << "Long: ";
				readbuf_str(stringl);
				cout << buf << endl;
				readbuf(8);
				cout << htobe64(*reinterpret_cast<long*>(buf.get())) << endl;
			break;
			case NBT_ID_SHORT:
				cout << "Short: ";
				readbuf_str(stringl);
				cout << buf << endl;
				readbuf(2);
				cout << htobe16(*reinterpret_cast<short*>(buf.get())) << endl;
			break;
			case NBT_ID_FLOAT:
				cout << "Float: ";
				readbuf_str(stringl);
				cout << buf << endl;
				readbuf(4);
				cout << htobe32(*reinterpret_cast<float*>(buf.get())) << endl;
			break;
			case NBT_ID_DOUBLE:
				cout << "Double: ";
				readbuf_str(stringl);
				cout << buf << endl;
				readbuf(8);
				cout << htobe64(*reinterpret_cast<double*>(buf.get())) << endl;
			break;
		}
	}

	return 0;
}