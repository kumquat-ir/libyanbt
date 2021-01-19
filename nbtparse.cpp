#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <endian.h>
#include "nbtparse.h"
#include "nbttag.h"
#include "nbtfile.h"

using namespace std;

/* im using these specific snippets a lot this just makes it so much easier 
   dont use these outside of parse() */
/* read amt characters from nbtfilein and store them in buf */
#define readbuf( amt ) { buf.reset(new char[amt]); nbtfilein.read(buf.get(), amt); }
/* read an unsigned short from nbtfilein and store the result in res */
#define readbuf_ushort( res ) { buf.reset(new char[2]); nbtfilein.read(buf.get(), 2); res = htobe16(*reinterpret_cast<unsigned short*>(buf.get())); }
/* read a length-prefixed string from nbtfilein, store the length in res, null-terminate the string and store it in buf */
#define readbuf_str( res ) { readbuf_ushort( res ); if(res > 0) {\
	buf.reset(new char[res + 1]); nbtfilein.read(buf.get(), res);\
	buf[res] = '\x00';\
} }

int parse(ifstream &nbtfilein){
	nbtfilein.seekg(ios::beg);

	unique_ptr<char[]> buf;
	unsigned short stringl;
	vector<unique_ptr<nbttag>> parents = {};
	nbttag root_tag;

	readbuf(1);
	switch (buf[0]){
		case NBT_ID_COMPOUND:
			readbuf_str(stringl);
			root_tag = nbttag(NBT_ID_COMPOUND, static_cast<string>(buf.get()));
		break;
		case NBT_ID_LIST:
			//TODO, eventually
			cout << "No support for root list tags yet" << endl;
			return 1;
		break;
	}
	cout << root_tag << endl;
	parents.push_back(static_cast<unique_ptr<nbttag>>(&root_tag));

	nbtfile nbtdata = nbtfile("default_filename", &root_tag);

	while(!nbtfilein.eof()){
		readbuf(1);
		/* display floats/doubles more precisely */
		cout << setprecision(17);
		switch(buf[0]){
			case NBT_ID_COMPOUND:
				readbuf_str(stringl);
				parents.back()->add_child(new nbttag(NBT_ID_COMPOUND, static_cast<string>(buf.get())));
				parents.push_back(static_cast<unique_ptr<nbttag>>(parents.back()->get_last_child()));
			break;
			case NBT_ID_STRING:
				cout << "String: ";
				readbuf_str(stringl);
				cout << buf << endl << "\t";
				parents.back()->add_child(new nbttag(NBT_ID_STRING, static_cast<string>(buf.get())));
				readbuf_str(stringl);
				parents.back()->get_last_child()->init_payload(static_cast<string>(buf.get()));
				cout << buf << endl;
			break;
			case NBT_ID_END:
				cout << "End" << endl;
				if(parents.back()->type == NBT_ID_COMPOUND){
					parents.back().release();
					parents.pop_back();
					cout << "popped!" << endl;
				}
			break;
			case NBT_ID_BYTE:
				cout << "Byte: ";
				readbuf_str(stringl);
				cout << buf << endl;
				parents.back()->add_child(new nbttag(NBT_ID_BYTE, static_cast<string>(buf.get())));
				readbuf(1);
				cout << static_cast<unsigned char>(buf[0]) << endl;
			break;
			case NBT_ID_INT:
				cout << "Int: ";
				readbuf_str(stringl);
				cout << buf << endl;
				parents.back()->add_child(new nbttag(NBT_ID_INT, static_cast<string>(buf.get())));
				readbuf(4);
				cout << htobe32(*reinterpret_cast<int*>(buf.get())) << endl;
			break;
			case NBT_ID_LONG:
				cout << "Long: ";
				readbuf_str(stringl);
				cout << buf << endl;
				parents.back()->add_child(new nbttag(NBT_ID_LONG, static_cast<string>(buf.get())));
				readbuf(8);
				cout << htobe64(*reinterpret_cast<long*>(buf.get())) << endl;
			break;
			case NBT_ID_SHORT:
				cout << "Short: ";
				readbuf_str(stringl);
				cout << buf << endl;
				parents.back()->add_child(new nbttag(NBT_ID_SHORT, static_cast<string>(buf.get())));
				readbuf(2);
				cout << htobe16(*reinterpret_cast<short*>(buf.get())) << endl;
			break;
			case NBT_ID_FLOAT:
				cout << "Float: ";
				readbuf_str(stringl);
				cout << buf << endl;
				parents.back()->add_child(new nbttag(NBT_ID_FLOAT, static_cast<string>(buf.get())));
				readbuf(4);
				{unsigned int tempout = htobe32(*reinterpret_cast<unsigned int*>(buf.get()));
				cout << *reinterpret_cast<float*>(&tempout) << endl;}
			break;
			case NBT_ID_DOUBLE:
				cout << "Double: ";
				readbuf_str(stringl);
				parents.back()->add_child(new nbttag(NBT_ID_DOUBLE, static_cast<string>(buf.get())));
				cout << buf << endl;
				readbuf(8);
				{unsigned long tempout = htobe64(*reinterpret_cast<unsigned long*>(buf.get()));
				cout << *reinterpret_cast<double*>(&tempout) << endl;}
			break;
			case NBT_ID_BYTE_ARR:
				cout << "Byte arr: ";
				readbuf_str(stringl);
				parents.back()->add_child(new nbttag(NBT_ID_BYTE_ARR, static_cast<string>(buf.get())));
				cout << buf << endl;
				readbuf(4);
				{signed int tempout = htobe32(*reinterpret_cast<unsigned int*>(buf.get()));
				cout << "Length: " << tempout << endl;
				readbuf(tempout);}
			break;
			case NBT_ID_INT_ARR:
				cout << "Int arr: ";
				readbuf_str(stringl);
				parents.back()->add_child(new nbttag(NBT_ID_INT_ARR, static_cast<string>(buf.get())));
				cout << buf << endl;
				readbuf(4);
				{signed int tempout = htobe32(*reinterpret_cast<unsigned int*>(buf.get()));
				cout << "Length: " << tempout << endl;
				readbuf(tempout * 4);}
			break;
			case NBT_ID_LONG_ARR:
				cout << "Long arr: ";
				readbuf_str(stringl);
				parents.back()->add_child(new nbttag(NBT_ID_LONG_ARR, static_cast<string>(buf.get())));
				cout << buf << endl;
				readbuf(4);
				{signed int tempout = htobe32(*reinterpret_cast<unsigned int*>(buf.get()));
				cout << "Length: " << tempout << endl;
				readbuf(tempout * 8);}
			break;
		}
	}
	cout << nbtdata << endl;

	return 0;
}