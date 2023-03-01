#pragma once

#include <QString>
#include <fstream>
#include <iostream>
#include <vector>

bool recreate(std::string fileName);
//! \note forbids/allows hero/spell randomizing
int changeLine(std::string fileName, std::string heroName, int what, int AccessibleHeroes);
QString *getPossibleSkills(int numCommand, QString *fighters);
std::vector<std::string> getPossibleHeroes(std::string fileName);
