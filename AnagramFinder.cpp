#include <string>
#include "Dictionary.cpp"

using namespace std;

class AnagramFinder{
public:
	AnagramFinder(){}
	Dictionary dictionary;

	int initialize( string filePath, int minWordLen, string searchString){
		dictionary.loadFromFile( filePath, minWordLen, searchString);
		return 0;
	}
		
};