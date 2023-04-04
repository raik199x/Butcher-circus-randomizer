#include <criterion/criterion.h>

#include <array>
#include <iostream>

#include "../../include/random.h"

using prec_t = long double;

bool approximately_equal(const prec_t a, const prec_t b, const prec_t epsilon = 1e-3l) {
	return std::abs(a - b) < epsilon;
}

Test(Random, Unit1) {
	constexpr static const size_t NUM = 100lu;
	constexpr static const size_t LIM = NUM * NUM * 1024lu;
	constexpr static const prec_t PRC = 1.0l / NUM;
	std::array<prec_t, NUM> array;
	array.fill(0lu);
	for (auto i = 0lu; i < LIM; ++i) {
		const auto index = Random::Uniform::integral<size_t>(0lu, NUM - 1lu);
		cr_assert(index < NUM, "Index out of bounds: %lu out of %lu", index, NUM);
		++array[index];
	}
	for (auto& element : array) {
		element /= LIM;
	}
	for (const auto element : array) {
		static size_t index = 0lu;
		std::cout << index++ << '\t' << element << std::endl;
		cr_expect(approximately_equal(element, PRC), "Element %lu out of %lu is not approximately equal to %Lf", index, NUM, PRC);
	}
}
