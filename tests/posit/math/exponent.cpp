// function_exponent.cpp: test suite runner for exponent (exp, exp2, exp10) functions
//
// Copyright (C) 2017-2021 Stillwater Supercomputing, Inc.
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.

// when you define POSIT_VERBOSE_OUTPUT the code will print intermediate results for selected arithmetic operations
//#define POSIT_VERBOSE_OUTPUT
#define POSIT_TRACE_POW

// minimum set of include files to reflect source code dependencies
#include <universal/number/posit/posit.hpp>
#include <universal/number/posit/manipulators.hpp>
#include <universal/number/posit/math/exponent.hpp>
#include <universal/number/quire/exceptions.hpp>  // math library might use quire
#include <universal/verification/posit_math_test_suite.hpp>

// Background: http://numbers.computation.free.fr/Constants/E/e.html
//
// generate digits of Euler's number
void GenerateEulersNumber() {
	int N = 9009, a[9009], x = 0;
	for (int n = N - 1; n > 0; --n) {
		a[n] = 1;
	}
	a[1] = 2;
	while (N > 9) {
		int n = N--;
		while (--n) {
			a[n] = x % n;
			x = 10 * a[n - 1] + x / n;
		}
		std::cout << x;
	}
	std::cout << std::endl;
}

// generate specific test case that you can trace with the trace conditions in posit.h
// for most bugs they are traceable with _trace_conversion and _trace_add
template<size_t nbits, size_t es, typename Ty>
void GenerateTestCase(Ty a) {
	Ty ref;
	sw::universal::posit<nbits, es> pa, pref, pexp;
	pa = a;
	ref = std::exp(a);
	pref = ref;
	pexp = sw::universal::exp(pa);
	std::cout << std::setprecision(nbits - 2);
	std::cout << std::setw(nbits) << a << " -> exp(" << a << ") = " << std::setw(nbits) << ref << std::endl;
	std::cout << pa.get() << " -> exp( " << pa << ") = " << pexp.get() << " (reference: " << pref.get() << ")   ";
	std::cout << (pref == pexp ? "PASS" : "FAIL") << std::endl << std::endl;
	std::cout << std::setprecision(5);
}

#define MANUAL_TESTING 0
#define STRESS_TESTING 0


