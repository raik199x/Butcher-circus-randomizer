#pragma once

#include <vector>
#include <array>

#include <QString>

#include "config.h"

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

std::string        getBaseFileContent();
bool               recreate(const std::string &fileName);
Errors::ChangeLine changeLine(const std::string &fileName, const std::string &heroName, const uint8_t &button_index,
                              uint8_t accessible_heroes);
std::vector<std::string> getPossibleHeroes(const std::string &fileName, bool suppress = false);

std::array<QString, kRequiredNumberOfFighters>
getPossibleSkills(const std::string &fileName, const std::array<QString, kRequiredNumberOfFighters> &fighters,
                  bool suppress = false);
