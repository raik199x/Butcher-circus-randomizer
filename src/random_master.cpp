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
    for (auto &iter_squad : fighters) {
      // if already equipped
      if (!iter_squad.trinket1.isEmpty() && !iter_squad.trinket2.isEmpty()) {
        continue;
      }

      if (randomized_trinket.hero_restriction != manager.kTrinketWithoutHeroLimit &&
          randomized_trinket.hero_restriction != iter_squad.name) {
        continue;
      }

      // Giving trinket
      if (iter_squad.trinket1.isEmpty()) {
        iter_squad.trinket1 = randomized_trinket.name;
      } else if (iter_squad.trinket2.isEmpty() && randomized_trinket.name != iter_squad.trinket1) {
        iter_squad.trinket2 = randomized_trinket.name;
      }

      trinkets_randomized++;
    }

    // Removing used trinket
    possible_trinkets.erase(possible_trinkets.cbegin() + random_trinket_number);
  }

  return fighters;
}

squad RandomMaster::equipFighters(squad                                                               fighters,
                                  const std::array<FighterRandomizeRules, kRequiredNumberOfFighters> &fighters_rules) {
  for (uint8_t iter = 0; iter < kRequiredSkillsForFighter; iter++) {
    fighters[iter].name = QString::fromStdString(fighters_rules[iter].fighter_name);
  }

  return fighters;
}

squad RandomMaster::equipSKills(squad fighters, const std::array<QString, kRequiredNumberOfFighters> &skills) {
  for (uint8_t iter = 0; iter < kRequiredSkillsForFighter; iter++) {
    fighters[iter].skills = skills[iter];
  }

  return fighters;
}

squad RandomMaster::getFullRandomizedSquad(const std::shared_ptr<RandomizeRules> &rules, const uint8_t lvl) {
  squad result;

  std::array<FighterRandomizeRules, kRequiredNumberOfFighters> randomized_fighters = this->getFighters(rules);
  result = RandomMaster::equipFighters(result, randomized_fighters);

  std::array<QString, kRequiredNumberOfFighters> randomized_skills = RandomMaster::getSkills(randomized_fighters);
  result = RandomMaster::equipSKills(result, randomized_skills);

  result = RandomMaster::getAndEquipTrinkets(lvl, result);

  return result;
}