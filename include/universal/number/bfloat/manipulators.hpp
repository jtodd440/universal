#pragma once
// manipulators.hpp: definitions of helper functions for bfloat type manipulation
//
// Copyright (C) 2017-2021 Stillwater Supercomputing, Inc.
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.
#include <iostream>
#include <iomanip>
#include <typeinfo>  // for typeid()

// pull in the color printing for shells utility
#include <universal/utility/color_print.hpp>

// This file contains functions that manipulate a posit type
// using posit number system knowledge.

namespace sw::universal {

// report dynamic range of a type, specialized for a posit
template<size_t nbits, size_t es, typename bt>
std::string dynamic_range(bfloat<nbits, es, bt>& b) {
	std::stringstream ss;
	ss << type_tag(b) << ": ";
	ss << "minpos scale " << std::setw(10) << minpos(b).scale() << "     ";
	ss << "maxpos scale " << std::setw(10) << maxpos(b).scale();
	return ss.str();
}

// Generate a type tag for this posit, for example, posit<8,1>
template<size_t nbits, size_t es, typename bt>
std::string type_tag(const bfloat<nbits, es, bt>& v) {
	std::stringstream ss;
	ss << "bfloat<" << nbits << "," << es << ">";
	return ss.str();
}

// Generate a string representing the bfloat components: sign, exponent, faction, uncertainty bit, and value
template<size_t nbits, size_t es, typename bt>
std::string components(const bfloat<nbits, es, bt>& v) {
	std::stringstream ss;
	bool s{ false };
	blockbinary<v.es, bt> e;
	blockbinary<v.fbits, bt> f;
	decode(v, s, e, f);

	// TODO: hardcoded field width is governed by pretty printing bfloat tables, which by construction will always be small bfloats
	ss << std::setw(14) << to_binary(v) 
		<< " Sign : " << std::setw(2) << s
		<< " Exponent : " << std::setw(5) << e
		<< " Fraction : " << std::setw(8) << f
		<< " Value : " << std::setw(16) << v;

	return ss.str();
}

// generate a binary string for bfloat
template<size_t nbits, size_t es, typename bt>
inline std::string to_hex(const bfloat<nbits, es, bt>& v) {
	constexpr size_t bitsInByte = 8;
	constexpr size_t bitsInBlock = sizeof(bt) * bitsInByte;
	char hexChar[16] = {
		'0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
	};
	std::stringstream ss;
	ss << "0x" << std::hex;
	long nrNibbles = long(1ull + ((nbits - 1ull) >> 2ull));
	for (long n = nrNibbles - 1; n >= 0; --n) {
		uint8_t nibble = v.nibble(size_t(n));
		ss << hexChar[nibble];
		if (n > 0 && ((n * 4ll) % bitsInBlock) == 0) ss << '\'';
	}
	return ss.str();
}

// generate a bfloat format ASCII format nbits.esxNN...NNa
template<size_t nbits, size_t es, typename bt>
inline std::string hex_print(const bfloat<nbits, es, bt>& r) {
	std::stringstream ss;
	ss << nbits << '.' << es << 'x' << to_hex(r) << 'r';
	return ss.str();
}

template<size_t nbits, size_t es, typename bt>
std::string pretty_print(const bfloat<nbits, es, bt>& r) {
	std::stringstream ss;
	constexpr size_t fbits = bfloat<nbits, es, bt>::fbits;
	bool s{ false };
	blockbinary<es, bt> e;
	blockbinary<fbits, bt> f;
	decode(r, s, e, f);

	// sign bit
	ss << (r.isneg() ? '1' : '0');

	// exponent bits
	ss << '-';
	for (int i = int(es) - 1; i >= 0; --i) {
		ss << (e.test(static_cast<size_t>(i)) ? '1' : '0');
	}

	// fraction bits
	ss << '-';
	for (int i = int(r.fbits) - 1; i >= 0; --i) {
		ss << (f.test(static_cast<size_t>(i)) ? '1' : '0');
	}

	return ss.str();
}

template<size_t nbits, size_t es, typename bt>
std::string info_print(const bfloat<nbits, es, bt>& p, int printPrecision = 17) {
	return "TBD";
}

template<size_t nbits, size_t es, typename bt>
std::string color_print(const bfloat<nbits, es, bt>& r) {
	using Real = bfloat<nbits, es, bt>;
	std::stringstream ss;
	bool s{ false };
	blockbinary<es,bt> e;
	blockbinary<Real::fbits,bt> f;
	decode(r, s, e, f);

	Color red(ColorCode::FG_RED);
	Color yellow(ColorCode::FG_YELLOW);
	Color blue(ColorCode::FG_BLUE);
	Color magenta(ColorCode::FG_MAGENTA);
	Color cyan(ColorCode::FG_CYAN);
	Color white(ColorCode::FG_WHITE);
	Color def(ColorCode::FG_DEFAULT);

	// sign bit
	ss << red << (r.isneg() ? '1' : '0');

	// exponent bits
	for (int i = int(es) - 1; i >= 0; --i) {
		ss << cyan << (e.test(static_cast<size_t>(i)) ? '1' : '0');
	}

	// fraction bits
	for (int i = int(r.fbits) - 1; i >= 0; --i) {
		ss << magenta << (f.test(static_cast<size_t>(i)) ? '1' : '0');
	}

	ss << def;
	return ss.str();
}

}  // namespace sw::universal

