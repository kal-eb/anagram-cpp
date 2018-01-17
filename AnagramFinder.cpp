#include "Dictionary.cpp"
#include "md5.h"

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


	set<vector<string>> getAllAnagrams()
	{
		vector<string> dictKeys = dictionary.getWordsKeys();
		vector<string>::iterator dkIt;
		string anagSeed = dictionary.getSeed().key;
		set<vector<string>> result;
		for( dkIt = dictKeys.begin(); dkIt != dictKeys.end(); dkIt++)
		{
			//cout << "Iteration for Dictionary entry: "<<*dkIt<<"\n";
			//
			set<set<string>> currDictWordAnagramsKeys = searchAnagramsKeys( dictKeys, dkIt, dictKeys.end(), anagSeed);
			set<vector<string>> currAnagrams;
			if( !currDictWordAnagramsKeys.empty())
			{
				//dump_nestedStrSet(currDictWordAnagramsKeys);
				set<set<string>>::iterator cdqakIt;
				for( cdqakIt=currDictWordAnagramsKeys.begin(); cdqakIt != currDictWordAnagramsKeys.end(); cdqakIt++ )
				{
					set<vector<string>> thisIterationAnagrams = buildAnagramsFromKeysSet( *cdqakIt);
					currAnagrams.insert( thisIterationAnagrams.begin(), thisIterationAnagrams.end());
				}
			}
			if( !currAnagrams.empty())
			{
				result.insert( currAnagrams.begin(), currAnagrams.end());				
			}
		}
		//dump_nestedVecSet(result);
		return result;
	}

	//Retrieve all variants from anagrams key set and calls method for combine them
	set<vector<string>> buildAnagramsFromKeysSet( set<string> anagramKeys )
	{
		set<set<string>> anagramsSet;

		if( anagramKeys.empty())
		{
			set<vector<string>> empty;
			return empty;
		}

		set<string>::iterator akIt;
		for( akIt = anagramKeys.begin(); akIt != anagramKeys.end(); akIt++)
		{
			set<string> keyVariants;
			keyVariants = dictionary.getVariantsForKey( *akIt);
			anagramsSet.insert(keyVariants);
		}

		set<vector<string>> anagramsCombinedSet = getCombinedAnagramsSet(anagramsSet);
		return anagramsCombinedSet;		
	}
	
	set<vector<string>> getCombinedAnagramsSet( set<set<string>> multiSet)
	{
		if( multiSet.empty())
		{
			set<vector<string>> empty;
			return empty;
		}

		vector<vector<string>> vCartesianProduct = getCartesianProduct( toNestedVector(multiSet));
		//dump_nestedStrVec(vCartesianProduct);
		set<vector<string>> allPermutationsSet = getPermutations(vCartesianProduct);
		//dump_nestedVecSet(allPermutationsSet);
		return allPermutationsSet;
	}

	set<vector<string>> getPermutations( vector<vector<string>> inputVector)
	{
		set<vector<string>> sResult;
		vector<vector<string>>::iterator ivIt;
		for( ivIt= inputVector.begin(); ivIt != inputVector.end(); ivIt++)
		{			
			vector<string> curV = *ivIt;
			do
			{
				vector<string> tmpSet( curV.begin(), curV.end());
				sResult.insert( tmpSet);
			}while( next_permutation( curV.begin(), curV.end()));
		}

		return sResult;
	}

	vector<vector<string>> getCartesianProduct (const vector<vector<string>>& inputVector)
	{
	    vector<vector<string>> vResult = {{}};
	    for( auto& ivIt : inputVector)
	    {
	        vector<vector<string>> auxVector;
	        for (auto& rvIt : vResult)
	        {
	            for (auto iivIt : ivIt)
	            {
	                auxVector.push_back(rvIt);
	                auxVector.back().push_back(iivIt);
	            }
	        }
	        vResult.swap(auxVector);
	    }
	    return vResult;
	}

	vector<vector<string>> toNestedVector( set<set<string>> multiSet)
	{
		vector<vector<string>> vResult;
		set<set<string>>::iterator msIt;
		for( msIt = multiSet.begin(); msIt != multiSet.end(); msIt++)
		{
			vector<string> sToV( msIt->begin(), msIt->end());
			vResult.push_back(sToV);
		}

		return vResult;
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

	map<string,string> convertToPhrase( set<vector<string>> nestedVecSet)
	{
		map<string, string> resultMap;
		set<vector<string>>::iterator nvIt;
		for( nvIt = nestedVecSet.begin(); nvIt != nestedVecSet.end(); nvIt++)
		{
			vector<string> currVec = *nvIt;
			vector<string>::iterator cvIt;
			string str = "";
			for( cvIt = currVec.begin(); cvIt != currVec.end(); cvIt++)
			{
				string sp = str.length() > 0 ? " " : "";
				str += sp + *cvIt;
			}

			char *cstr = new char[str.length() + 1];
			strcpy(cstr, str.c_str());
			
			//using third party md5 library
			MD5 md5 ;
			const char *md5sum = md5.digestString( cstr);
			string strMD5( md5sum);
			delete [] cstr;

			resultMap.insert(pair<string, string>( strMD5, str));
			//cout << str << "\n";
		}

		return resultMap;
	}

	void dump_nestedVecSet( set<vector<string>> nestedVecSet)
	{
		set<vector<string>>::iterator nnsIt;
		cout<<"*********** printing nested string vector****************\n";
		for( nnsIt = nestedVecSet.begin(); nnsIt != nestedVecSet.end(); nnsIt++)
		{
			vector<string> currSet = *nnsIt;
			vector<string>::iterator csIt;
			for( csIt = currSet.begin(); csIt != currSet.end(); csIt++)
			{
				cout << *csIt << " ";
			}
			cout << "\n";
		}		
	}

	void dump_nestedStrVec( vector<vector<string>> nestedStrVec)
	{
		vector<vector<string>>::iterator nnsIt;
		cout<<"*********** printing nested string vector****************\n";
		for( nnsIt = nestedStrVec.begin(); nnsIt != nestedStrVec.end(); nnsIt++)
		{
			vector<string> currSet = *nnsIt;
			vector<string>::iterator csIt;
			for( csIt = currSet.begin(); csIt != currSet.end(); csIt++)
			{
				cout << *csIt << " ";
			}
			cout << "\n";
		}
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