int main(int argc, char** argv)
try {
	using namespace std;
	using namespace sw::universal;

	GenerateEulersNumber();

	return 0;
	bool bReportIndividualTestCases = true;
	int nrOfFailedTestCases = 0;

	std::string tag = "Addition failed: ";

#if MANUAL_TESTING
	// generate individual testcases to hand trace/debug
	GenerateTestCase<16, 1, float>(4.0f);

#if GENERATE_EXPONENT_TABLES
	GenerateExponentTable<3, 0>();
	GenerateExponentTable<4, 0>();
	GenerateExponentTable<4, 1>();
	GenerateExponentTable<5, 0>();
	GenerateExponentTable<5, 1>();
	GenerateExponentTable<5, 2>();
	GenerateExponentTable<6, 0>();
	GenerateExponentTable<6, 1>();
	GenerateExponentTable<6, 2>();
	GenerateExponentTable<6, 3>();
	GenerateExponentTable<7, 0>();
#endif

	cout << endl;

	// manual exhaustive test
	nrOfFailedTestCases += ReportTestResult(VerifyExp<2, 0>("Manual Testing", bReportIndividualTestCases), "posit<2,0>", "exp");

	nrOfFailedTestCases += ReportTestResult(VerifyExp<3, 0>("Manual Testing", bReportIndividualTestCases), "posit<3,0>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<3, 1>("Manual Testing", bReportIndividualTestCases), "posit<3,1>", "exp");

	nrOfFailedTestCases += ReportTestResult(VerifyExp<4, 0>("Manual Testing", bReportIndividualTestCases), "posit<4,0>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<4, 1>("Manual Testing", bReportIndividualTestCases), "posit<4,1>", "exp");

	nrOfFailedTestCases += ReportTestResult(VerifyExp<5, 0>("Manual Testing", bReportIndividualTestCases), "posit<5,0>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<5, 1>("Manual Testing", bReportIndividualTestCases), "posit<5,1>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<5, 2>("Manual Testing", bReportIndividualTestCases), "posit<5,2>", "exp");

	nrOfFailedTestCases += ReportTestResult(VerifyExp<8, 4>("Manual Testing", bReportIndividualTestCases), "posit<8,4>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp2<8, 4>("Manual Testing", bReportIndividualTestCases), "posit<8,4>", "exp2");

#else

	cout << "Posit exponential function validation" << endl;

	nrOfFailedTestCases += ReportTestResult(VerifyExp<2, 0>(bReportIndividualTestCases), "posit<2,0>", "exp");

	nrOfFailedTestCases += ReportTestResult(VerifyExp<3, 0>(bReportIndividualTestCases), "posit<3,0>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<3, 1>(bReportIndividualTestCases), "posit<3,1>", "exp");

	nrOfFailedTestCases += ReportTestResult(VerifyExp<4, 0>(bReportIndividualTestCases), "posit<4,0>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<4, 1>(bReportIndividualTestCases), "posit<4,1>", "exp");

	nrOfFailedTestCases += ReportTestResult(VerifyExp<5, 0>(bReportIndividualTestCases), "posit<5,0>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<5, 1>(bReportIndividualTestCases), "posit<5,1>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<5, 2>(bReportIndividualTestCases), "posit<5,2>", "exp");

	nrOfFailedTestCases += ReportTestResult(VerifyExp<6, 0>(bReportIndividualTestCases), "posit<6,0>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<6, 1>(bReportIndividualTestCases), "posit<6,1>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<6, 2>(bReportIndividualTestCases), "posit<6,2>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<6, 3>(bReportIndividualTestCases), "posit<6,3>", "exp");

	nrOfFailedTestCases += ReportTestResult(VerifyExp<7, 0>(bReportIndividualTestCases), "posit<7,0>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<7, 1>(bReportIndividualTestCases), "posit<7,1>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<7, 2>(bReportIndividualTestCases), "posit<7,2>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<7, 3>(bReportIndividualTestCases), "posit<7,3>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<7, 4>(bReportIndividualTestCases), "posit<7,4>", "exp");

	nrOfFailedTestCases += ReportTestResult(VerifyExp<8, 0>(bReportIndividualTestCases), "posit<8,0>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<8, 1>(bReportIndividualTestCases), "posit<8,1>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<8, 2>(bReportIndividualTestCases), "posit<8,2>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<8, 3>(bReportIndividualTestCases), "posit<8,3>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<8, 4>(bReportIndividualTestCases), "posit<8,4>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<8, 5>(bReportIndividualTestCases), "posit<8,5>", "exp");

	nrOfFailedTestCases += ReportTestResult(VerifyExp<9, 0>(bReportIndividualTestCases), "posit<9,0>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<9, 1>(bReportIndividualTestCases), "posit<9,1>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<9, 2>(bReportIndividualTestCases), "posit<9,2>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<9, 3>(bReportIndividualTestCases), "posit<9,3>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<9, 4>(bReportIndividualTestCases), "posit<9,4>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<9, 5>(bReportIndividualTestCases), "posit<9,5>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<9, 6>(bReportIndividualTestCases), "posit<9,6>", "exp");
	
	nrOfFailedTestCases += ReportTestResult(VerifyExp<10, 0>(bReportIndividualTestCases), "posit<10,0>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<10, 1>(bReportIndividualTestCases), "posit<10,1>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<10, 2>(bReportIndividualTestCases), "posit<10,2>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<10, 7>(bReportIndividualTestCases), "posit<10,7>", "exp");

	nrOfFailedTestCases += ReportTestResult(VerifyExp<12, 0>(bReportIndividualTestCases), "posit<12,0>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<12, 1>(bReportIndividualTestCases), "posit<12,1>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<12, 2>(bReportIndividualTestCases), "posit<12,2>", "exp");

	nrOfFailedTestCases += ReportTestResult(VerifyExp<16, 0>(bReportIndividualTestCases), "posit<16,0>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<16, 1>(bReportIndividualTestCases), "posit<16,1>", "exp");
	nrOfFailedTestCases += ReportTestResult(VerifyExp<16, 2>(bReportIndividualTestCases), "posit<16,2>", "exp");

	// base-2 exponent testing
	nrOfFailedTestCases += ReportTestResult(VerifyExp2<2, 0>(bReportIndividualTestCases), "posit<2,0>", "exp2");

	nrOfFailedTestCases += ReportTestResult(VerifyExp2<3, 0>(bReportIndividualTestCases), "posit<3,0>", "exp2");
	nrOfFailedTestCases += ReportTestResult(VerifyExp2<3, 1>(bReportIndividualTestCases), "posit<3,1>", "exp2");

	nrOfFailedTestCases += ReportTestResult(VerifyExp2<4, 0>(bReportIndividualTestCases), "posit<4,0>", "exp2");
	nrOfFailedTestCases += ReportTestResult(VerifyExp2<4, 1>(bReportIndividualTestCases), "posit<4,1>", "exp2");

	nrOfFailedTestCases += ReportTestResult(VerifyExp2<5, 0>(bReportIndividualTestCases), "posit<5,0>", "exp2");
	nrOfFailedTestCases += ReportTestResult(VerifyExp2<5, 1>(bReportIndividualTestCases), "posit<5,1>", "exp2");
	nrOfFailedTestCases += ReportTestResult(VerifyExp2<5, 2>(bReportIndividualTestCases), "posit<5,2>", "exp2");

	nrOfFailedTestCases += ReportTestResult(VerifyExp2<6, 0>(bReportIndividualTestCases), "posit<6,0>", "exp2");
	nrOfFailedTestCases += ReportTestResult(VerifyExp2<6, 1>(bReportIndividualTestCases), "posit<6,1>", "exp2");
	nrOfFailedTestCases += ReportTestResult(VerifyExp2<6, 2>(bReportIndividualTestCases), "posit<6,2>", "exp2");
	nrOfFailedTestCases += ReportTestResult(VerifyExp2<6, 3>(bReportIndividualTestCases), "posit<6,3>", "exp2");

	nrOfFailedTestCases += ReportTestResult(VerifyExp2<7, 0>(bReportIndividualTestCases), "posit<7,0>", "exp2");
	nrOfFailedTestCases += ReportTestResult(VerifyExp2<7, 1>(bReportIndividualTestCases), "posit<7,1>", "exp2");
	nrOfFailedTestCases += ReportTestResult(VerifyExp2<7, 2>(bReportIndividualTestCases), "posit<7,2>", "exp2");
	nrOfFailedTestCases += ReportTestResult(VerifyExp2<7, 3>(bReportIndividualTestCases), "posit<7,3>", "exp2");
	nrOfFailedTestCases += ReportTestResult(VerifyExp2<7, 4>(bReportIndividualTestCases), "posit<7,4>", "exp2");

	nrOfFailedTestCases += ReportTestResult(VerifyExp2<8, 0>(bReportIndividualTestCases), "posit<8,0>", "exp2");
	nrOfFailedTestCases += ReportTestResult(VerifyExp2<8, 1>(bReportIndividualTestCases), "posit<8,1>", "exp2");
	nrOfFailedTestCases += ReportTestResult(VerifyExp2<8, 2>(bReportIndividualTestCases), "posit<8,2>", "exp2");
	nrOfFailedTestCases += ReportTestResult(VerifyExp2<8, 3>(bReportIndividualTestCases), "posit<8,3>", "exp2");
	nrOfFailedTestCases += ReportTestResult(VerifyExp2<8, 4>(bReportIndividualTestCases), "posit<8,4>", "exp2");
	nrOfFailedTestCases += ReportTestResult(VerifyExp2<8, 5>(bReportIndividualTestCases), "posit<8,5>", "exp2");

	nrOfFailedTestCases += ReportTestResult(VerifyExp2<9, 0>(bReportIndividualTestCases), "posit<9,0>", "exp2");
	nrOfFailedTestCases += ReportTestResult(VerifyExp2<9, 1>(bReportIndividualTestCases), "posit<9,1>", "exp2");
	nrOfFailedTestCases += ReportTestResult(VerifyExp2<9, 2>(bReportIndividualTestCases), "posit<9,2>", "exp2");
	nrOfFailedTestCases += ReportTestResult(VerifyExp2<9, 3>(bReportIndividualTestCases), "posit<9,3>", "exp2");
	nrOfFailedTestCases += ReportTestResult(VerifyExp2<9, 4>(bReportIndividualTestCases), "posit<9,4>", "exp2");
	nrOfFailedTestCases += ReportTestResult(VerifyExp2<9, 5>(bReportIndividualTestCases), "posit<9,5>", "exp2");
	nrOfFailedTestCases += ReportTestResult(VerifyExp2<9, 6>(bReportIndividualTestCases), "posit<9,6>", "exp2");

	nrOfFailedTestCases += ReportTestResult(VerifyExp2<10, 0>(bReportIndividualTestCases), "posit<10,0>", "exp2");
	nrOfFailedTestCases += ReportTestResult(VerifyExp2<10, 1>(bReportIndividualTestCases), "posit<10,1>", "exp2");
	nrOfFailedTestCases += ReportTestResult(VerifyExp2<10, 2>(bReportIndividualTestCases), "posit<10,2>", "exp2");
	nrOfFailedTestCases += ReportTestResult(VerifyExp2<10, 7>(bReportIndividualTestCases), "posit<10,7>", "exp2");

	nrOfFailedTestCases += ReportTestResult(VerifyExp2<12, 0>(bReportIndividualTestCases), "posit<12,0>", "exp2");
	nrOfFailedTestCases += ReportTestResult(VerifyExp2<12, 1>(bReportIndividualTestCases), "posit<12,1>", "exp2");
	nrOfFailedTestCases += ReportTestResult(VerifyExp2<12, 2>(bReportIndividualTestCases), "posit<12,2>", "exp2");

	nrOfFailedTestCases += ReportTestResult(VerifyExp2<16, 0>(bReportIndividualTestCases), "posit<16,0>", "exp2");
	nrOfFailedTestCases += ReportTestResult(VerifyExp2<16, 1>(bReportIndividualTestCases), "posit<16,1>", "exp2");
	nrOfFailedTestCases += ReportTestResult(VerifyExp2<16, 2>(bReportIndividualTestCases), "posit<16,2>", "exp2");


#if STRESS_TESTING
	
#endif  // STRESS_TESTING

#endif  // MANUAL_TESTING

	return (nrOfFailedTestCases > 0 ? EXIT_FAILURE : EXIT_SUCCESS);
}
catch (char const* msg) {
	std::cerr << msg << std::endl;
	return EXIT_FAILURE;
}
catch (const sw::universal::posit_arithmetic_exception& err) {
	std::cerr << "Uncaught posit arithmetic exception: " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (const sw::universal::quire_exception& err) {
	std::cerr << "Uncaught quire exception: " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (const sw::universal::posit_internal_exception& err) {
	std::cerr << "Uncaught posit internal exception: " << err.what() << std::endl;
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
