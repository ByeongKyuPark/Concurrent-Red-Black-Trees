#include "redBlackTree.h"
#include <iostream>
#include <iomanip>
#include <random>
#include <set>
#include <string>
#include <thread>

void GetRandomValues(int* array, int size, int seed) {
	std::mt19937 rand(seed);
	std::uniform_int_distribution<> dist(0, size * 3);
	std::set<int> set;
	for (int i = 0; i < size;) {
		int val = dist(rand);
		if (set.find(val) == set.end()) {
			set.insert(val);
			array[i++] = val;
		}
	}
}

void Insert_Basic() {
	std::cout << "==================== Insert ====================" << std::endl;
	RedBlackTree tree;

	int values[] = { 5,2,9,8,12,15,17,19,25,6,7 };
	constexpr int NumValues = sizeof(values) / sizeof(*values);
	for (int i{}; i < NumValues; ++i) {
		std::cout << "Inserting: " << values[i] << std::endl;
		tree.Add(values[i]);
		tree.PrintTree();
	}
}
void Insert_Random_Seed1() {
	std::cout << "==================== Insert Random(seed 1) ====================" << std::endl;
	RedBlackTree tree;

	constexpr int Size = 20;
	int values[Size];
	GetRandomValues(values, Size, 1);

	for (int i{}; i < Size; ++i) {
		std::cout << "Inserting: " << values[i] << std::endl;
		tree.Add(values[i]);
		tree.PrintTree();
	}
}
void Insert_Random_Seed2() {
	std::cout << "==================== Insert Random(seed 2) ====================" << std::endl;
	RedBlackTree tree;

	constexpr int Size = 20;
	int values[Size];
	GetRandomValues(values, Size, 2);

	for (int i{}; i < Size; ++i) {
		std::cout << "Inserting: " << values[i] << std::endl;
		tree.Add(values[i]);
		tree.PrintTree();
	}
}

void Insert_Random_Seed3() {
	std::cout << "==================== Insert Random(seed 3) ====================" << std::endl;
	RedBlackTree tree;

	constexpr int Size = 20;
	int values[Size];
	GetRandomValues(values, Size, 3);

	for (int i{}; i < Size; ++i) {
		std::cout << "Inserting: " << values[i] << std::endl;
		tree.Add(values[i]);
		tree.PrintTree();
	}
}

std::mutex print_mtx;

void MultiThreadedInsertRandom(RedBlackTree& tree, int numItrs, int numThreads, int threadIdx, const int* values) {
	for (int i{}; i < numItrs; ++i) {
		tree.Add(values[threadIdx + i * numThreads]);
	}
}
std::mutex mtx;
void MultiThreadedSearch(RedBlackTree& tree, int numItrs, int numThreads, int threadIdx, const int* values, std::vector<int>& missing) {
	for (int i{}; i < numItrs; ++i) {
		if (tree.Contains(values[threadIdx + i * numThreads]) == false) {
			std::unique_lock<std::mutex>lk(mtx);
			missing.push_back(values[threadIdx + i * numThreads]);
			lk.unlock();
		}
	}
}

void Search_Basic() {

	std::cout << "==================== Search ====================" << std::endl;
	RedBlackTree tree;

	int values[] = { 5,2,9,8,12,15,17,19,25,6,7 };
	constexpr int NumValues = sizeof(values) / sizeof(*values);
	for (int i{}; i < NumValues; ++i) {
		tree.Add(values[i]);
	}
	tree.PrintTree();

	std::cout << "begin searching\n";

	std::vector<int> missing;
	for (int i{}; i < NumValues; ++i) {
		if (tree.Contains(values[i]) == false) {
			missing.push_back(values[i]);
		}
	}
	if (missing.empty() == true) {
		std::cout << "found all\n";
	}
	else {
		std::cout << "missing: ";
		for (auto i : missing) {
			std::cout << i << " ";
		}
		std::cout << '\n';
	}
}

void Search_Random_Seed1() {

	std::cout << "==================== Search Random (seed 1) ====================" << std::endl;
	RedBlackTree tree;

	constexpr int Size = 20;
	int values[Size];
	GetRandomValues(values, Size, 1);

	for (int i{}; i < Size; ++i) {
		tree.Add(values[i]);
	}
	tree.PrintTree();

	std::cout << "begin searching\n";

	std::vector<int> missing;
	for (int i{}; i < Size; ++i) {
		if (tree.Contains(values[i]) == false) {
			missing.push_back(values[i]);
		}
	}
	if (missing.empty() == true) {
		std::cout << "-> found all\n";
	}
	else {
		std::cout << "missing: ";
		for (auto i : missing) {
			std::cout << i << " ";
		}
		std::cout << '\n';
	}
}
void Search_Random_Seed2() {

	std::cout << "==================== Search Random (seed 2) ====================" << std::endl;
	RedBlackTree tree;

	constexpr int Size = 20;
	int values[Size];
	GetRandomValues(values, Size, 2);

	for (int i{}; i < Size; ++i) {
		tree.Add(values[i]);
	}
	tree.PrintTree();

	std::cout << "begin searching\n";

	std::vector<int> missing;
	for (int i{}; i < Size; ++i) {
		if (tree.Contains(values[i]) == false) {
			missing.push_back(values[i]);
		}
	}
	if (missing.empty() == true) {
		std::cout << "-> found all\n";
	}
	else {
		std::cout << "missing: ";
		for (auto i : missing) {
			std::cout << i << " ";
		}
		std::cout << '\n';
	}
}

