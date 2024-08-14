#pragma once

#include <string>
#include <array>
#include <map>
#include <vector>

#include "config.h"

using FighterSpellsStateArray_t = std::array<bool, kTotalFighterSpells>;

enum class RandomizeRulesReturnCodes {
  kOutOfRange,
  kInvalidHeroesAmount,
  kNoHero,
  kHeroForbidden,
  kTooFewSpells,
  kTooFewHeroes,
  kNoError = 0
};

struct FighterRandomizeRules {
  bool                      is_participates;
  std::string               fighter_name;
  uint8_t                   skills_enabled;
  FighterSpellsStateArray_t skills;
};

struct GetterSpellsResult {
  RandomizeRulesReturnCodes code;
  FighterSpellsStateArray_t requestedFighterSpellsStates;
};

struct GetterFighterResult {
  RandomizeRulesReturnCodes code;
  FighterRandomizeRules     fighter;
};

class RandomizeRules {
public:
  RandomizeRules();
  virtual ~RandomizeRules();

  // Deleted since unimplemented
  RandomizeRules(const RandomizeRules &other)  = delete;
  RandomizeRules(const RandomizeRules &&other) = delete;

  [[nodiscard]] uint8_t                            getAmountParticipates() const;
  [[nodiscard]] int                                getAmountOfEnabledSkills(const std::string &fighter_name);
  [[nodiscard]] GetterSpellsResult                 getHeroSpellsStates(const std::string &fighter_name);
  [[nodiscard]] GetterFighterResult                getFighter(const std::string &fighter_name);
  [[nodiscard]] std::vector<FighterRandomizeRules> getAllParticipates();

  RandomizeRulesReturnCodes reverseFighterRule(const std::string &fighter_name);
  RandomizeRulesReturnCodes setFighterRule(const std::string &fighter_name, bool new_state);

  RandomizeRulesReturnCodes reverseSkillRule(const std::string &fighter_name, uint8_t skill_index);
  RandomizeRulesReturnCodes setSkillRule(const std::string &fighter_name, uint8_t skill_index, bool new_state);

private:
  uint8_t                                      amount_participates;
  std::map<std::string, FighterRandomizeRules> fighters_map;

  [[nodiscard]] static bool isTryingDisable(bool setted, bool requested);
  [[nodiscard]] static bool isTryingEnable(bool setted, bool requested);
  [[nodiscard]] bool        canReduceParticipants() const;
  [[nodiscard]] static bool canReduceSkills(uint8_t skills_enabled);
};