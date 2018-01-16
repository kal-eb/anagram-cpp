#include <set>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <string>
//Using UTF8-CPP library
#include "utf8.h"
using namespace std;

class Word{
public:
	string key;
	set<string> variants;
	
	Word(){}
	
	string load( string rawWord)
	{
		string plainWord = removeAccented( rawWord);		
		string onlyAlpha = stripInvalidChars(plainWord);
		string sortedWord = sortWord(onlyAlpha);
		key = sortedWord;
		variants.insert(rawWord);
		//cout << rawWord << "->Converted to: "<< plainWord <<" | "<<onlyAlpha<<"| "<<sortedWord<<"("<<sortedWord.length()<<")\n";
 		/*std::cout << "variants contains:";
 		set<string>::iterator it;
  		for (it=variants.begin(); it!=variants.end(); ++it)
    		std::cout << ' ' << *it;
  		std::cout << '\n';	*/
  		return key;	
	}

	bool addVariant( string variant)
	{
		pair<set<string>::iterator, bool> result;
		result = variants.insert(variant);
		return result.second;
	}

	string getFirstVariant()
	{
		set<string>::iterator vIt = variants.begin();
		return *vIt;
	}
	
	static bool isInvalidChar(char c)
	{
	    switch(c)
	    {
		    case '(':
		    case ')':
		    case '-':
		    case '\'':
		    case ' ':
		        return true;
		    default:
		        return false;
	    }
	}

	static string sortWord( string unsorted)
	{
		if( unsorted.length() < 1)
		{
			return "";
		}
		char letters[unsorted.length()+1] = "";
		strcpy( letters, unsorted.c_str());
		sort( letters, letters + unsorted.length());
		return string(letters);
	}

private:
	string stripInvalidChars( string input)
	{
		string str = input;
		str.erase(std::remove_if(str.begin(), str.end(), &isInvalidChar), str.end());
    	return str;
	}

	string removeAccented( string inputStr ) 
	{
		//               "ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿ"
		const char* tr = "AAAAAAECEEEEIIIIDNOOOOOx0UUUUYPsaaaaaaeceeeeiiiiOnooooo/0uuuuypy";	        	        
	    char* str = (char*)inputStr.c_str();    // utf-8 string
	    char* str_i = str;                  	// string iterator
	    char* end = str+strlen(str)+1;      	// end iterator
	    unsigned char symbol[5] = {0,0,0,0,0};

	    char result[inputStr.length()+1] = "";
	    int resPtr = 0;
	    //cout << symbol << endl;
	    do
	    {
	        uint32_t code = utf8::next(str_i, end); // get 32 bit code of a utf-8 symbol
	        if (code == 0)
	            continue;

	        //cout << "utf 32 code:" << code << endl;
			memset(symbol, 0, sizeof(symbol));
	        utf8::append(code, symbol); // initialize array `symbol`
	        result[resPtr] = code >= 192 ? tr[code - 192] : code;	        
	        //cout << symbol;
	        resPtr++;	        
	    }
	    while ( str_i < end );
	    //cout << "Array of c:'"<< string(result) << "' ("<<strlen(result)<<")\n";
	    return string(result);
	}	
};