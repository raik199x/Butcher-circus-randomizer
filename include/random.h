#pragma once

#include <random>
#include <vector>

/*!
 * \file random.h
 * \author Timuri Alvarez
 * \brief File for generating random numbers
 */

namespace Random::Uniform {
/*!
 * \brief Generates a random integer in the range (min, max)
 *
 * \tparam T type of the number
 * \param min minimum value of the range (exclusive)
 * \param max maximum value of the range (exclusive)
 * \return T random number in the range (min, max)
 */
template <typename T>
inline T real(const T min, const T max) {
  static_assert(std::is_floating_point_v<T>, "Uniform::real only works with floating point types");
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
template <typename T>
inline T integral(const T min, const T max) {
  static_assert(std::is_integral_v<T>, "Uniform::integral only works with integral types");
  std::random_device rdev;
  return std::uniform_int_distribution<T>(std::min(min, max), std::max(min, max))(rdev);
}
} // namespace Random::Uniform

namespace Random {
template <typename T, const size_t BARRIER = 1LU>
class Tricky;
}

/*!
 * \brief Generates a random real number in the range [min, max]
 *
 * \tparam T type of the number
 * \tparam BARRIER number of times a number generation is forbidden after it was generated
 * \example Tricky<int, 2> prng(0, 10); // Generates a random number in the range [0, 10], forbids a number to be
 * generated 2 times after it was generated \example Tricky<int, 0> prng(0, 10); // Generates a random number in the
 * range [0, 10], forbids a number to be generated 0 times after it was generated (equivalent to Uniform::integral)
 * \param min minimum value of the range (inclusive)
 * \param max maximum value of the range (inclusive)
 * \return T random number in the range [min, max]
 */
template <typename T, const size_t BARRIER>
class Random::Tricky {
protected:
  //! \brief Protection from invalid template parameters
  static_assert(std::is_integral_v<T>, "Tricky PRNG only works with integral types");

public:
  //! \brief Tricky PRNG constructor (min and max are inclusive)
  Tricky(const T min, const T max) : min(std::min(min, max)), max(std::max(min, max)) {
    //! \note initialize chances vector
    chances.resize(max - min + 1LU);
    //! \note set all chances to BARRIER + 1, required for prng to work properly and not stuck in an infinite loop
    for (auto &chance : chances) {
      chance = BARRIER + 1LU;
    }
  }

  //! \brief Tricky PRNG operator, aka "generate a random number"
  T operator()() {
    //! \note calculate the sum of all chances
    const size_t sum = std::accumulate(chances.begin(), chances.end(), 0LU);
    while (true) {
      //! \note generate a random number in the range [0, sum - 1]
      auto random = Uniform::integral<size_t>(0LU, sum - 1LU);
      //! \note get the index of the number in chances vector that corresponds to the given random number
      size_t index = index_of(random);
      // If the number was chosen BARRIER times ago, then skip it
      if (chances[index] <= BARRIER) {
        continue;
      }
      // Increment all chances
      for (auto &chance : chances) {
        ++chance;
      }
      // Set to zero the chance of the number that was chosen
      chances[index] = 0LU;
      // Return the number
      return min + index;
    }
  }

private:
  //! \note min and max values for generation
  T min, max;
  //! \note chances vector for generation
  std::vector<size_t> chances;

  //! \brief Returns the index of the number in chances vector that corresponds to the given random number
  size_t index_of(size_t random) {
    size_t index = 0LU;
    //! \note iterate over chances to find the index of the random number
    for (const auto chance : chances) {
      //! \note number was found, return the index
      if (random < chance) {
        break;
      }
      //! \note decrement random by the chance of the current number
      random -= chance;
      //! \note increment index
      ++index;
    }
    //! \note return the index
    return index;
  }
};
