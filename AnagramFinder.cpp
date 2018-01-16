#include "Dictionary.cpp"

using namespace std;

class AnagramFinder{
public:	
	Dictionary dictionary;

	AnagramFinder(){}

	bool initialize( string filePath, int minWordLen, string searchString){
		return dictionary.loadFromFile( filePath, minWordLen, searchString);
	}


	set<set<string>> getAllAnagrams()
	{
		vector<string> dictKeys = dictionary.getWordsKeys();
		vector<string>::iterator dkIt;
		string anagSeed = dictionary.getSeed().key;
		set<set<string>> result;
		for( dkIt = dictKeys.begin(); dkIt != dictKeys.end(); dkIt++)
		{
			set<set<string>> currDictWordAnagrams = searchAnagrams( dictKeys, dkIt, anagSeed);
		}

		return result;
	}

	//Recursive method for searching anagrams for all dictionary keys that match anagramSeed criteria
	set<set<string>> searchAnagrams( vector<string> dictionaryKeys, vector<string>::iterator currposIt, string anagramSeed)
	{
		//no more dictionaty entries to check or anagramSeed is depleted 
		if( currposIt == dictionaryKeys.end() || anagramSeed.length() < 1)
		{
			set<set<string>> empty;
			return empty;
		}
		//cout << "currposIt:" << *currposIt << " anagramSeed:" << anagramSeed << "\n";
		//if the current anagramSeed key is the same as current dictionary key, the basic case has been reached
		Word currSeedWord;
		currSeedWord.load( anagramSeed);
		if( currSeedWord.key.compare(*currposIt) == 0 )
		{
			set<set<string>> nestedAnagramSet;
			set<string> innerAnagramSet;
			innerAnagramSet.insert(*currposIt);
			nestedAnagramSet.insert(innerAnagramSet);
			return nestedAnagramSet;
		}

		set<set<string>> empty;
		return empty;
	}
};