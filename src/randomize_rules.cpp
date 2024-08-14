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
  return skills_enabled > kRequiredSpellsForFighter;
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

ChangeRuleReturnCode RandomizeRules::reverseFighterRule(const std::string &fighter_name) {
  auto map_iter = this->fighters_map.find(fighter_name);
  if (map_iter == this->fighters_map.cend()) {
    return ChangeRuleReturnCode::kNoHero;
  }

  const bool fighter_participate_state = map_iter->second.is_participates;
  if (RandomizeRules::isTryingDisable(fighter_participate_state, !fighter_participate_state) &&
      !this->canReduceParticipants()) {
    return ChangeRuleReturnCode::kTooFewHeroes;
  }

  this->amount_participates +=
      RandomizeRules::isTryingDisable(fighter_participate_state, !fighter_participate_state) ? -1 : 1;
  map_iter->second.is_participates = !fighter_participate_state;

  return ChangeRuleReturnCode::kNoError;
}

ChangeRuleReturnCode RandomizeRules::setFighterRule(const std::string &fighter_name, const bool new_state) {
  auto map_iter = this->fighters_map.find(fighter_name);
  if (map_iter == this->fighters_map.cend()) {
    return ChangeRuleReturnCode::kNoHero;
  }

  if (RandomizeRules::isTryingDisable(map_iter->second.is_participates, new_state) && !this->canReduceParticipants()) {
    return ChangeRuleReturnCode::kTooFewHeroes;
  }

  if (new_state != map_iter->second.is_participates) {
    map_iter->second.is_participates = new_state;
    this->amount_participates += new_state ? 1 : -1;
  }

  return ChangeRuleReturnCode::kNoError;
}

ChangeRuleReturnCode RandomizeRules::reverseSkillRule(const std::string &fighter_name, const uint8_t skill_index) {
  auto map_iter = this->fighters_map.find(fighter_name);
  if (map_iter == this->fighters_map.cend()) {
    return ChangeRuleReturnCode::kNoHero;
  }

  if (skill_index >= kTotalFighterSpells) {
    return ChangeRuleReturnCode::kOutOfRange;
  }

  const bool skill_state = map_iter->second.skills[skill_index];
  if (isTryingDisable(skill_state, !skill_state) &&
      !RandomizeRules::canReduceSkills(map_iter->second.skills_enabled)) {
    return ChangeRuleReturnCode::kTooFewSpells;
  }

  map_iter->second.skills_enabled += isTryingDisable(skill_state, !skill_state) ? -1 : 1;
  map_iter->second.skills[skill_index] = !skill_state;

  return ChangeRuleReturnCode::kNoError;
}

ChangeRuleReturnCode RandomizeRules::setSkillRule(const std::string &fighter_name, const uint8_t skill_index,
                                                  const bool new_state) {
  auto map_iter = this->fighters_map.find(fighter_name);
  if (map_iter == this->fighters_map.cend()) {
    return ChangeRuleReturnCode::kNoHero;
  }

  if (skill_index >= kTotalFighterSpells) {
    return ChangeRuleReturnCode::kOutOfRange;
  }

  if (map_iter->second.skills[skill_index] != new_state) {
    if (isTryingDisable(map_iter->second.skills[skill_index], new_state) &&
        !RandomizeRules::canReduceSkills(map_iter->second.skills_enabled)) {
      return ChangeRuleReturnCode::kTooFewSpells;
    }

    map_iter->second.skills_enabled += isTryingDisable(map_iter->second.skills[skill_index], new_state) ? -1 : 1;
    map_iter->second.skills[skill_index] = new_state;
  }

  return ChangeRuleReturnCode::kNoError;
}