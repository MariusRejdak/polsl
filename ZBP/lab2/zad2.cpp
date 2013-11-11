// Marius Rejdak
// Informatyka, mgr, OS1

/*

Running:
#!/bin/fish
g++ -Wall -std=c++11 -o3 -o zad2 zad2.cpp
./zad2 polish.dic << cat text.txt

*/

#include <iostream>     // std::cout, std::cin
#include <fstream>		// std::ifstream
#include <iterator>		// std::istream_iterator
#include <string>		// std::string
#include <sstream>		// std::ostringstream
#include <vector>		// std::vector
#include <unordered_set> // std::unordered_set
#include <ctype.h>		// isupper, tolower

enum STRING_CASE {STRING_CASE_Lower, STRING_CASE_First_Upper, STRING_CASE_All_Upper, STRING_CASE_Mixed};

enum STRING_CASE test_string_case(std::string &s)
{
	size_t num_upper_case = 0, num_lower_case = 0, i = 0;
	bool first_upper_case = false;

	for(auto c = s.begin(); c < s.end(); ++c, ++i)
	{
		if(isupper(*c))
		{
			++num_upper_case;
			if(i == 0)
			{
				first_upper_case = true;
			}
		}
		else
		{
			++num_lower_case;
		}
	}

	if(num_lower_case == s.size())
		return STRING_CASE_Lower;
	else if(first_upper_case && num_upper_case == 1)
		return STRING_CASE_First_Upper;
	else if(num_upper_case == s.size())
		return STRING_CASE_All_Upper;
	else
		return STRING_CASE_Mixed;
}

void string_to_lower(std::string &s, size_t first = 0)
{
	for(auto c = s.begin()+first; c < s.end(); ++c)
	{
		*c = tolower(*c);
	}
}

bool spellchecker(std::string &word, std::unordered_set<std::string> &dictionary)
{
	enum STRING_CASE word_case = test_string_case(word);
	std::string word_temp(word);
	std::unordered_set<std::string> possible_words;

	switch(word_case)
	{
	case STRING_CASE_All_Upper:
		possible_words.insert(word);
		string_to_lower(word_temp, 1);
	case STRING_CASE_First_Upper:
		possible_words.insert(word_temp);
		string_to_lower(word_temp);
	case STRING_CASE_Lower:
		possible_words.insert(word_temp);
		break;
	case STRING_CASE_Mixed:
	default:
		possible_words.insert(word);
	}

	for(auto test : possible_words)
	{
		if(dictionary.find(test) != dictionary.end())
			return true;
	}
	return false;
}

std::vector<std::string> get_words()
{
	std::vector<std::string> words;
	std::string word;
	while(std::cin >> word)
	{
		words.push_back(word);
	}
	return words;
}

std::string set_color(int color)
{
	std::ostringstream stream;
	stream << "\033[" << color << "m";
	return stream.str();
}

std::string print_with_context(std::vector<std::string>::iterator error, std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end)
{
	std::ostringstream stream;
	for(auto i = error-2; i >= begin && i < error; ++i)
	{
		stream << *i << " ";
	}
	stream << set_color(31) << *error << set_color(0) << " ";
	for(auto i = error+1; i <= error+2 && i < end; ++i)
	{
		stream << *i << " ";
	}
	stream << std::endl;
	return stream.str();
}

int main(int argc, char *argv[])
{
	std::ifstream in_file(argv[1], std::ifstream::in);
	std::istream_iterator<std::string> in_iter_file(in_file), in_iter_eof;
	std::unordered_set<std::string> uset_words(in_iter_file, in_iter_eof);
	std::vector<std::string> text = get_words();

	std::cout << "Found errors:" << std::endl;
	for(auto word_iter = text.begin(); word_iter < text.end(); ++word_iter)
	{
		if(!spellchecker(*word_iter, uset_words))
		{
			std::cout << print_with_context(word_iter, text.begin(), text.end());
		}
	}

	return 0; //Huge success!
}