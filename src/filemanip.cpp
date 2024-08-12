#include <fstream>
#include <vector>

#include <QMessageBox>

#include "heroselection.h"
#include "filemanip.h"

/**
 * @brief Get the Base File Content object
 *
 * @return std::string
 */
std::string getBaseFileContent() {
  std::string result;

  for (const auto &k_fighter : kFighters) {
    result += std::string(k_fighter) + ':' +
              std::string(HeroSelection::kAmountOfButtonsForEachFighter, HeroSelection::kStateEnable) + '\n';
  }

  return result;
}

/**
 * @brief Recreates file with default settings
 *
 * @param fileName Path to file
 * @return true if file was recreated
 * @return false if file wasn't recreated
 */
bool recreate(const std::string &fileName) {
  std::ofstream file(fileName);
  if (!file) {
    return false;
  }
  file << getBaseFileContent();
  file.close();

  return true;
}

/**
 * @brief Changes line in file
 *
 * @param fileName
 * @param heroName which specific hero need to be changed
 * @param button_index which specific argument need to be changed (numeration begins
 * from 0)
 * @param accessible_heroes amount of heroes that not banned for randomizing
 * @return int result code of operation
 */
Errors::ChangeLine changeLine(const std::string &fileName, const std::string &heroName, const uint8_t &button_index,
                              const uint8_t accessible_heroes) {
  if (button_index > HeroSelection::kAmountOfButtonsForEachFighter - 1) {
    return Errors::ChangeLine::OutOfRange;
  }
  if (accessible_heroes < kRequiredNumberOfFighters) {
    return Errors::ChangeLine::InvalidHeroesAmount;
  }
  if (heroName == HeroSelection::kHeroWithAllSkills && button_index != HeroSelection::kHeroStatusSwitcherButtonIndex) {
    return Errors::ChangeLine::HeroForbidden;
  }

  std::fstream file(fileName);
  if (!file) {
    return Errors::ChangeLine::NoFile;
  }

  Errors::ChangeLine return_code;
  std::string        line;
  do {
    file >> line;
    if (file.eof()) {
      return Errors::ChangeLine::NoHero;
    }
  } while (line.find(heroName) == std::string::npos);

  file.seekp(-line.size(), std::ios::cur); // moving back in file
  // finding pos of first
  size_t pos_start = 0;
  while (line[pos_start] != ':') {
    pos_start++;
  }
  pos_start++;
  if (button_index == HeroSelection::kHeroStatusSwitcherButtonIndex && accessible_heroes <= kRequiredNumberOfFighters &&
      line[pos_start + button_index] == HeroSelection::kStateEnable) {
    return Errors::ChangeLine::TooFewHeroes;
  }
  if (button_index == HeroSelection::kHeroStatusSwitcherButtonIndex &&
      line[pos_start + button_index] == HeroSelection::kStateEnable) {
    return_code = Errors::ChangeLine::HeroRemoved;
  } else if (button_index == 0 && line[pos_start + button_index] == HeroSelection::kStateDisabled) {
    return_code = Errors::ChangeLine::HeroAdded;
  } else {
    return_code = Errors::ChangeLine::SkillRemoved;
  }

  uint8_t check_spells = 0;
  for (unsigned int i = pos_start + 1; i < line.length(); i++) {
    if (line[i] == HeroSelection::kStateEnable) {
      check_spells++;
    }
  }
  if (check_spells == kRequiredSpellsForFighter && line[pos_start + button_index] == HeroSelection::kStateEnable &&
      button_index != 0) {
    return Errors::ChangeLine::TooFewSpells;
  }
  line[pos_start + button_index] == HeroSelection::kStateDisabled
      ? line[pos_start + button_index] = HeroSelection::kStateEnable
      : line[pos_start + button_index] = HeroSelection::kStateDisabled;

  file << line + '\n';

  return return_code;
}

/*!
 * \brief Get the Possible Heroes function
 *
 * \param fileName The name of the file to be parsed
 * \return std::string* List of all possible heroes
 */
std::vector<std::string> getPossibleHeroes(const std::string &fileName, const bool suppress) {
  std::ifstream file(fileName);
  if (!file && !suppress) {
    QMessageBox::critical(nullptr, "Cannot open file", "For some reason BCR cannot open file for reading");
    return {};
  }
  std::vector<std::string> possible_heroes;
  for (const auto &k_fighter : kFighters) {
    std::string line;
    file >> line;
    if (line[line.find(':') + 1] == HeroSelection::kStateEnable) {
      possible_heroes.emplace_back(k_fighter);
    }
  }
  return possible_heroes;
}

/**
 * @brief Get the Possible Skills object
 *
 * @param fileName Path to file for parsing
 * @param fighters Already selected fighters, skills of which will be collected
 * @param suppress For testing purpose. Do we need to suppress warnings.
 * @return std::array<QString, kRequiredNumberOfFighters> numbers of enable skills
 */
std::array<QString, kRequiredNumberOfFighters>
getPossibleSkills(const std::string &fileName, const std::array<QString, kRequiredNumberOfFighters> &fighters,
                  const bool suppress) {
  std::array<QString, kRequiredNumberOfFighters> result;
  std::ifstream                                  file(fileName);

  if (!file && !suppress) {
    QMessageBox::critical(nullptr, "Cannot open file", "For some reason BCR cannot open file for reading");
    return result;
  }

  uint8_t index = 0;
  for (uint8_t iter_all_fighters = 0; iter_all_fighters < kTotalNumberOfFighters && index != kRequiredNumberOfFighters;
       ++iter_all_fighters) {
    std::string line;
    file >> line;
    std::string hero_name(line.substr(0, line.find(':')));
    bool        is_required = false;
    // Checking if entry from file is one of requested hero
    for (int iter_selected_fighters = 0; iter_selected_fighters < kRequiredNumberOfFighters; iter_selected_fighters++) {
      if (QString::fromStdString(hero_name) == fighters[iter_selected_fighters]) {
        is_required = true;
        break;
      }
    }

    if (!is_required) {
      continue;
    }

    // getting indexes of available skills
    std::string skills = line.substr(line.find(':') + 2); // 1 to skip ':' and 1 to skip index of enabled hero
    for (size_t j = 0; j < skills.length(); j++) {
      if (skills[j] == HeroSelection::kStateEnable) {
        result[index] += QString::number(j + 1);
      }
    }
    index++;
  }

  file.close();
  return result;
}
