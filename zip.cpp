#include <fstream>
#include <iostream>
#include "huffman.h"

using namespace std;

void displayTable ( int argumentCount , HuffmanTree tree );
void compressFile ( char* argumentArray [ ] , HuffmanTree tree );
void constructHuffmanTree ( char* argumentArray [ ] , HuffmanTree & tree );
bool decipherCommandLine ( int argumentCount , char* argumentArray [ ] );
bool decipherOneInput ( char* argumentArray [ ] );
bool decipherTwoInputs ( char* argumentArray [ ] );
string getFilename ( char* argumentArray [ ] );
void compressionRatio ( HuffmanTree tree );
void printHelpInformation ( );
void printInvalidFormat ( );
void renameFile ( char* argumentArray [ ] );

int main ( int argumentCount , char* argumentArray [ ] ) {

	bool isCommandLineValid = decipherCommandLine ( argumentCount , argumentArray );

	if ( ! isCommandLineValid )
		return 1;

	HuffmanTree tree;

	constructHuffmanTree ( argumentArray , tree );
	compressFile ( argumentArray , tree );
	displayTable ( argumentCount , tree );
	compressionRatio ( tree );	

return 0;
}

void displayTable ( int argumentCount , HuffmanTree tree ) {

	//Pre:   The argument count has been passed in via parameter.
	//Post:  If this method is being called, that means all conditions
	//	 have been met and the arguments were valid, therefore if
	//	 argumentCount = 3, that means the user requested --t
	
	if ( argumentCount == 3 )
		tree.PrintTable();
	
}

void compressFile ( char* argumentArray [ ] , HuffmanTree tree ) {

	//Pre:   The argument array and huffman tree are passed in
	//	 via parameter.
	//Post:  The file has been successfully overwriten with the
	// 	 compressed data and .zip extension has been added
	// 	 to the file name.



	string filename = getFilename ( argumentArray );
	
	ifstream inFile ( filename.c_str() );

	string compressedData;

	while ( inFile ) {
	
		char currLetter = inFile.get();
		compressedData += tree.GetCode ( currLetter );

	}

	ofstream outFile ( filename.c_str() );

	outFile <<  ( tree.numNodes() / 2 ) + 1 << endl;
	
	for ( char ch = char ( 1 ) ; ch <= char ( 126 ) ; ch++ ) {

		if ( tree.inTree ( ch ) )
			outFile << ( ( int ) ch ) << " " << tree.GetCode ( ch ) << endl;

	}
	
	outFile << compressedData;		
	renameFile ( argumentArray );

}

void constructHuffmanTree ( char* argumentArray [ ] , HuffmanTree & tree ) {

	//Pre:   the argumentArray as well as the huffman tree are passed in 
	//	 via parameter.
	//Post:  The huffman tree is successfully built using the data from the
	//	 inputted file.


	string filename = getFilename ( argumentArray );
	
	ifstream inFile ( filename.c_str() );

	char ch;
	int characters [ 256 ];

	for ( char ch = char ( 0 ) ; ch <= char ( 126 ) ; ch++ )
		characters [ ch ] = 0;

	inFile.get ( ch );
	while ( inFile ) {

		characters [ ch ] += 1;
		inFile.get ( ch );

	}
	
	for ( char ch = char ( 0 ) ; ch <= char ( 126 ) ; ch++ ) 
		if ( characters [ ch ] != 0 ) 
			tree.insert ( ch , characters [ ch ] );

	tree.build();
} 

bool decipherCommandLine ( int argumentCount , char* argumentArray [ ] ) {
	
	//Pre:   The argumentCount representing the amount of arguments entered
	//	 from the command line as well as the array that holds these
	//	 arguments is passed in via parameter. 
	//Post:  This program can only have one or two additional arguments
	//	 entered from the command line.  If there is one additional
	//	 argument, then the method to handle one argument is called.
	//	 If there are two arguments, then the method to handle two
	//	 arguments is called.  Any other cases of argumentCount will
	//	 print a message to the user, cause this method to return a 
	//	 false bool so the main method can easily exit the program.
	//Note:  calling ./a.out counts as an argument in the command line, so
	//	 one or two additional arguments are represented as 2 or 3 in
	//	 the argumentCount.

	bool isValid;

	switch ( argumentCount ) {

		case 2 :	isValid = decipherOneInput ( argumentArray );
				if ( ! isValid )
				   cout << "ZIP:  file " << argumentArray[1] << " does not exist." << endl;
				return isValid;
				break;
		
		case 3 : 	return decipherTwoInputs ( argumentArray );
				break;
	
		default : 	printInvalidFormat ( );
				return false;
				break;

	}

}

