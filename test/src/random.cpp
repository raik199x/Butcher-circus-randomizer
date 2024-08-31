#include <gtest/gtest.h>

#include <array>

#include "random.h"

using prec_t = long double;

bool approximatelyEqual(const prec_t val_a, const prec_t val_b, const prec_t epsilon = 1e-3L) {
  return std::abs(val_a - val_b) < epsilon;
}

TEST(Random, Unit1) {
  constexpr static size_t  kNum = 25LU;
  constexpr static size_t  kLim = kNum * kNum * 1024LU;
  constexpr static prec_t  kPrc = 1.0L / kNum;
  std::array<prec_t, kNum> array;
  array.fill(0.0L);
  for (auto i = 0LU; i < kLim; ++i) {
    const auto index = Random::Uniform::integral<size_t>(0LU, kNum - 1LU);
    EXPECT_LT(index, kNum) << "Index out of bounds: " << index << "out of" << kNum << "\n";
    ++array[index];
  }
  for (auto &element : array) {
    element /= kLim;
  }
  for (const auto element : array) {
    static size_t index = 0LU;
    const bool    equal = approximatelyEqual(element, kPrc);
    EXPECT_TRUE(equal) << "Element " << index << " out of " << kNum << " is not approximately equal to " << kPrc;
  }
}

TEST(Random, Unit2) {
  constexpr static size_t  kNum = 25LU;
  constexpr static size_t  kLim = kNum * kNum * 1024LU;
  constexpr static prec_t  kPrc = 1.0L / kNum;
  std::array<prec_t, kNum> array;
  array.fill(0.0L);
  auto prng = Random::Tricky(0LU, kNum - 1LU);
  for (auto i = 0LU; i < kLim; ++i) {
    const auto index = prng();
    EXPECT_LT(index, kNum) << "Index out of bounds: " << index << "out of" << kNum << "\n";
    ++array[index];
  }
  for (auto &element : array) {
    element /= kLim;
  }
  for (const auto element : array) {
    static size_t index = 0LU;
    const bool    equal = approximatelyEqual(element, kPrc);
    ++index;
    EXPECT_TRUE(equal) << "Element " << index << " out of " << kNum << " is not approximately equal to " << kPrc;
  }
}

TEST(Random, Unit3) {
  constexpr static size_t kNum    = 10LU;
  constexpr static size_t kLim    = 1024LU;
  constexpr unsigned long kMaxVal = 9LU;
  auto                    prng    = Random::Tricky(0LU, kMaxVal);
  size_t                  last0   = prng();
  size_t                  last1   = prng();
  for (auto i = 0LU; i < kLim; ++i) {
    size_t current = prng();
    EXPECT_NE(current, last1) << "Current number is equal to the last one";
    EXPECT_NE(current, last0) << "Current number is equal to the one before the last one";
    last0 = last1;
    last1 = current;
  }
}
