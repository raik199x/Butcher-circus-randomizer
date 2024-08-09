#pragma once

#include <string>

using std::string;

/*!
 * \file config.h
 * \author Timuri Alvarez
 * \brief File for global configuration and constants
 */

constexpr static size_t kTotalNumberOfFighters    = 18;
constexpr size_t        kRequiredNumberOfFighters = 4;
constexpr size_t        kRequiredSpellsForFighter = 4;

// The crowd of fighters
const char *const kFighters[kTotalNumberOfFighters] = {
    "abomination",  "antiquarian", "bounty_hunter", "crusader",      "arbalest",      "flagellant",
    "grave_robber", "hellion",     "highwayman",    "houndmaster",   "jester",        "leper",
    "man_at_arms",  "musketeer",   "occultist",     "plague_doctor", "shieldbreaker", "vestal"};
