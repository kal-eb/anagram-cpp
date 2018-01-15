#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include "Word.cpp"

using namespace std;

class Dictionary{
public:
	Dictionary(){}
	map<string, Word> words;
	int loadFromFile( string filePath, int minWordLen, string searchString)
	{
		ifstream inputFile( filePath);
		if( inputFile.is_open())
		{
			string readLine;
			while( getline( inputFile, readLine))
			{				
				Word newWord;
				newWord.load(readLine);
			}	
			return 1;
		}
		else
			cout << "Error: Unable to open file "<<filePath<<"\n";

		return 0;		
	}
};