void Search_Random_Seed3() {

	std::cout << "==================== Search Random (seed 3) ====================" << std::endl;
	RedBlackTree tree;

	constexpr int Size = 20;
	int values[Size];
	GetRandomValues(values, Size, 3);

	for (int i{}; i < Size; ++i) {
		tree.Add(values[i]);
	}
	tree.PrintTree();

	std::cout << "begin searching\n";

	std::vector<int> missing;
	for (int i{}; i < Size; ++i) {
		if (tree.Contains(values[i]) == false) {
			missing.push_back(values[i]);
		}
	}
	if (missing.empty() == true) {
		std::cout << "-> found all\n";
	}
	else {
		std::cout << "missing: ";
		for (auto i : missing) {
			std::cout << i << " ";
		}
		std::cout << '\n';
	}
}
void Insert_Multi_Threads() {
	std::cout << "==================== Insert Random (multi-threaded) ====================" << std::endl;

	constexpr int Num_Threads = 8;
	constexpr int Num_Per_Threads = 100;
	constexpr int Num_Tests = 1000;

	constexpr int Size = Num_Threads * Num_Per_Threads;

	for (int testIdx{}; testIdx < Num_Tests; ++testIdx) {
		int values[Size];
		GetRandomValues(values, Size, testIdx);

		std::vector<std::pair<int, int>> inputs;
		RedBlackTree tree;

		std::vector<std::thread> threads;
		std::cout << "[INSERT] test(" << testIdx << ") #threads = " << Num_Threads << ", #iterations (per thread) = " << Num_Per_Threads << '\n';
		for (int i = 0; i < Num_Threads; ++i) {
			threads.push_back(std::thread(MultiThreadedInsertRandom, std::ref(tree), Num_Per_Threads, Num_Threads, i, values));
		}
		for (auto& th : threads) {
			th.join();
		}

		std::vector<int> missing;
		for (int i{}; i < Size; ++i) {
			if (tree.Contains(values[i]) == false) {
				missing.push_back(values[i]);
			}
		}
		if (missing.empty() == true) {
			std::cout << "-> found all\n";
		}
		else {
			std::cout << "missing: ";
			for (auto i : missing) {
				std::cout << i << " ";
			}
			std::cout << '\n';

			std::cout << "inputs: ";
			for (std::pair<int, int> pr : inputs) {
				std::cout << pr.first << "(threadId:" << pr.second << ")  ";
			}
			std::cout << "!!!!!!!!!!!!!!!!!!!\n";
			tree.PrintTree();
		}
	}
}

void Search_Multi_Threads() {
	std::cout << "==================== Search (multi-threaded) ====================" << std::endl;

	constexpr int Num_Threads = 8;
	constexpr int Num_Per_Threads = 100;
	constexpr int Size = Num_Threads * Num_Per_Threads;
	constexpr int Num_Tests = 1000;

	for (int testIdx{}; testIdx < Num_Tests; ++testIdx) {
		int values[Size]{};
		std::cout << "[SEARCH] test(" << testIdx << ") #threads = " << Num_Threads << ", #iterations (per thread) = " << Num_Per_Threads << '\n';

		for (int i{}; i < Size; ++i) {
			values[i] = i;
		}
		GetRandomValues(values, Size, testIdx);

		RedBlackTree tree;
		std::vector<std::pair<int, int>> inputs;

		for (int i{}; i < Size; ++i) {
			tree.Add(values[i]);
		}

		std::cout << "done searching \n";

		std::vector<std::thread> threads;
		std::vector<int> missing;
		for (int i = 0; i < Num_Threads; ++i) {
			threads.push_back(std::thread(MultiThreadedSearch, std::ref(tree), Num_Per_Threads, Num_Threads, i, values, std::ref(missing)));
		}
		for (auto& th : threads) {
			th.join();
		}

		if (missing.empty() == true) {
			std::cout << "-> found all\n";
		}
		else {
			std::cout << "missing: ";
			for (auto i : missing) {
				std::cout << i << " ";
			}
			std::cout << '\n';
		}
	}
}

int main() {

	void(*tests[])() = {
		//SIGNLE_THREAD
			//insert
				Insert_Basic,
				Insert_Random_Seed1,
				Insert_Random_Seed2,
				Insert_Random_Seed3,
			//search
				Search_Basic,
				Search_Random_Seed1,
				Search_Random_Seed2,
				Search_Random_Seed3,
		//MULTI_THREADS
			//insert
				Insert_Multi_Threads,
			//search
				Search_Multi_Threads,
			//insert + search
				//Search_Insert_Mixed_Multi_Threads //TODO
	};

	const int numTests = static_cast<int>(sizeof(tests) / sizeof(*tests));
	for (int i = 0; i < numTests; i++) {
		tests[i]();
		std::cout << std::endl << std::endl;
	}

	return 0;
}