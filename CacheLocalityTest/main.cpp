#include <chrono>
#include <thread>
#include <vector>
#include <iostream>
#include <random>

const int ENTITIES = 100000000;

int main() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, ENTITIES - 1);

	std::vector<int> testVector;
	std::vector<int> sequentialVector;
	std::vector<int> randomVector;

	
	testVector.reserve(ENTITIES);
	sequentialVector.reserve(ENTITIES);
	randomVector.reserve(ENTITIES);

	for (int i = 0; i < ENTITIES; i++) {
		testVector.push_back(i);
		sequentialVector.push_back(i);
		// We fill a vector with random values which we will use to index the testVector,
		// this is my chosen method as I want to avoid the overhead of genereating random indexes in the loop,
		// and other access patterns (e.g. strides) can be optimized away by the compiler.
		randomVector.push_back(dis(gen));
	}


	// The test which gets the most fair timed comparison, 
	// although sequentialVector[i] will just retrieve i, this is necessary to ensure that the time difference
	// is due to memory access order and not the overhead of indexing the randomVector.
	
	long long sum1 = 0;
	auto sequentialTestStart = std::chrono::steady_clock::now();
	for (int i = 0; i < ENTITIES; i++) {
		sum1 += testVector[sequentialVector[i]];
	}
	auto sequentialTestEnd = std::chrono::steady_clock::now();
	
	std::this_thread::sleep_for(std::chrono::seconds(1));

	long long sum2 = 0;
	auto randomTestStart = std::chrono::steady_clock::now();
	for (int i = 0; i < ENTITIES; i++) {
		sum2 += testVector[randomVector[i]];
	}
	auto randomTestEnd = std::chrono::steady_clock::now();


	// This is the code I used for memory profiling as we are only indexing one vector rather than two per loop.
	// This isn't suitable for timing due to the overhead of generating a random index.
	/*
	long long sum1 = 0;
	auto sequentialTestStart = std::chrono::steady_clock::now();
	for (int i = 0; i < ENTITIES; i++) {
		sum1 += testVector[i];
	}
	auto sequentialTestEnd = std::chrono::steady_clock::now();
	
	std::this_thread::sleep_for(std::chrono::seconds(1));

	long long sum2 = 0;
	auto randomTestStart = std::chrono::steady_clock::now();
	for (int i = 0; i < ENTITIES; i++) {
		sum2 += testVector[dis(gen)];
	}
	auto randomTestEnd = std::chrono::steady_clock::now();
	*/



	// This is necessary because without utilised output the compiler optimizes the loops away entirely, 
	// this is due to dead code elimination.
	std::cout << "Sum 1: " << sum1 << "\n";
	std::cout << "Sum 2: " << sum2 << "\n\n";

	std::cout << "Sequential milliseconds: "
		<< std::chrono::duration_cast<std::chrono::milliseconds>
		(sequentialTestEnd - sequentialTestStart).count() << "ms\n";
	std::cout << "Random milliseconds: "
		<< std::chrono::duration_cast<std::chrono::milliseconds>
		(randomTestEnd - randomTestStart).count() << "ms\n";
}
