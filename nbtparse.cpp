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

int parse(ifstream &nbtfilein, nbtfile &nbtdata){
	nbtfilein.seekg(ios::beg);

	unique_ptr<char[]> buf;
	unsigned short stringl;
	vector<unique_ptr<nbttag>> parents = {};
	vector<bool> in_list = {};
	nbttag root_tag;
	bool list_del = false;
	/* type, number of elements remaining */
	vector<pair<char, int>> list_vars;

	readbuf(1);
	switch (buf[0]){
		case NBT_ID_COMPOUND:
			readbuf_str(stringl);
			root_tag = nbttag(NBT_ID_COMPOUND, static_cast<string>(buf.get()));
			in_list.push_back(false);
		break;
		case NBT_ID_LIST:
			//TODO, eventually
			cerr << "No support for root list tags yet" << endl;
			return 1;
		break;
	}
	parents.push_back(static_cast<unique_ptr<nbttag>>(&root_tag));

	while(true){
		char type;
		if(in_list.back() == false){
			readbuf(1);
			if(nbtfilein.eof())
				break;
			type = buf[0];

			if(type > NBT_ID_LONG_ARR || type < NBT_ID_END){
				cerr << "Invalid tag id! The file may be malformed or corrupted" << endl;
				return 1;
			}

			if(type == NBT_ID_END){
				if(parents.back()->type == NBT_ID_COMPOUND){
					parents.back().release();
					parents.pop_back();
					in_list.pop_back();
				}
				if(in_list.back() == true && list_vars.back().second <= 0){
					parents.back().release();
					parents.pop_back();
					list_vars.pop_back();
					in_list.pop_back();
					list_del = false;
				}
				continue;
			}

			readbuf_str(stringl);
			parents.back()->add_child(new nbttag(type, static_cast<string>(buf.get())));
		} else {
			type = list_vars.back().first;
			parents.back()->add_child(new nbttag(type));
			list_vars.back().second--;
		}

		switch(type){
			case NBT_ID_COMPOUND:
				parents.push_back(static_cast<unique_ptr<nbttag>>(parents.back()->get_last_child()));
				in_list.push_back(false);
			break;
			case NBT_ID_LIST:
				parents.push_back(static_cast<unique_ptr<nbttag>>(parents.back()->get_last_child()));
				in_list.push_back(true);
				readbuf(1);
				type = buf[0];
				readbuf(4);
				/* TODO properly parse as signed int. should be fine for now */
				list_vars.push_back(pair(type, (htobe32(*reinterpret_cast<int*>(buf.get())))));
			break;
			case NBT_ID_STRING:
				readbuf_str(stringl);
				parents.back()->get_last_child()->init_payload(static_cast<string>(buf.get()));
			break;
			case NBT_ID_END:
				cerr << "Unexpected TAG_END!" << endl;
				return 1;
			break;
			case NBT_ID_BYTE:
				readbuf(1);
				parents.back()->get_last_child()->init_payload(*reinterpret_cast<signed char*>(buf.get()));
			break;
			case NBT_ID_INT:
				readbuf(4);
				{unsigned int tempout = htobe32(*reinterpret_cast<unsigned int*>(buf.get()));
				parents.back()->get_last_child()->init_payload(*reinterpret_cast<int*>(&tempout));}
			break;
			case NBT_ID_LONG:
				readbuf(8);
				{unsigned long tempout = htobe64(*reinterpret_cast<unsigned long*>(buf.get()));
				parents.back()->get_last_child()->init_payload(*reinterpret_cast<long*>(&tempout));}
			break;
			case NBT_ID_SHORT:
				readbuf(2);
				{unsigned short tempout = htobe16(*reinterpret_cast<unsigned short*>(buf.get()));
				parents.back()->get_last_child()->init_payload(*reinterpret_cast<short*>(&tempout));}
			break;
			case NBT_ID_FLOAT:
				readbuf(4);
				{unsigned int tempout = htobe32(*reinterpret_cast<unsigned int*>(buf.get()));
				parents.back()->get_last_child()->init_payload(*reinterpret_cast<float*>(&tempout));}
			break;
			case NBT_ID_DOUBLE:
				readbuf(8);
				{unsigned long tempout = htobe64(*reinterpret_cast<unsigned long*>(buf.get()));
				parents.back()->get_last_child()->init_payload(*reinterpret_cast<double*>(&tempout));}
			break;
			case NBT_ID_BYTE_ARR:
				readbuf(4);
				{signed int tempout = htobe32(*reinterpret_cast<unsigned int*>(buf.get()));
				vector<signed char> tmpvec;
				for(int i = 0; i < tempout; i++){
					readbuf(1);
					tmpvec.push_back(*reinterpret_cast<signed char*>(buf.get()));
				}
				parents.back()->get_last_child()->init_payload(tmpvec);
				}
			break;
			case NBT_ID_INT_ARR:
				readbuf(4);
				{signed int tempout = htobe32(*reinterpret_cast<unsigned int*>(buf.get()));
				vector<signed int> tmpvec;
				for(int i = 0; i < tempout; i++){
					readbuf(4);
					unsigned int tempout2 = htobe32(*reinterpret_cast<unsigned int*>(buf.get()));
					tmpvec.push_back(*reinterpret_cast<signed int*>(&tempout2));
				}
				parents.back()->get_last_child()->init_payload(tmpvec);
				}
			break;
			case NBT_ID_LONG_ARR:
				readbuf(4);
				{signed int tempout = htobe32(*reinterpret_cast<unsigned int*>(buf.get()));
				vector<signed long> tmpvec;
				for(int i = 0; i < tempout; i++){
					readbuf(8);
					unsigned long tempout2 = htobe64(*reinterpret_cast<unsigned long*>(buf.get()));
					tmpvec.push_back(*reinterpret_cast<signed long*>(&tempout2));
				}
				parents.back()->get_last_child()->init_payload(tmpvec);
				}
			break;
		}

		if(in_list.back() == true && list_vars.back().second <= 0){
			parents.back().release();
			parents.pop_back();
			list_vars.pop_back();
			in_list.pop_back();
			list_del = false;
		}
	}
	nbtdata.init_root_tag(root_tag);

	return 0;
}