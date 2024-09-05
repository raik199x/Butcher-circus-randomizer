#include <algorithm>

#include "random_master.hpp"
#include "random.h"

RandomMaster::RandomMaster()  = default;
RandomMaster::~RandomMaster() = default;

/**
 * @brief Get array of randomly selected fighters
 *
 * @param rules
 * @return array of randomized fighters
 */
std::array<FighterRandomizeRules, kRequiredNumberOfFighters>
RandomMaster::getFighters(const std::shared_ptr<RandomizeRules> &rules) {
  std::vector<FighterRandomizeRules>                           participators = rules->getAllParticipates();
  std::array<FighterRandomizeRules, kRequiredNumberOfFighters> result;

  // randomize heroes
  for (int iter_getting_hero = 0; iter_getting_hero < kRequiredNumberOfFighters; iter_getting_hero++) {
    uint8_t random_value      = Random::Uniform::integral(0, static_cast<int>(participators.size() - 1));
    result[iter_getting_hero] = participators[random_value];
    participators.erase(participators.cbegin() + random_value);
  }

  return result;
}

/**
 * @brief Get already randomized QString of skills
 *
 * @param Fighters Names of fighters
 * @return array of QString (format ex: "1234" "1234" "1234" "1234"")
 */
std::array<QString, kRequiredNumberOfFighters>
RandomMaster::getSkills(const std::array<FighterRandomizeRules, kRequiredNumberOfFighters> &fighters) {
  std::array<QString, kRequiredNumberOfFighters> skills;

  for (size_t iter_fighter = 0; iter_fighter < fighters.size(); iter_fighter++) {
    QString skill_indexes = "";
    for (size_t iter_states = 0; iter_states < fighters[iter_fighter].skills.size(); iter_states++) {
      if (fighters[iter_fighter].skills[iter_states]) {
        skill_indexes += QString::number(iter_states + 1);
      }
    }

    skills[iter_fighter] = skill_indexes;
  }

  std::array<QString, kRequiredSkillsForFighter> result; // skills for each hero
  for (size_t iter_fighters = 0; iter_fighters < fighters.size(); iter_fighters++) {
    result[iter_fighters] = "";
    if (fighters[iter_fighters].fighter_name == kFighterWithAllSkills) {
      continue;
    }

    for (size_t iter_skills = 0; iter_skills < kRequiredSkillsForFighter; iter_skills++) {
      int get_num = Random::Uniform::integral(0, (int)skills[iter_fighters].size() -
                                                     1);       // TODO(alexander): return -1 if skills empty
      result[iter_fighters] += skills[iter_fighters][get_num]; // TODO(alexander): will break if skills empty
      // delete symbol from QString
      skills[iter_fighters].remove(get_num, 1);
    }
  }

  return result;
}

/**
 * @brief Parses asset trinket entrees to remove trinkets that not suitable by level.
 *
 * @param trinkets list of trinkets
 * @param lvl player level
 * @return std::vector<TrinketInfo> vector containing trinkets that only suitable for current level.
 */
std::vector<TrinketInfo> RandomMaster::parseLevelRestriction(const std::vector<TrinketInfo> &trinkets,
                                                             const uint8_t                   lvl) {
  std::vector<TrinketInfo> result;

  for (const auto &iter : trinkets) {
    if (iter.level_restriction <= lvl) {
      result.emplace_back(iter);
    }
  }

  return result;
}

/**
 * @brief Randomize and sets trinket to fighters.
 *
 * @param lvl Current account level
 * @param fighters already randomized fighters
 * @return squad modified squad which now has equipped trinkets
 */
squad RandomMaster::getAndEquipTrinkets(const uint8_t lvl, squad fighters) {
  AssetManagerSingletone &manager = AssetManagerSingletone::getInstance();
  if (!manager.isTrinketsParsed()) { // TODO(alexander): manager can return false
    manager.loadTrinkets(kQtAssetTrinketList);
  }

  std::vector<TrinketInfo> possible_trinkets = manager.getTrinkets();
  possible_trinkets                          = RandomMaster::parseLevelRestriction(possible_trinkets, lvl);

  uint8_t trinkets_randomized = 0;
  while (trinkets_randomized != kAmountOfTrinketsForTeam) {
    if (possible_trinkets.empty()) {
      break; // TODO(alexander): Add something for empty trinkets
    }

    int random_trinket_number = static_cast<int>(possible_trinkets.size() - 1);
    random_trinket_number     = Random::Uniform::integral(0, random_trinket_number);

    TrinketInfo randomized_trinket = possible_trinkets[random_trinket_number];
    // Searching for fighter to equip trinket
    for (auto &iter_fighter : fighters) {
      // if fighter already has all trinkets
      if (std::all_of(iter_fighter.trinkets.cbegin(), iter_fighter.trinkets.cend(),
                      [](const QString &trinket) { return !trinket.isEmpty(); })) {
        continue;
      }

      if (randomized_trinket.hero_restriction != manager.kTrinketWithoutHeroLimit &&
          randomized_trinket.hero_restriction != iter_fighter.name) {
        continue;
      }

      // trying to give trinket to a fighter
      // TODO(alexander): move to separate function
      bool is_trinket_given = false;
      for (auto &trinket : iter_fighter.trinkets) {
        if (trinket.isEmpty()) {
          trinket          = randomized_trinket.name;
          is_trinket_given = true;
          break;
        }

        if (trinket == randomized_trinket.name) {
          break;
        }
      }

      if (is_trinket_given) {
        trinkets_randomized++;
        break; // Start random of next trinket
      }
      // If in did not gave trinket, we should check if other heroes can take this trinket
    }

    // Removing used trinket
    possible_trinkets.erase(possible_trinkets.cbegin() + random_trinket_number);
  }

  return fighters;
}

/**
 * @brief Sets randomized fighters into squad
 *
 * @param fighters squad of fighters
 * @param fighters_rules randomized fighters, that stored in FighterRandomizeRules struct
 * @return squad squad with setted fighters
 */
squad RandomMaster::equipFighters(squad                                                               fighters,
                                  const std::array<FighterRandomizeRules, kRequiredNumberOfFighters> &fighters_rules) {
  for (uint8_t iter = 0; iter < kRequiredSkillsForFighter; iter++) {
    fighters[iter].name = QString::fromStdString(fighters_rules[iter].fighter_name);
  }

  return fighters;
}

/**
 * @brief Sets skills for randomized squad
 *
 * @param fighters randomized fighters
 * @param skills randomized skills
 * @return squad squad with setted skills
 */
squad RandomMaster::equipSKills(squad fighters, const std::array<QString, kRequiredNumberOfFighters> &skills) {
  for (uint8_t iter = 0; iter < kRequiredSkillsForFighter; iter++) {
    fighters[iter].skills = skills[iter];
  }

  return fighters;
}

/**
 * @brief Helper function to perform all required calls to get fully equipped randomized team.
 *
 * @param rules randomize rules for current player
 * @param lvl account level
 * @return squad fully equipped randomized team.
 */
squad RandomMaster::getFullRandomizedSquad(const std::shared_ptr<RandomizeRules> &rules, const uint8_t lvl) {
  squad result;

  std::array<FighterRandomizeRules, kRequiredNumberOfFighters> randomized_fighters = RandomMaster::getFighters(rules);
  result = RandomMaster::equipFighters(result, randomized_fighters);

  std::array<QString, kRequiredNumberOfFighters> randomized_skills = RandomMaster::getSkills(randomized_fighters);
  result = RandomMaster::equipSKills(result, randomized_skills);

  result = RandomMaster::getAndEquipTrinkets(lvl, result);

  return result;
}