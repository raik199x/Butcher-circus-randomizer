#pragma once

#include <QString>

#include "config.h"
#include "randomize_rules.hpp"
#include "asset_manager.hpp"

struct Fighter {
  QString                                          name;
  QString                                          skills;
  std::array<QString, kMaxTrinketAmountForFighter> trinkets;
};

using squad = std::array<Fighter, kRequiredNumberOfFighters>;

class RandomMaster {
public:
  RandomMaster();
  virtual ~RandomMaster();

  // TODO(alexander): implement
  RandomMaster(const RandomMaster &other)       = delete;
  RandomMaster(const RandomMaster &&other)      = delete;
  RandomMaster &operator=(RandomMaster &other)  = delete;
  RandomMaster &operator=(RandomMaster &&other) = delete;

  static std::vector<TrinketInfo> parseLevelRestriction(const std::vector<TrinketInfo> &trinkets, uint8_t lvl);

  static std::array<FighterRandomizeRules, kRequiredNumberOfFighters>
  getFighters(const std::shared_ptr<RandomizeRules> &rules);

  static std::array<QString, kRequiredNumberOfFighters>
  getSkills(const std::array<FighterRandomizeRules, kRequiredNumberOfFighters> &fighters);

  static constexpr uint8_t kAmountOfTrinketsForTeam = 8;
  static squad             getAndEquipTrinkets(uint8_t lvl, squad fighters);

  static squad equipFighters(squad                                                               fighters,
                             const std::array<FighterRandomizeRules, kRequiredNumberOfFighters> &fighters_rules);
  static squad equipSKills(squad fighters, const std::array<QString, kRequiredNumberOfFighters> &skills);
  static squad getFullRandomizedSquad(const std::shared_ptr<RandomizeRules> &rules, uint8_t lvl);

private:
};