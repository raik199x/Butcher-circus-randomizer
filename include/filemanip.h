#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <array>

#include <QString>

/**
 * @file filemanip.h
 * @author raik
 * @brief Functions for working with files
 *
 * @note This header implementation might be used represented by two different files (filemanip.cpp and
 * filemanip_windows.cpp)
 */

namespace Errors {
enum class ChangeLine {
  OutOfRange          = -7, // internal error, should never happen under normal circumstances
  InvalidHeroesAmount = -6, // internal error, should never happen under normal circumstances
  NoHero              = -5,
  HeroForbidden       = -4,
  TooFewSpells        = -3,
  TooFewHeroes        = -2,
  NoFile              = -1,
  HeroRemoved         = 0,
  HeroAdded           = 1,
  SkillRemoved        = 2, // historically the value was 3
};
}

//! \note Tested as FileManip::Unit1
bool recreate(const std::string &fileName);
//! \note Tested as FileManip::Unit3
Errors::ChangeLine changeLine(const std::string &fileName, const std::string &heroName, uint8_t what,
                              size_t AccessibleHeroes);
//! \note Tested as FileManip::Unit2
std::vector<std::string> getPossibleHeroes(const std::string &fileName, const bool suppress = false);

//! \todo Test this function:
QString *d_getPossibleSkills(int numCommand, QString *fighters, const bool suppress = false);

std::array<QString, 4> getPossibleSkills(const std::string &fileName, const std::array<std::string, 4> fighters,
                                         const bool suppress = false);
