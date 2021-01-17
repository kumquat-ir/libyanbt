#include <string>

using namespace std;

class nbttag {
public:
	char type;
	bool named;
	string name;
};

class nbttag_compound : public nbttag {
public:
	int length;
};

class nbttag_list : public nbttag {
public:
	int length;
	char type_contains;
};

class nbttag_arr : public nbttag {
public:
	int length;
};