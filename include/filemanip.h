#ifndef FILEMANIP_H
#define FILEMANIP_H
#include <iostream>
#include <fstream>

bool recreate(std::string fileName, std::string *fighters); // recreates file with default settings
int changeLine(std::string fileName, std::string heroName, int what, int AccesableHeroes); // forbids/allows randoming hero/spell

#endif // FILEMANIP_H
