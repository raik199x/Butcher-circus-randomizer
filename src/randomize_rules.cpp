#include "randomize_rules.hpp"

RandomizeRules::RandomizeRules() {
  for (const auto &k_fighter : kFighters) {
    FighterRandomizeRules current_fighter;

    current_fighter.fighter_name    = k_fighter;
    current_fighter.is_participates = true;
    current_fighter.skills.fill(true);
    current_fighter.skills_enabled = kTotalFighterSpells;

    this->fighters_map.emplace(std::string(k_fighter), current_fighter);
  }
  this->amount_participates = kTotalNumberOfFighters;
}

RandomizeRules::~RandomizeRules() = default;

bool RandomizeRules::isTryingDisable(const bool setted, const bool requested) {
  return !requested && setted;
}

bool RandomizeRules::isTryingEnable(const bool setted, const bool requested) {
  return !RandomizeRules::isTryingDisable(setted, requested);
}

bool RandomizeRules::canReduceParticipants() const {
  return !(this->amount_participates == kRequiredNumberOfFighters);
}

bool RandomizeRules::canReduceSkills(const uint8_t skills_enabled) {
  return skills_enabled > kRequiredSkillsForFighter;
}

uint8_t RandomizeRules::getAmountParticipates() const {
  return this->amount_participates;
}

int RandomizeRules::getAmountOfEnabledSkills(const std::string &fighter_name) {
  auto map_iter = this->fighters_map.find(fighter_name);
  if (map_iter == this->fighters_map.cend()) {
    return -1;
  }

  return map_iter->second.skills_enabled;
}

GetterSkillsResult RandomizeRules::getHeroSkillsStates(const std::string &fighter_name) {
  GetterSkillsResult result;
  auto               map_iter = this->fighters_map.find(fighter_name);
  if (map_iter == this->fighters_map.cend()) {
    result.code = RandomizeRulesReturnCodes::kNoHero;
    return result;
  }

  result.code                         = RandomizeRulesReturnCodes::kNoError;
  result.requestedFighterSpellsStates = map_iter->second.skills;

  return result;
}

GetterFighterResult RandomizeRules::getFighter(const std::string &fighter_name) {
  GetterFighterResult result;
  auto                map_iter = this->fighters_map.find(fighter_name);
  if (map_iter == this->fighters_map.cend()) {
    result.code = RandomizeRulesReturnCodes::kNoHero;
    return result;
  }

  result.code    = RandomizeRulesReturnCodes::kNoError;
  result.fighter = map_iter->second;

  return result;
}

std::vector<FighterRandomizeRules> RandomizeRules::getAllParticipates() {
  std::vector<FighterRandomizeRules> result;
  for (const auto &[key, val] : this->fighters_map) {
    if (val.is_participates) {
      result.emplace_back(val);
    }
  }

  return result;
}

RandomizeRulesReturnCodes RandomizeRules::reverseFighterRule(const std::string &fighter_name) {
  auto map_iter = this->fighters_map.find(fighter_name);
  if (map_iter == this->fighters_map.cend()) {
    return RandomizeRulesReturnCodes::kNoHero;
  }

  const bool fighter_participate_state = map_iter->second.is_participates;
  if (RandomizeRules::isTryingDisable(fighter_participate_state, !fighter_participate_state) &&
      !this->canReduceParticipants()) {
    return RandomizeRulesReturnCodes::kTooFewHeroes;
  }

  this->amount_participates +=
      RandomizeRules::isTryingDisable(fighter_participate_state, !fighter_participate_state) ? -1 : 1;
  map_iter->second.is_participates = !fighter_participate_state;

  return RandomizeRulesReturnCodes::kNoError;
}

RandomizeRulesReturnCodes RandomizeRules::setFighterRule(const std::string &fighter_name, const bool new_state) {
  auto map_iter = this->fighters_map.find(fighter_name);
  if (map_iter == this->fighters_map.cend()) {
    return RandomizeRulesReturnCodes::kNoHero;
  }

  if (RandomizeRules::isTryingDisable(map_iter->second.is_participates, new_state) && !this->canReduceParticipants()) {
    return RandomizeRulesReturnCodes::kTooFewHeroes;
  }

  if (new_state != map_iter->second.is_participates) {
    map_iter->second.is_participates = new_state;
    this->amount_participates += new_state ? 1 : -1;
  }

  return RandomizeRulesReturnCodes::kNoError;
}

RandomizeRulesReturnCodes RandomizeRules::reverseSkillRule(const std::string &fighter_name, const uint8_t skill_index) {
  auto map_iter = this->fighters_map.find(fighter_name);
  if (map_iter == this->fighters_map.cend()) {
    return RandomizeRulesReturnCodes::kNoHero;
  }

  if (map_iter->second.fighter_name == RandomizeRules::kHeroWithAllSkills) {
    return RandomizeRulesReturnCodes::kForbiddenStateChange;
  }

  if (skill_index >= kTotalFighterSpells) {
    return RandomizeRulesReturnCodes::kOutOfRange;
  }

  const bool skill_state = map_iter->second.skills[skill_index];
  if (isTryingDisable(skill_state, !skill_state) && !RandomizeRules::canReduceSkills(map_iter->second.skills_enabled)) {
    return RandomizeRulesReturnCodes::kTooFewSpells;
  }

  map_iter->second.skills_enabled += isTryingDisable(skill_state, !skill_state) ? -1 : 1;
  map_iter->second.skills[skill_index] = !skill_state;

  return RandomizeRulesReturnCodes::kNoError;
}

RandomizeRulesReturnCodes RandomizeRules::setSkillRule(const std::string &fighter_name, const uint8_t skill_index,
                                                       const bool new_state) {
  auto map_iter = this->fighters_map.find(fighter_name);
  if (map_iter == this->fighters_map.cend()) {
    return RandomizeRulesReturnCodes::kNoHero;
  }

  if (map_iter->second.fighter_name == RandomizeRules::kHeroWithAllSkills) {
    return RandomizeRulesReturnCodes::kForbiddenStateChange;
  }

  if (skill_index >= kTotalFighterSpells) {
    return RandomizeRulesReturnCodes::kOutOfRange;
  }

  if (map_iter->second.skills[skill_index] != new_state) {
    if (isTryingDisable(map_iter->second.skills[skill_index], new_state) &&
        !RandomizeRules::canReduceSkills(map_iter->second.skills_enabled)) {
      return RandomizeRulesReturnCodes::kTooFewSpells;
    }

    map_iter->second.skills_enabled += isTryingDisable(map_iter->second.skills[skill_index], new_state) ? -1 : 1;
    map_iter->second.skills[skill_index] = new_state;
  }

  return RandomizeRulesReturnCodes::kNoError;
}