#include "Dictionary.cpp"

using namespace std;

class AnagramFinder{
public:	
	Dictionary dictionary;
	int minWordLength;

	AnagramFinder(){}

	bool initialize( string filePath, int minWordLen, string searchString){
		minWordLength = minWordLen;
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
			//cout << "Iteration for Dictionary entry: "<<*dkIt<<"\n";
			//
			set<set<string>> currDictWordAnagramsKeys = searchAnagramsKeys( dictKeys, dkIt, dictKeys.end(), anagSeed);
			set<set<string>> currAnagrams;
			if( !currDictWordAnagramsKeys.empty())
			{
				//dump_nestedStrSet(currDictWordAnagramsKeys);
				set<set<string>>::iterator cdqakIt;
				for( cdqakIt=currDictWordAnagramsKeys.begin(); cdqakIt != currDictWordAnagramsKeys.end(); cdqakIt++ )
				{
					set<set<string>> thisIterationAnagrams;
					thisIterationAnagrams = buildAnagramsFromKeysSet( *cdqakIt);
					currAnagrams.insert( thisIterationAnagrams.begin(), thisIterationAnagrams.end());
				}
			}
			if( !currAnagrams.empty())
			{
				dump_nestedStrSet(currAnagrams);
			}
		}

		return result;
	}

	//Retrieve all variants from anagrams key set and calls method for combine them
	set<set<string>> buildAnagramsFromKeysSet( set<string> anagramKeys )
	{
		set<set<string>> anagramsSet, anagramsCombinedSet;

		if( anagramKeys.empty())
		{
			set<set<string>> empty;
			return empty;
		}

		set<string>::iterator akIt;
		for( akIt = anagramKeys.begin(); akIt != anagramKeys.end(); akIt++)
		{
			set<string> keyVariants;
			keyVariants = dictionary.getVariantsForKey( *akIt);
			anagramsSet.insert(keyVariants);
		}

		anagramsCombinedSet = getCombinedAnagramsSet(anagramsSet);
		return anagramsCombinedSet;
		//return anagramsSet;
	}
	
	set<set<string>> getCombinedAnagramsSet( set<set<string>> multiSet)
	{
		if( multiSet.empty())
		{
			set<set<string>> empty;
			return empty;
		}
		set<set<string>> cartesianProd;
		cartesianProd = getCartesianProduct( multiSet);

		return cartesianProd;
	}

	set<set<string>> getCartesianProduct( set<set<string>> multiSet)
	{
		set<set<string>> multipliedMultiset;
		set<string> tempStrSet;

		set<set<string>>::iterator msIt;		
		for( msIt = multiSet.begin(); msIt != multiSet.end(); msIt++)
		{
			set<string>::iterator sIt;
			set<string> stringSet = *msIt;
			for( sIt = stringSet.begin(); sIt != stringSet.end(); sIt++)
			{
				multipliedMultiset.insert( *sIt);
				set<set<string>>::iterator mmsIt;		
				for( mmsIt = ++msIt; mmsIt != multiSet.end(); mmsIt++)
				{
					set<string>::iterator mmmsIt;
					set<string> mmstringSet = *mmsIt;
					for( mmmsIt = mmstringSet.begin(); mmmsIt != mmstringSet.end(); mmmsIt++)
					{
						tempStrSet.insert( *mmmsIt);						
					}
				}
				multipliedMultiset.insert( stringSet);
				tempStrSet.erase( tempStrSet.begin(), tempStrSet.end());
			}
		}

		return multipliedMultiset;
	}

	//Recursive method for searching anagrams for all dictionary keys that match anagramSeed criteria
	set<set<string>> searchAnagramsKeys( vector<string> dictionaryKeys, vector<string>::iterator currposIt, vector<string>::iterator lastposIt, string anagramSeed)
	{
		//vector<string>::iterator currposIt = inputIterator;
		//no more dictionaty entries to check or anagramSeed is depleted 
		if( currposIt == lastposIt || anagramSeed.length() < minWordLength)
		{
			set<set<string>> empty;
			return empty;
		}

		string currDictionaryKey = *currposIt;
		//cout << "currposIt:" << *currposIt << " currDictionaryKey:" << currDictionaryKey << " anagramSeed:" << anagramSeed << "\n";
		//if the current anagramSeed key is the same as current dictionary key, the basic case has been reached
		Word currSeedWord;
		currSeedWord.load( anagramSeed);
		if( currSeedWord.key.compare( currDictionaryKey) == 0 )
		{
			set<set<string>> nestedAnagramSet;
			set<string> innerAnagramSet;
			innerAnagramSet.insert( currDictionaryKey);
			nestedAnagramSet.insert( innerAnagramSet);
			return nestedAnagramSet;
		}

		//here is where recursion takes place, when the anagramSeed is still bigger than the dictionary entry		
		if( Dictionary::isSubset( currDictionaryKey, anagramSeed))
		{
			//generating a new anagram seed removing all current dictionary key letters from anagram seed
			string newAnagramSeed = Dictionary::subtractStringFrom( anagramSeed, currDictionaryKey);
			//cout << "anagramSeed: "<<anagramSeed<<" currDictionaryKey: "<<currDictionaryKey<<" newAnagramSeed: "<<newAnagramSeed<<"\n";
			//new anagram seed was generated
			if( newAnagramSeed.length() >= minWordLength )
			{
				set<set<string>> nestedAnagramSet;
				//check next entry in dictionary keys set
				//vector<string>::iterator innerIt;				
				//for( innerIt = currposIt++; innerIt != lastposIt; innerIt++)
				for( currposIt++; currposIt != lastposIt; currposIt++)
				{
					//This is the anagram set for current dictionary key
					//cout << "Calling recursion with "<<*currposIt<< " was "<<currDictionaryKey <<" and "<< newAnagramSeed << "\n";
					set<set<string>> nestedAnagramSetRec = searchAnagramsKeys( dictionaryKeys, currposIt, lastposIt, newAnagramSeed);
					if( !nestedAnagramSetRec.empty())
					{
						//to form a real anagram, add the curernt dictionary key to all anagrms found
						//cout << "Oringal nested string set: \n";
						//dump_nestedStrSet(nestedAnagramSetRec);
						//cout << "Now adding " << currDictionaryKey << "\n";
						set<set<string>>updatedNestedAnagramSetRec = addWordToAllAnagramsSets( currDictionaryKey, nestedAnagramSetRec);
						//dump_nestedStrSet(updatedNestedAnagramSetRec);
						if( !updatedNestedAnagramSetRec.empty())
							nestedAnagramSet.insert( updatedNestedAnagramSetRec.begin(), updatedNestedAnagramSetRec.end());
					}
				}

				return nestedAnagramSet;
			}
		}

		set<set<string>> empty;		
		return empty;
	}

	set<set<string>> addWordToAllAnagramsSets( string wordToAdd, set<set<string>> nestedStrSet)
	{
		set<set<string>> newNestedStrSet;
		set<set<string>>::iterator nnsIt;

		if( wordToAdd.length() < 1)
		{
			set<set<string>> empty;		
			return empty;
		}

		for( nnsIt = nestedStrSet.begin(); nnsIt != nestedStrSet.end(); nnsIt++)
		{
			set<string> currSet = *nnsIt;
			//This should not really happen
			if( currSet.empty())
			{
				continue;
			}
			currSet.insert( wordToAdd);
			newNestedStrSet.insert( currSet);
		}

		return newNestedStrSet;
	}

	void dump_nestedStrSet( set<set<string>> nestedStrSet)
	{
		set<set<string>>::iterator nnsIt;
		cout<<"*********** printing nested string set****************\n";
		for( nnsIt = nestedStrSet.begin(); nnsIt != nestedStrSet.end(); nnsIt++)
		{
			set<string> currSet = *nnsIt;
			set<string>::iterator csIt;
			for( csIt = currSet.begin(); csIt != currSet.end(); csIt++)
			{
				cout << *csIt << " ";
			}
			cout << "\n";
		}
	}
};