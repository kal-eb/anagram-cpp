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
		cout << "Initializing Anagram Finder...\n";
		if( aFinder.initialize( argv[1], atoi(argv[2]), argv[3]))
		{
			vector<string> md5hashSolutions = {
				"e4820b45d2277f3844eac66c903e84be", 
				"23170acc097c24edb98fc5488ab033fe", 
				"665e5bcb0c20062fe8abaaf4628bb154"};

			cout << "Done.\nSearching for anagrams...\n";
			set<vector<string>> anagramsSet = aFinder.getAllAnagrams();
			cout << "Done.\nBuilding results...\n";
			map<string, string> phrases = aFinder.convertToPhrase( anagramsSet);

			map<string, string>::iterator mIt; 
			cout << "Anagram Phrases Found for '" << argv[3] << "'\n";
			for( mIt = phrases.begin(); mIt != phrases.end(); mIt++)
			{
				cout <<"["<< mIt->first << "] -> " << mIt->second << "\n";
			}

			vector<string>::iterator solIt;
			for( solIt = md5hashSolutions.begin(); solIt != md5hashSolutions.end(); solIt++)
			{
				cout << "Searching for solution phrase with md5 = " << *solIt <<"\n";
				if( phrases.find(*solIt) != phrases.end())
				{
					cout << "FOUND! -> "<< phrases.at(*solIt)<<"\n";
				}
				else
				{
					cout << "Not found\n";
				}
				
			}
		}
	}

	return 0;
}