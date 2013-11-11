// Marius Rejdak
// Informatyka, mgr, OS1

/*

Running:
#!/bin/fish
g++ -Wall -std=c++11 -o3 -o file2 file2.cpp; and ./file2

*/

#include <iostream>	 	// std::cout, std::endl
#include <vector>		// std::vector

//
// Zad 3
//
template <class BidirectionalIterator>
bool next_combination(BidirectionalIterator first1, BidirectionalIterator last1, BidirectionalIterator first2, BidirectionalIterator last2)
{
	bool b = false;
	BidirectionalIterator vi_it = last1-1, vc_it;

	for(auto it1 = last2-1; it1 != first2 || it1 == first2; --it1, --vi_it)
	{
		if(*it1 == *vi_it)
		{
			if(it1 != first2)
			{
				b = true;
				vc_it = it1-1;
				continue;
			}
			else
			{
				return false;
			}
		}
		else
		{
			if(b)
			{
				auto tmp = it1;
				for(auto it2 = first1; it2 != last1; ++it2)
				{
					if(*vc_it == *it2)
					{
						tmp = it2+1;
						break;
					}
				}

				for(auto it3 = vc_it; it3 != last2; ++it3, ++tmp)
				{
					*it3 = *tmp;
				}
				
				return true;
			}
			for(auto it2 = first1; it2 != last1; ++it2)
			{
				if(*it1 == *it2)
				{
					*it1 = *(++it2);
					return true;
				}
			}
		}
	}
	return true;
}

template <class T>
void show_collection(T &c)
{
	for(auto i : c)
	{
		std::cout << i << " ";
	}
	std::cout << std::endl;
}

int main(int argc, char *argv[]) {

	char tab [10] = {'A', 'B', 'C', 'E', 'G', 'I', 'M', 'O', 'P', 'Y'};
	std::vector<char> vi(tab, tab+10);
	std::vector<char> vc(tab, tab+4);

	do
	{
		show_collection(vc);
	} 
	while(next_combination(vi.begin(), vi.end(), vc.begin(), vc.end()));

	return 0; //Huge success!
}