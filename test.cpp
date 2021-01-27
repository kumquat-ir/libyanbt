#include <iostream>
#include <filesystem>
#include "nbtfile.h"
#include "nbtread.h"

using namespace std;

int main(int argc, char** argv){
	if(argc <= 1){
		cerr << "no file provided" << endl;
		return 1;
	}

	nbtfile nbtdata = read_nbt(argv[1]);
	cout << nbtdata << endl;
	nbtdata.write_file(filesystem::temp_directory_path() / "yanbt_out.nbt");

	return 0;
}