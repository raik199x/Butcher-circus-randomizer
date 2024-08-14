#pragma once

#include <string>
#include <array>
#include <map>

#include "config.h"

enum class ChangeRuleReturnCode {
  kOutOfRange,
  kInvalidHeroesAmount,
  kNoHero,
  kHeroForbidden,
  kTooFewSpells,
  kTooFewHeroes,
  kNoError = 0
};

struct FighterRandomizeRules {
  bool                                  is_participates;
  std::string                           fighter_name;
  uint8_t                               skills_enabled;
  std::array<bool, kTotalFighterSpells> skills;
};

class RandomizeRules {
public:
  RandomizeRules();
  virtual ~RandomizeRules();

  // Deleted since unimplemented
  RandomizeRules(const RandomizeRules &other)  = delete;
  RandomizeRules(const RandomizeRules &&other) = delete;

  [[nodiscard]] uint8_t getAmountParticipates() const;
  int                   getAmountOfEnabledSkills(const std::string &fighter_name);

  ChangeRuleReturnCode reverseFighterRule(const std::string &fighter_name);
  ChangeRuleReturnCode setFighterRule(const std::string &fighter_name, bool new_state);

  ChangeRuleReturnCode reverseSkillRule(const std::string &fighter_name, uint8_t skill_index);
  ChangeRuleReturnCode setSkillRule(const std::string &fighter_name, uint8_t skill_index, bool new_state);

private:
  uint8_t                                      amount_participates;
  std::map<std::string, FighterRandomizeRules> fighters_map;

  [[nodiscard]] static bool isTryingDisable(bool setted, bool requested);
  [[nodiscard]] static bool isTryingEnable(bool setted, bool requested);
  [[nodiscard]] bool        canReduceParticipants() const;
  [[nodiscard]] static bool canReduceSkills(uint8_t skills_enabled);
};