bool decipherTwoInputs ( char* argumentArray [ ] ) {

	//Pre:   The array holding the arguments from the command line is 
	//	 passed in via parameter.
  	//Post:  If the first of the two arguments is anything other than
  	//	 the --t command, the method returns a false bool meaning the 
  	//	 command line input is invalid.  If the first command is --t,
  	//	 then the second one is checked to see if it is a file that 
  	//	 exists, and returns a corresponding bool.

	string firstInput = argumentArray [ 1 ];

	if ( firstInput != "--t" ) {
		
		printInvalidFormat ( ); 
		return false;
	
	}
	else { 
		string secondInput = argumentArray [ 2 ];
		ifstream file ( secondInput.c_str() );

		if ( file )
			return true;
	
		else { 
	
			cout << "ZIP:  file " << secondInput << " does not exist." << endl;
			return false;

		}		
	}
}

bool decipherOneInput ( char* argumentArray [ ] ) {

	//Pre:   The array holding the arguments from the command line is
	//	 passed in via parameter.
	//Post:  If the additional argument is --help, then the correct 
	//	 information is printed to the user, and a true bool meaning
	//	 this was a valid command line input is returned.  If anything
	//	 other than --help is entered, then the program attempts to 
	//	 begin writing to this file.  If the program can successfully
	//	 write to this file, this means the file is found and a true
	//	 bool is returned, otherwise a message is printed and a false 
	//	 bool is returned.




	string input = argumentArray [ 1 ];

	if ( input == "--help" ) {

		printHelpInformation ( ); 
	}
	else { 

		ifstream file ( input.c_str() );

		if ( file ) {

			return true;
		}

		else {

			return false;

		}	

	}
}

void compressionRatio ( HuffmanTree tree ) {

	int numBits = tree.numBits();
	int numChars = ( tree.numNodes() / 2 ) - 1;

	double compressionRatio = ( 1 - ( ( numBits ) / ( numChars * 8 ) ) );

	int compressedBits = numBits * compressionRatio;

	cout << "File Successfully Compressed To " << numBits << " Bits (" << compressionRatio << "% less)." << endl;



}
 
string getFilename ( char* argumentArray [ ] ) {

	//Pre:   The array holding the arguments from the command line is 
	//	 passed in via parameter.
	//Post:  This method will be called after the command line has been
	//	 established as a valid command, therefore either 
	//	 the method decipherOneInput or decipherTwoInputs has to be true.
	//	 if decipherOneInput is true, then the filename must be the second
	//	 argument in the array, otherwise it must be the third.  this method
	//	 gets the correct location of the filename and returns it.

	string filename;

	if ( decipherOneInput ( argumentArray ) )
		filename = argumentArray [ 1 ];
	else
		filename = argumentArray [ 2 ];
	
	return filename;

}

void printHelpInformation ( ) {

	//Post:  Information to help the user when the user enters the 
	//	 command --help is printed.

	cout << "Usage: ZIP [OPTION]... [FILE]..." << endl;
	cout << "Compress a text file using Huffman Encoding." << endl;
	cout << "\n  --t              Display the Huffman encoding table." << endl;
	cout << "  --help           Provide help on command." << endl;

}

void printInvalidFormat ( ) {

	//Post:  Information to help the user when they enter an incorrect
	//	 command line argument is printed.

	cout << "ZIP: Invalid format" << endl;
	cout << "Try 'ZIP --help' for more information" << endl;

}

void renameFile ( char* argumentArray [ ] ) {

	//Pre:   The argumentArray is passed in via parameter with the goal if 
	//	 retrieving the file name with it.
	//Post:  The file name is renamed to the same file name with a ".zip" 
	//	 extension since the file has been zipped.  

	string currFilename = getFilename ( argumentArray );
	string newFilename = currFilename + ".zip";
	
	rename ( currFilename.c_str() , newFilename.c_str() );

}
