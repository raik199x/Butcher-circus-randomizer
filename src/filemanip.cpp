#include <fstream>
#include <vector>

#include <QMessageBox>

#include "../include/config.h"
#include "../include/filemanip.h"

/**
 * @file filemanip.cpp
 * @author raik
 * @brief This file contains the functions for the file manipulation (Linux
 * aimed)
 */

/**
 * @brief Recreates file with default settings
 *
 * @param fileName
 * @param fighters Default list of heroes
 * @return true if file was recreated
 * @return false if file wasn't recreated
 */
[[maybe_unused]] bool recreate(const std::string &fileName) {
  std::ofstream file(fileName);
  if (!file)
    return false;
  for (int i = 0; i < 18; i++) {
    //! \note something strange was here:
    //		  std::string line = fighters[i] + ":" + std::string(8,
    //'1')['\n'];
    file << fighters[i] << ":" << std::string(8, '1') << '\n';
  }
  file.close();
  return true;
}

/**
 * @brief Changes line in file
 *
 * @param fileName
 * @param heroName which specific hero need to be changed
 * @param what which specific argument need to be changed (numeration begins
 * from 0)
 * @param AccessibleHeroes amount of heroes that can be randomized
 * @return int result code of operation
 */
[[nodiscard]] Errors::ChangeLine changeLine(const std::string &fileName, const std::string &heroName, uint8_t what,
                                            size_t AccessibleHeroes) {
  using namespace Errors;
  if (what < 0 || what > 7)
    return ChangeLine::OutOfRange;
  if (AccessibleHeroes < 4)
    return ChangeLine::InvalidHeroesAmount;
  if (heroName == "abomination" && what != 0)
    return ChangeLine::HeroForbidden;

  std::fstream file(fileName);
  if (!file)
    return ChangeLine::NoFile;

  Errors::ChangeLine returnCode;
  while (1) {
    std::string line;
    file >> line;
    if (file.eof())
      return ChangeLine::NoHero;
    if (line.find(heroName) == std::string::npos)
      continue;
    file.seekp(-line.size(), std::ios::cur); // moving back in file
    // finding pos of first
    size_t posStart = 0;
    while (line[posStart] != ':')
      posStart++;
    posStart++;
    if (what == 0 && AccessibleHeroes == 4 && line[posStart + what] == '1')
      return ChangeLine::TooFewHeroes;
    else if (what == 0 && line[posStart + what] == '1')
      returnCode = ChangeLine::HeroRemoved;
    else if (what == 0 && line[posStart + what] == '0')
      returnCode = ChangeLine::HeroAdded;
    else
      returnCode = ChangeLine::SkillRemoved;
    int checkSpells = 0;
    for (unsigned int i = posStart + 1; i < line.length(); i++)
      if (line[i] == '1')
        checkSpells++;
    if (checkSpells == 4 && line[posStart + what] == '1' && what != 0)
      return ChangeLine::TooFewSpells;
    line[posStart + what] == '0' ? line[posStart + what] = '1' : line[posStart + what] = '0';
    file << line + '\n';
    break;
  }

  return returnCode;
}

/*!
 * \brief Get the Possible Heroes function
 *
 * \param fileName The name of the file to be parsed
 * \return std::string* List of all possible heroes
 */
[[nodiscard]] std::vector<std::string> getPossibleHeroes(const std::string &fileName, const bool suppress) {
  std::ifstream file(fileName);
  if (!file) {
    if (!suppress)
      QMessageBox::critical(nullptr, "Cannot open file", "For some reason BCR cannot open file for reading");
    return {};
  }
  std::vector<string> possibleHeroes;
  for (int i = 0; i < 18; i++) {
    string line;
    file >> line;
    if (line[line.find(":") + 1] == '1')
      possibleHeroes.push_back(fighters[i]);
  }
  return possibleHeroes;
}

/**
 * @brief Get the Skills
 *
 * @param numCommand
 * @param fighters already randomized fighters
 * @return QString* array of possible skills for heroes in @param fighters or
 * nullptr if something went wrong
 */
[[nodiscard, deprecated]] QString *d_getPossibleSkills(int numCommand, QString *fighters, const bool suppress) {
  std::ifstream file(numCommand == 0 ? "BCR_T1.txt" : "BCR_T2.txt");
  if (!file) {
    if (!suppress)
      QMessageBox::critical(nullptr, "Cannot open file", "For some reason BCR cannot open file for reading");
    return nullptr;
  }
  QString *result;
  result       = new QString[4];
  size_t index = 0;

  for (int i = 0; i < 18; i++) {
    string line;
    file >> line;

    QString heroName = QString::fromStdString(line.substr(0, line.find(":"))); // cut line to get only hero's name
    bool    found    = false;
    for (int j = 0; j < 4; j++)
      if (heroName == fighters[j]) {
        found = true;
        break;
      }
    if (!found)
      continue;

    // getting indexes of available skills
    string skills = line.substr(line.find(":") + 2);
    for (size_t j = 0; j < skills.length(); j++)
      if (skills[j] == '1')
        result[index] += QString::number(j + 1);

    if (++index == 4)
      break;
  }
  return result;
}

std::array<QString, 4> getPossibleSkills(const std::string &fileName, const std::array<std::string, 4> fighters,
                                         const bool suppress) {
  std::array<QString, 4> result;
  std::ifstream          file(fileName);
  if (!file) {
    if (!suppress)
      QMessageBox::critical(nullptr, "Cannot open file", "For some reason BCR cannot open file for reading");
    return result;
  }
  size_t index = 0;
  for (int i = 0; i < NUMBER_OF_FIGHTERS; ++i) {
    string line;
    file >> line;
    string heroName = line.substr(0, line.find(":"));
    bool   found    = false;
    for (int j = 0; j < 4; j++)
      if (heroName == fighters[j]) {
        found = true;
        break;
      }
    if (!found)
      continue;

    // getting indexes of available skills
    string skills = line.substr(line.find(":") + 2);
    for (size_t j = 0; j < skills.length(); j++)
      if (skills[j] == '1')
        result[index] += QString::number(j + 1);
    if (++index == 4)
      break;
  }
  return result;
}
