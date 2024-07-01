#include <iostream>
#include <fstream>
#include <string.h>
#include <map>
#include <stdlib.h>
#include <cstdio>

using namespace std;

bool checkForCommandLine ( int argumentCount , char* argumentArray [ ] );
bool checkIfZip ( string fileName );
void bringInCodes ( string fileName , map < string , char > codes );
void translateCompressedData ( string fileName , string compressedData , map < string , char > codes );
void renameFile ( string fileName );

int main ( int argumentCount , char* argumentArray [ ] ) {
	
	string input = argumentArray [ 1 ];

	cout << input << endl;


	bool isCommandLineValid = checkForCommandLine ( argumentCount , argumentArray );
	if ( ! isCommandLineValid )
		return 1;

	string fileName = argumentArray [ 1 ];
	
	bool isFileZip = checkIfZip ( fileName );
	if ( ! isFileZip )
		return 1;

	map< string , char > codes;
	bringInCodes ( fileName , codes );
	
	renameFile ( fileName );

return 0;
}

bool checkForCommandLine ( int argumentCount , char* argumentArray [ ] ) {

	//Pre:   The number of additional arguments as well as the array where those arguments from the 
	//	 command line are stored are passed in as parameters.
	//Post:  If there is not only one additional argument in the command line when the program is 
	//	 executed, then this method will print out a statement to the user as well as return 
	//	 false.  The main method take this bool and if it is false, will return a 1 so the program
	//	 terminates.  
	//Note:  One extra argument means two total arguments ./a.out counts as one argument.


	if ( argumentCount != 2 ) { 

		cout << "\tBad Filename Entered On Command Line -- Now Aborting." << endl;
		return false;	
	
	}
	
	return true;

}

bool checkIfZip ( string fileName ) {

	//Pre:   The fileName pulled from the command line is passed in via parameter.
	//Post:  The file name is checked to see if it contains ".zip".  if it does not, 
	//	 the bool method returns a false so the main method can check this then 
	//	 return a 1, therefore terminating the program.  
	//Note:  The find function returns an arbitrarily large integer if the substring 
	//	 is not found, therefore by comparing this integer to the length of the 
	//	 string, this can be used to see if the substring was found or not.

	int zipLocation = fileName.find ( ".zip" );

	if ( zipLocation > fileName.length ( ) ) {

		cout << "\tFile Entered Is Not A Zip File -- Now Aborting." << endl;
		return false;
	}	
	
	return true;

}

void bringInCodes ( string fileName , map < string , char > codes ) {

	//Pre:   The file name and a map of string and char are passed in via parameter.
	//Post:  The data from the compressed file consists of an ascii value as well as 
	//	 its compressed binary form.  Each code and its corresponding ascii value, 
	//	 which is converted into the correct character, are stored in the map passed
	//	 in via parameter.  This will be used to easily translate the data.  Finally, 
	//	 the last string in the compressed file, the full string of the real compressed 
	//	 data is stored in a variable, and that variable is passed onto the method 
	//	 translateCompressedData.  
	//Note:  The last string in the compressed file is pulled in this method so that another
	//	 file in stream does not need to be declared in the future.

	ifstream inFile ( fileName.c_str ( ) );	

	int numCharacters;
	inFile >> numCharacters;

	for ( int characterCount = 0 ; characterCount < numCharacters ; characterCount++ ) {

		int asciiValue;
		inFile >> asciiValue;

		string huffmanCode;
		inFile >> huffmanCode;
			
		char asciiCharacter = ( char ) asciiValue;

		codes [ huffmanCode ] = asciiCharacter;

	}
	
	string compressedData;
	inFile >> compressedData;

	translateCompressedData ( fileName , compressedData , codes );

}

void translateCompressedData ( string fileName , string compressedData , map < string , char > codes ) {

	//Pre:   The file name, the map of string and char, as well as the entire compressedData string 
	//	 is sent in via parameter.
	//Post:  The compressedData string is parsed through.  For each letter in the string, a new string
	//	 is constructed in order to build each individual character's compressed binary code.  Each
	//	 time a new binary number is added to the small string, this string is passed in a find function
	//	 to see if this string corresponds to one in the map.  If it does, the small string is reset,
	//	 and this character from the map is written to the file, overwriting the previous information.


	ofstream outFile ( fileName.c_str ( ) );

	string currDataToTranslate;

	for ( int currPosition = 0 ; currPosition < compressedData.length ( ) ; currPosition++ ) {

		currDataToTranslate += compressedData [ currPosition ];

		if ( codes.find ( currDataToTranslate ) != codes.end ( ) ) {		
			
			outFile << codes [ currDataToTranslate ];
			currDataToTranslate = "";

		}

	}
		
}

void renameFile ( string fileName ) {

	//Pre:   The file name is passed in via parameter.
	//Post:  The file name is renamed to the same file name without the ".zip" substring since the file
	//	 has been unzipped.  A message is printed to the user that the file has successfully been 
	//	 uncompressed.

	string newFileName = fileName;
	int zipBeginningLocation = fileName.find ( ".zip" );
	newFileName.replace ( zipBeginningLocation , 4 , "" );

	rename ( fileName.c_str() , newFileName.c_str() );

	cout << "File Successfully Inflated Back To Original." << endl;

}
