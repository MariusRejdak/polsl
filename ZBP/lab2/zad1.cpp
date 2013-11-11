// Marius Rejdak
// Informatyka, mgr, OS1

/*

Running:
#!/bin/fish
g++ -Wall -std=c++11 -o3 -o file1 file1.cpp; and ./file1

*/

#include <iostream>     // std::cout
#include <fstream>		// std::ifstream
#//include <functional>	// std::function
#include <iterator>		// std::istream_iterator
#include <algorithm>    // std::sort, std::random_shuffle
#include <vector>       // std::vector
#include <string>		// std::string
#include <ctime>        // time, clock, clock_t, CLOCKS_PER_SEC
#include <cmath>		// std::round
//#include <cstdlib>      // srand, qsort
//#include <list>			// std::list
#include <set>			// std::set
#include <unordered_set> // std::unordered_set

//using namespace std; // http://stackoverflow.com/questions/1452721/why-is-using-namespace-std-considered-bad-practice

//template <class T>
//float get_time(T& container, std::function<void(T&)> sorting_function) {
//	clock_t clockStart = clock();
//	sorting_function(container);
//	return ((float)clock()-clockStart)/CLOCKS_PER_SEC;
//}

template <class E, class C, class S>
void insert_vector_to_set(C &cont, S &set)
{
	for(E item : cont)
	{
		set.insert(item);
	}
}

template <class E, class C, class S>
void test_searching_in_set(C &cont, S &set)
{
	for(E item : cont)
	{
		set.find(item);
	}
}

int main(int argc, char *argv[]) {
	clock_t clockStart;

	std::ifstream in_english("english.dic", std::ifstream::in);
	std::ifstream in_polish("polish.dic", std::ifstream::in);
	std::istream_iterator<std::string> in_iter_english(in_english), in_iter_polish(in_polish), eos;
	std::vector<std::string> lines_english(in_iter_english, eos);
	std::vector<std::string> lines_polish(in_iter_polish, eos);

	srand(unsigned(time(0)));
	std::random_shuffle(lines_english.begin(), lines_english.end());
	std::random_shuffle(lines_polish.begin(), lines_polish.end());

	std::vector<std::string> test_lines_english(lines_english.begin(), lines_english.begin()+10000);
	std::vector<std::string> test_lines_polish(lines_polish.begin(), lines_polish.begin()+10000);

	std::set<std::string> set_polish, set_english;
	std::unordered_set<std::string> uset_polish, uset_english;

	
	clockStart = clock();
	insert_vector_to_set<std::string>(lines_english, set_english);
	std::cout << "English to std::set " << ((float)clock()-clockStart)/CLOCKS_PER_SEC << "s\n";

	clockStart = clock();
	insert_vector_to_set<std::string>(lines_polish, set_polish);
	std::cout << "Polish to std::set " << ((float)clock()-clockStart)/CLOCKS_PER_SEC << "s\n";

	clockStart = clock();
	insert_vector_to_set<std::string>(lines_english, uset_english);
	std::cout << "English to std::unordered_set " << ((float)clock()-clockStart)/CLOCKS_PER_SEC << "s\n";

	clockStart = clock();
	insert_vector_to_set<std::string>(lines_polish, uset_polish);
	std::cout << "Polish to std::unordered_set " << ((float)clock()-clockStart)/CLOCKS_PER_SEC << "s\n";

	clockStart = clock();
	test_searching_in_set<std::string>(test_lines_english, set_english);
	std::cout << "English searching in std::set " << ((float)clock()-clockStart)/CLOCKS_PER_SEC << "s\n";

	clockStart = clock();
	test_searching_in_set<std::string>(test_lines_polish, set_polish);
	std::cout << "Polish searching in std::set " << ((float)clock()-clockStart)/CLOCKS_PER_SEC << "s\n";

	clockStart = clock();
	test_searching_in_set<std::string>(test_lines_english, uset_english);
	std::cout << "English searching in std::unordered_set " << ((float)clock()-clockStart)/CLOCKS_PER_SEC << "s\n";

	clockStart = clock();
	test_searching_in_set<std::string>(test_lines_polish, uset_polish);
	std::cout << "Polish searching in std::unordered_set " << ((float)clock()-clockStart)/CLOCKS_PER_SEC << "s\n";
	

	float slices[] = { 0.05, 0.1, 0.2, 0.4, 0.7, 1};
	test_lines_polish.clear();
	test_lines_polish.insert(test_lines_polish.end(), lines_polish.begin(), lines_polish.begin()+(int)round(lines_polish.size()*slices[0]));

	for(float ratio : slices)
	{
		set_polish.clear();
		set_polish.insert(lines_polish.begin(), lines_polish.begin()+(int)round(lines_polish.size()*ratio));
		clockStart = clock();
		test_searching_in_set<std::string>(test_lines_polish, set_polish);
		std::cout << "Polish searching in std::set for ratio " << ratio << " " << ((float)clock()-clockStart)/CLOCKS_PER_SEC << "s\n";
	}

	for(float ratio : slices)
	{
		uset_polish.clear();
		uset_polish.insert(lines_polish.begin(), lines_polish.begin()+(int)round(lines_polish.size()*ratio));
		clockStart = clock();
		test_searching_in_set<std::string>(test_lines_polish, uset_polish);
		std::cout << "Polish searching in std::unordered_set for ratio " << ratio << " " << ((float)clock()-clockStart)/CLOCKS_PER_SEC << "s\n";
	}

	return 0; //Huge success!
}