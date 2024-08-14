#pragma once

#include <cstdint>

/*!
 * \file config.h
 * \author Timuri Alvarez
 * \brief File for global configuration and constants
 */

constexpr uint8_t kTotalNumberOfFighters    = 18;
constexpr uint8_t kTotalFighterSpells       = 7;
constexpr uint8_t kRequiredNumberOfFighters = 4;
constexpr uint8_t kRequiredSkillsForFighter = 4;

// The crowd of fighters
const char *const kFighters[kTotalNumberOfFighters] = {
    "abomination",  "antiquarian", "bounty_hunter", "crusader",      "arbalest",      "flagellant",
    "grave_robber", "hellion",     "highwayman",    "houndmaster",   "jester",        "leper",
    "man_at_arms",  "musketeer",   "occultist",     "plague_doctor", "shieldbreaker", "vestal"};
