// Marius Rejdak
// Informatyka, mgr, OS1

/*

Running:
#!/bin/fish
g++ -Wall -std=c++11 -o3 -o zad1 zad1.cpp; and ./zad1

Output:
English to std::set 0.080332s
Polish to std::set 2.55033s
English to std::unordered_set 0.020802s
Polish to std::unordered_set 0.530011s
English searching in std::set 0.067151s
Polish searching in std::set 2.33132s
English searching in std::unordered_set 0.015167s
Polish searching in std::unordered_set 0.42904s
Polish searching in std::set for ratio 0.05 0.112123s
Polish searching in std::unordered_set for ratio 0.05 0.022894s
Polish searching in std::set for ratio 0.1 0.22494s
Polish searching in std::unordered_set for ratio 0.1 0.046557s
Polish searching in std::set for ratio 0.2 0.449548s
Polish searching in std::unordered_set for ratio 0.2 0.089799s
Polish searching in std::set for ratio 0.4 0.907567s
Polish searching in std::unordered_set for ratio 0.4 0.180477s
Polish searching in std::set for ratio 0.7 1.60857s
Polish searching in std::unordered_set for ratio 0.7 0.310871s
Polish searching in std::set for ratio 1 2.31883s
Polish searching in std::unordered_set for ratio 1 0.429663s

*/

#include <iostream>     // std::cout
#include <fstream>		// std::ifstream
#include <iterator>		// std::istream_iterator
#include <algorithm>    // std::sort, std::random_shuffle
#include <vector>       // std::vector
#include <string>		// std::string
#include <ctime>        // time, clock, clock_t, CLOCKS_PER_SEC
#include <cmath>		// std::round
#include <set>			// std::set
#include <unordered_set> // std::unordered_set

template <class AssocContainer, class ForwardIterator>
float insert_time(AssocContainer &container, ForwardIterator iter_begin, ForwardIterator iter_end)
{
	clock_t clockStart = clock();
	container.insert(iter_begin, iter_end);
	return ((float)clock()-clockStart)/CLOCKS_PER_SEC;
}

template <class AssocContainer, class Container>
float search_time(AssocContainer &container, Container &search_items)
{
	clock_t clockStart = clock();
	for(auto item : search_items)
	{
		container.find(item);
	}
	return ((float)clock()-clockStart)/CLOCKS_PER_SEC;
}

int main(int argc, char *argv[]) {
	std::ifstream in_english("english.dic", std::ifstream::in);
	std::ifstream in_polish("polish.dic", std::ifstream::in);
	std::istream_iterator<std::string> in_iter_english(in_english), in_iter_polish(in_polish), eos;
	std::vector<std::string> lines_english(in_iter_english, eos);
	std::vector<std::string> lines_polish(in_iter_polish, eos);

	srand(unsigned(time(0)));
	std::random_shuffle(lines_english.begin(), lines_english.end());
	std::random_shuffle(lines_polish.begin(), lines_polish.end());

	std::set<std::string> set_polish, set_english;
	std::unordered_set<std::string> uset_polish, uset_english;
	
	std::cout << "English to std::set " << insert_time(set_english, lines_english.begin(), lines_english.end()) << "s\n";
	std::cout << "Polish to std::set " << insert_time(set_polish, lines_polish.begin(), lines_polish.end()) << "s\n";
	std::cout << "English to std::unordered_set " << insert_time(uset_english, lines_english.begin(), lines_english.end()) << "s\n";
	std::cout << "Polish to std::unordered_set " << insert_time(uset_polish, lines_polish.begin(), lines_polish.end()) << "s\n";

	std::cout << "English searching in std::set " << search_time(set_english, lines_english) << "s\n";
	std::cout << "Polish searching in std::set " << search_time(set_polish, lines_polish) << "s\n";
	std::cout << "English searching in std::unordered_set " << search_time(uset_english, lines_english) << "s\n";
	std::cout << "Polish searching in std::unordered_set " << search_time(uset_polish, lines_polish) << "s\n";

	float slices[] = {0.05, 0.1, 0.2, 0.4, 0.7, 1};
	std::vector<std::string> test_lines;

	for(float ratio : slices)
	{
		test_lines.insert(test_lines.end(), lines_polish.begin(), lines_polish.begin()+(int)round(lines_polish.size()*ratio));
		std::cout << "Polish searching in std::set for ratio " << ratio << " " << search_time(set_polish, test_lines) << "s\n";
		std::cout << "Polish searching in std::unordered_set for ratio " << ratio << " " << search_time(uset_polish, test_lines) << "s\n";
		test_lines.clear();
	}

	return 0; //Huge success!
}