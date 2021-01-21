#ifndef NBTPARSE_H
#define NBTPARSE_H

#include <fstream>
#include <memory>
#include "nbtfile.h"

/* parse nbt data from an open ifstream of an uncompressed file into an nbtfile */
int parse(std::ifstream&, nbtfile&);

#endif /* NBTPARSE_H */