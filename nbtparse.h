#ifndef NBTPARSE_H
#define NBTPARSE_H

#include <fstream>

const char NBT_ID_END = '\x00';
const char NBT_ID_BYTE = '\x01';
const char NBT_ID_SHORT = '\x02';
const char NBT_ID_INT = '\x03';
const char NBT_ID_LONG = '\x04';
const char NBT_ID_FLOAT = '\x05';
const char NBT_ID_DOUBLE = '\x06';
const char NBT_ID_BYTE_ARR = '\x07';
const char NBT_ID_STRING = '\x08';
const char NBT_ID_LIST = '\x09';
const char NBT_ID_COMPOUND = '\x0a';
const char NBT_ID_INT_ARR = '\x0b';
const char NBT_ID_LONG_ARR = '\x0c';

/* parse nbt data from an open ifstream nbtfile */
int parse(std::ifstream &nbtfile);

#endif /* NBTPARSE_H */