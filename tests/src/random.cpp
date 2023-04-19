#include <criterion/criterion.h>

#include <array>
#include <iostream>

#include "../../include/random.h"

using prec_t = long double;

constexpr static const bool VERBOSE = false;

bool approximately_equal(const prec_t a, const prec_t b, const prec_t epsilon = 1e-3l) {
	return std::abs(a - b) < epsilon;
}

Test(Random, Unit1, .disabled = false) {
	constexpr static const size_t NUM = 25lu;
	constexpr static const size_t LIM = NUM * NUM * 1024lu;
	constexpr static const prec_t PRC = 1.0l / NUM;
	std::array<prec_t, NUM> array;
	array.fill(0.0l);
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
		const bool equal = approximately_equal(element, PRC);
		if (VERBOSE)
			std::cout << index++ << '\t' << element << '\t' << (equal ? "PASS" : "FAIL") << std::endl;
		cr_expect(equal, "Element %lu out of %lu is not approximately equal to %Lf", index, NUM, PRC);
	}
}

Test(Random, Unit2, .disabled = false) {
	constexpr static const size_t NUM = 25lu;
	constexpr static const size_t LIM = NUM * NUM * 1024lu;
	constexpr static const prec_t PRC = 1.0l / NUM;
	std::array<prec_t, NUM> array;
	array.fill(0.0l);
	auto prng = Random::Tricky(0lu, NUM - 1lu);
	for (auto i = 0lu; i < LIM; ++i) {
		const auto index = prng();
		cr_assert(index < NUM, "Index out of bounds: %lu out of %lu", index, NUM);
		++array[index];
	}
	for (auto& element : array) {
		element /= LIM;
	}
	for (const auto element : array) {
		static size_t index = 0lu;
		const bool equal = approximately_equal(element, PRC);
		if (VERBOSE)
			std::cout << index << '\t' << element << '\t' << (equal ? "PASS" : "FAIL") << std::endl;
		++index;
		cr_expect(equal, "Element %lu out of %lu is not approximately equal to %Lf", index, NUM, PRC);
	}
}

Test(Random, Unit3) {
	constexpr static const size_t NUM = 10lu;
	constexpr static const size_t LIM = 1024lu;
	auto prng = Random::Tricky(0lu, 9lu);
	size_t last0 = prng(), last1 = prng();
	if (VERBOSE)
		std::cout << last0 << ' ' << last1 << ' ';
	for (auto i = 0lu; i < LIM; ++i) {
		size_t current = prng();
		if (VERBOSE)
			std::cout << current << ' ';
		cr_assert(current != last1, "Current number is equal to the last one");
		cr_assert(current != last0, "Current number is equal to the one before the last one");
		last0 = last1;
		last1 = current;
	}
	if (VERBOSE)
		std::cout << std::endl;
}
