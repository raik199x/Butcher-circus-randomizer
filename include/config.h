#pragma once

#include <string>

using std::string;

/*!
 * \file config.h
 * \author Timuri Alvarez
 * \brief File for global configuration and constants
 */

// Number of fighters available for selection
constexpr static const size_t NUMBER_OF_FIGHTERS = 18;

// The crowd of fighters
const char* const fighters[NUMBER_OF_FIGHTERS] = { "abomination",
												   "antiquarian",
								 				   "bounty_hunter",
												   "crusader",
												   "arbalest",
												   "flagellant",
												   "grave_robber",
												   "hellion",
												   "highwayman",
												   "houndmaster",
												   "jester",
												   "leper",
												   "man_at_arms",
												   "musketeer",
												   "occultist",
												   "plague_doctor",
												   "shieldbreaker",
												   "vestal" };
