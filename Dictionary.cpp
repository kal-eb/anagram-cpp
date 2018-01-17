#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include "Word.cpp"

using namespace std;

class Dictionary{
public:
	map<string, Word> words;
	Word anagramSeed;

	Dictionary(){}
	
	map<string, Word> getWords()
	{
		return words;
	}

	Word getSeed()
	{
		return anagramSeed;
	}

	vector<string> getWordsKeys()
	{
		vector<string> keysVector;
		for(auto const& mapElem : words) {
		  keysVector.push_back(mapElem.first);
		}
		return keysVector;
	}

	set<string> getVariantsForKey( string key)
	{
		return words.at( key).getVariants();
	}

	bool loadFromFile( string filePath, int minWordLen, string searchString)
	{
		anagramSeed.load( searchString);

		ifstream inputFile( filePath);
		if( inputFile.is_open())
		{
			string readLine;
			while( getline( inputFile, readLine))
			{							
				Word newWord;
				string newKey = newWord.load(readLine);

				if( !isSubset( newKey, anagramSeed.key) || newKey.length() < minWordLen)
					continue;

				//if key not in map, insert new word set
				if(  words.find(newKey) == words.end())
				{
					words.insert( pair<string, Word>( newKey, newWord));
				}
				else //if already exists, just add string as variant
				{
					string newVariant = newWord.getFirstVariant();
					words.at( newKey).addVariant(newVariant);
				}
			}	
			//dumpDictionary();
			//dumpAnagramSeed();
			return true;
		}
		else
			cout << "Error: Unable to open file "<<filePath<<"\n";

		return false;		
	}

	//Return a string formed by all letters in str2 that are not in str1
	static string subtractStringFrom( string str1, string str2)
	{
		vector<char> vString1( str1.begin(), str1.end());
		vector<char> vString2( str2.begin(), str2.end());
		vector<char>::iterator foundAt;
		vector<char>::iterator vstwoIt;
		for( vstwoIt = vString2.begin(); vstwoIt != vString2.end(); vstwoIt++)
		{
			foundAt = find( vString1.begin(), vString1.end(), *vstwoIt);
			if( foundAt != vString1.end())
			{
				vString1.erase(foundAt);
			}
		}
		
		string result( vString1.begin(), vString1.end());		
		return result;
	}

	//Check if chunk is a subset of universe
	static bool isSubset( string chunk, string universe)
	{
		if( chunk.length() > universe.length())
		{
			return false;
		}
		//cout << "\nChunk: "<<chunk<<" Universe: "<<universe;
		vector<char> vChunk(chunk.begin(), chunk.end());
		vector<char> vUniverse( universe.begin(), universe.end());		
		vector<char>::iterator vcIt;
		vector<char>::iterator foundAt;
		for( vcIt = vChunk.begin(); vcIt != vChunk.end(); vcIt++)
		{
			foundAt = find( vUniverse.begin(), vUniverse.end(), *vcIt);
			if( foundAt != vUniverse.end())
			{
				vUniverse.erase(foundAt);
			}
			else
			{
				return false;
			}
		}

		/*vector<char>::iterator fIt;
		cout << " TRUE! Universe after checking: ";
		for( fIt = vUniverse.begin(); fIt != vUniverse.end(); fIt++){
			cout << *fIt;
		}*/
		return true;
	}
	
	void dumpAnagramSeed()
	{
		cout << "["<< anagramSeed.key << "] -> ["<< anagramSeed.getFirstVariant() << "]\n";
	}
	void dumpDictionary()
	{
		map<string, Word>::iterator mapIt;
		for( mapIt = words.begin(); mapIt != words.end(); mapIt++)
		{
			cout <<"["<< mapIt->first << "] -> [";
			set<string>::iterator vIt;
			for( vIt = mapIt->second.variants.begin(); vIt != mapIt->second.variants.end(); vIt++)
			{
				string space = vIt == mapIt->second.variants.begin() ? "" : "|";
				cout << space << *vIt;
			}
			cout << "]\n";
		}
		cout << "(size: " << words.size() << ")\n";
	}
};

