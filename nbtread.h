#ifndef NBTREAD_H
#define NBTREAD_H

#include <filesystem>
#include <memory>
#include "nbtfile.h"

void print_hex(std::unique_ptr<char[]>&, int, int);
char* checkmagic(std::filesystem::path ifpath);
nbtfile read_nbt(std::filesystem::path ifpath);


#endif /* NBTREAD_H */