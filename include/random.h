#pragma once

#include <random>

/*!
 * \file random.h
 * \author Timuri Alvarez
 * \brief File for generating random numbers
 */

namespace Random::Uniform
{
	/*!
	 * \brief Generates a random integer in the range (min, max)
	 * 
	 * \tparam T type of the number
	 * \param min minimum value of the range (exclusive)
	 * \param max maximum value of the range (exclusive)
	 * \return T random number in the range (min, max)
	 */
	template<typename T>
	inline T real(const T min, const T max) {
		std::random_device rdev;
		return std::uniform_real_distribution<T>(std::min(min, max), std::max(min, max))(rdev);
	}

	/*!
	 * \brief Generates a random real number in the range [min, max]
	 * 
	 * \tparam T type of the number
	 * \param min minimum value of the range (inclusive)
	 * \param max maximum value of the range (inclusive)
	 * \return T random number in the range [min, max]
	 */
	template<typename T>
	inline T integral(const T min, const T max) {
		std::random_device rdev;
		return std::uniform_int_distribution<T>(std::min(min, max), std::max(min, max))(rdev);
	}
}
