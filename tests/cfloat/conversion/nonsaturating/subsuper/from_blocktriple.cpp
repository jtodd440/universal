// from_blocktriple.cpp: test suite runner for conversion tests between blocktriple and cfloats
//
// Copyright (C) 2017-2021 Stillwater Supercomputing, Inc.
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.
#include <universal/utility/directives.hpp>

// minimum set of include files to reflect source code dependencies
// Configure the cfloat template environment
// first: enable general or specialized configurations
#define CFLOAT_FAST_SPECIALIZATION
// second: enable/disable arithmetic exceptions
#define CFLOAT_THROW_ARITHMETIC_EXCEPTION 0
// third: enable trace conversion
#define TRACE_CONVERSION 0

#include <universal/number/cfloat/cfloat_impl.hpp>
#include <universal/number/cfloat/manipulators.hpp>
#include <universal/number/cfloat/mathlib.hpp>
#include <universal/verification/test_suite_conversion.hpp>
#include <universal/verification/cfloat_test_suite.hpp>
#include <universal/number/cfloat/table.hpp>

/*
How do you test the conversion state space of blocktriple to cfloat.
We need to convert the blocktriple that comes out of an ADD, a MUL, and a DIV operation.
The blocktriples have bits that need to be rounded by convert.
How do you test that rounding?

Convert the blocktriple to a value.
Use the cfloat operator=() to round. That is your reference. This assumes that cfloat operator=() has been validated.
Use convert() to convert to a cfloat.
Compare the operator=() and convert() cfloat patterns to check correctness
 */

// conditional compile flags
#define MANUAL_TESTING 1
#define STRESS_TESTING 0

int main()
try {
	using namespace sw::universal;

	// testing cfloat with subnormals and supernormals, but no saturation
	constexpr bool hasSubnormals = true;
	constexpr bool hasSupernormals = true;
	constexpr bool isSaturating = false;

	bool bReportIndividualTestCases = false;
	int nrOfFailedTestCases = 0;
	std::string tag = "conversion ";

#if MANUAL_TESTING

	std::cout << "Conversion from blocktriple to cfloat\n\n";

	// to track conversion in more detail
	std::cout << std::setprecision(8);
	std::cerr << std::setprecision(8);

	{
		// how do you round a non-normalized blocktriple?
		// you would need to modify the lsb/guard/round/sticky bit masks
		// so that you use all info to make the rounding decision,
		// then normalize (basically shift to the right) and apply
		// the rounding decision.
		{
			using Cfloat = cfloat<4, 2, uint8_t, hasSubnormals, hasSupernormals, isSaturating>;
			constexpr size_t fbits = Cfloat::fbits;
			typedef Cfloat::BlockType bt;
			blocktriple<fbits, BlockTripleOperator::ADD, bt> b;
			// 0b001.1  == 0.75, scale = -1
			b.setbits(0x03);
			b.setscale(-1);
			float v = float(b);
			Cfloat nut, ref;
			convert(b, nut);
			ref = v;
			std::cout << "blocktriple: " << to_binary(b) << " : " << float(b) << '\n';
			std::cout << "cfloat     : " << to_binary(nut) << " : " << nut << '\n';
			std::cout << "cfloat ref : " << to_binary(ref) << " : " << ref << '\n';
		}

		{
			// checking the other side of the exponential adjustments with cfloats
			// that expand on the dynamic range of IEEE-754
			using Cfloat = cfloat<80, 15, uint8_t, hasSubnormals, hasSupernormals, isSaturating>;
			Cfloat a;
			a = -1.0f;
			std::cout << type_tag(a) << '\n' << to_binary(a) << " : " << a << '\n';
			//			a.constexprClassParameters();
		}

		{
			using Cfloat = cfloat<5, 2, uint8_t, hasSubnormals, hasSupernormals, isSaturating>;
			constexpr size_t fbits = Cfloat::fbits;
			typedef Cfloat::BlockType bt;
			blocktriple<fbits, BlockTripleOperator::MUL, bt> b; // blocktriple type that comes out of a multiplication operation
			// 0b01.1110  == 1.875
			b.setbits(0x1e);
			float v = float(b);
			Cfloat nut, ref;
			convert(b, nut);
			ref = v;
			std::cout << "blocktriple: " << to_binary(b) << " : " << float(b) << '\n';
			std::cout << "cfloat     : " << to_binary(nut) << " : " << nut << '\n';
			std::cout << "cfloat ref : " << to_binary(ref) << " : " << ref << '\n';
		}
	}

	nrOfFailedTestCases += ReportTestResult(VerifyCfloatFromBlocktripleConversion<cfloat<4, 1, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD>(bReportIndividualTestCases), tag, "cfloat<4,1,uint8_t,0,0,0> from blocktriple ADD");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatFromBlocktripleConversion<cfloat<4, 2, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD>(true), tag, "cfloat<4,2,uint8_t,0,0,0> from blocktriple ADD");

