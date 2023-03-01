#ifndef FILEMANIP_H
#define FILEMANIP_H
#include <fstream>
#include <iostream>
#include <vector>

bool recreate(std::string fileName, std::string *fighters);																 // recreates file with default settings
int changeLine(std::string fileName, std::string heroName, int what, int AccesableHeroes); // forbids/allows hero/spell randomizing
vector<QString> getPossibleHeroes(std::string fileName);																	 // returns list of heroes that can be randomized

#endif // FILEMANIP_H
