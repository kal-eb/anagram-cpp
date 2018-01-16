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

				if( !isSubset( newKey, anagramSeed.key))
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
			dumpDictionary();
			//dumpAnagramSeed();
			return true;
		}
		else
			cout << "Error: Unable to open file "<<filePath<<"\n";

		return false;		
	}

	static bool isSubset( string chunk, string universe)
	{
		if( chunk.length() > universe.length())
		{
			return false;
		}

		vector<char> vChunk(chunk.begin(), chunk.end());
		vector<char> vUniverse( universe.begin(), universe.end());
		//cout << "\nChunk: "<<chunk<<" Universe: "<<universe;
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

