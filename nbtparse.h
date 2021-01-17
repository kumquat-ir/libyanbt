#ifndef NBTPARSE_H
#define NBTPARSE_H

#include <fstream>

/* parse nbt data from an open ifstream nbtfile */
int parse(std::ifstream &nbtfile);

#endif /* NBTPARSE_H */