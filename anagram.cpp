#include <iostream>
#include "AnagramFinder.cpp"

using namespace std;

int main( int argc, char *argv[])
{
	if( argc != 4)
	{
		cout << "Anagram finder for single or multiple word.\n";
		cout << "Use: " << argv[0] << " <dictionary path>" << " <min word length>" << " <input phrase/word>\n";
	}
	else
	{
		AnagramFinder aFinder;
		if( aFinder.initialize( argv[1], atoi(argv[2]), argv[3]))
		{
			aFinder.getAllAnagrams();
		}
	}

	return 0;
}