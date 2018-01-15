#include <set>
#include <cstring>
using namespace std;

class Word{
public:
	string key;
	set<string> variants;
	Word(){}
	int load( string rawWord)
	{
		cout << rawWord << "\n";
		string plainWord = removeAccented( rawWord);
		cout << "Converted to: "<< plainWord <<"\n";
	}
private:
	string removeAccented( string inputStr ) 
	{
		char * str = new char[inputStr.length()+1];
		strcpy( str, inputStr.c_str());
		char *p = str;
cout << p << "\n";
	    while ( (*p)!=0 ) {
	        const char*
	        //   "ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿ"
	        tr = "AAAAAAECEEEEIIIIDNOOOOOx0UUUUYPsaaaaaaeceeeeiiiiOnooooo/0uuuuypy";
	        unsigned char ch = (*p);
	        cout << ch << "|";
	        if ( ch >=192 ) {
	            (*p) = tr[ ch-192 ];
	        }
	        ++p; 
	    }
	    cout <<"\n";
	    return string(str);
	}	
};