#if STRESS_TESTING

	// manual exhaustive test
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatFromBlocktripleConversion<cfloat<8, 1, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD>(bReportIndividualTestCases), tag, "cfloat<8,1,uint8_t,0,0,0> from blocktriple ADD");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatFromBlocktripleConversion<cfloat<8, 2, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD>(bReportIndividualTestCases), tag, "cfloat<8,2,uint8_t,0,0,0> from blocktriple ADD");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatFromBlocktripleConversion<cfloat<8, 3, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD>(bReportIndividualTestCases), tag, "cfloat<8,3,uint8_t,0,0,0> from blocktriple ADD");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatFromBlocktripleConversion<cfloat<8, 4, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD>(bReportIndividualTestCases), tag, "cfloat<8,4,uint8_t,0,0,0> from blocktriple ADD");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatFromBlocktripleConversion<cfloat<8, 5, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD>(bReportIndividualTestCases), tag, "cfloat<8,5,uint8_t,0,0,0> from blocktriple ADD");
	nrOfFailedTestCases += ReportTestResult(VerifyCfloatFromBlocktripleConversion<cfloat<8, 6, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD>(bReportIndividualTestCases), tag, "cfloat<8,6,uint8_t,0,0,0> from blocktriple ADD");
#endif

	std::cout << "failed tests: " << nrOfFailedTestCases << '\n';
	nrOfFailedTestCases = 0; // in manual testing we ignore failures for the regression system

#else  // !MANUAL_TESTING

	std::cout << "cfloat from blocktriple conversion validation" << '\n';

	// es = 1
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat< 3, 1, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat< 3,1>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat< 4, 1, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat< 4,1>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat< 5, 1, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat< 5,1>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat< 6, 1, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat< 6,1>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat< 7, 1, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat< 7,1>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat< 8, 1, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat< 8,1>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat< 9, 1, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat< 9,1>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat<10, 1, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat<10,1>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat<12, 1, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat<12,1>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat<16, 1, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat<16,1>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat<18, 1, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat<18,1>");   // 3 blocks


	// es = 2
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat< 4, 2, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat< 4,2>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat< 5, 2, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat< 5,2>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat< 6, 2, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat< 6,2>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat< 7, 2, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat< 7,2>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat< 8, 2, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat< 8,2>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat<10, 2, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat<10,2>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat<12, 2, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat<12,2>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat<14, 2, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat<14,2>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat<16, 2, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat<16,2>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat<18, 2, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat<18,2>");   // 3 blocks


	// es = 3
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat< 5, 3, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat< 5,3>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat< 6, 3, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat< 6,3>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat< 7, 3, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat< 7,3>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat< 8, 3, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat< 8,3>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat<10, 3, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat<10,3>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat<12, 3, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat<12,3>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat<14, 3, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat<14,3>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat<18, 3, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat<18,3>");   // 3 blocks


	// es = 4
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat< 6, 4, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat< 6,4>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat< 7, 4, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat< 7,4>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat< 8, 4, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat< 8,4>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat<10, 4, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat<10,4>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat<12, 4, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat<12,4>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat<14, 4, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat<14,4>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat<18, 4, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat<18,4>");   // 3 blocks


	// es = 5
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat< 7, 5, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat< 7,5>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat< 8, 5, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat< 8,5>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat<10, 5, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat<10,5>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat<12, 5, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat<12,5>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat<14, 5, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat<14,5>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat<18, 5, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat<18,5>");   // 3 blocks


	// es = 6
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat< 8, 6, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat< 8,6>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat< 9, 6, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat< 9,6>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat<10, 6, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat<10,6>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat<12, 6, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat<12,6>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat<14, 6, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat<14,6>");


	// es = 7
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat< 9, 7, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat< 9,7>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat<10, 7, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat<10,7>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat<12, 7, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat<12,7>");
	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat<14, 7, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat<14,7>");

	// still failing
	// es = 8
//	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat<11, 8, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat<11,8>");
//	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat<12, 8, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat<12,8>");
//	nrOfFailedTestCases = ReportTestResult(VerifyCfloatFromBlocktripleConversion< cfloat<14, 8, uint8_t, hasSubnormals, hasSupernormals, isSaturating>, BlockTripleOperator::ADD >(bReportIndividualTestCases), tag, "cfloat<14,8>");


#if STRESS_TESTING


#endif  // STRESS_TESTING

#endif  // MANUAL_TESTING

	return (nrOfFailedTestCases > 0 ? EXIT_FAILURE : EXIT_SUCCESS);
}
catch (char const* msg) {
	std::cerr << "Caught exception: " << msg << std::endl;
	return EXIT_FAILURE;
}
catch (const sw::universal::cfloat_arithmetic_exception& err) {
	std::cerr << "Uncaught cfloat arithmetic exception: " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (const sw::universal::cfloat_internal_exception& err) {
	std::cerr << "Uncaught cfloat internal exception: " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (const std::runtime_error& err) {
	std::cerr << "Uncaught runtime exception: " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (...) {
	std::cerr << "Caught unknown exception" << std::endl;
	return EXIT_FAILURE;
}
