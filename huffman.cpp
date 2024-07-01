#include "huffman.h"
#include <iostream>
using namespace std;

HuffmanTree:: HuffmanTree()
	: numChars(0), built(false) {}

void HuffmanTree:: insert(char ch, int weight) {

   	HNode newNode = {ch, weight, -1, -1}; 
    	built = false;
   	nodes.push_back(newNode);
   	numChars++;

}

bool HuffmanTree:: inTree(char ch) {

	int location = lookUp ( ch );

	if ( location == -1 )
		return false;
	else
		return true;

}

int HuffmanTree:: GetFrequency(char ch) {

	int location = lookUp ( ch );

	if ( location == -1 )
		return 0;
	else
		return GetFrequency ( location );

}

int HuffmanTree:: GetFrequency(int i) {
	
	return nodes [ i ].weight;

}

int HuffmanTree:: lookUp(char ch) {

	for ( int count = 0 ; count < numChars ; count++ ) {

		if ( nodes [ count ].ch == ch ) 
			return count;

	}
	return -1;

}

string HuffmanTree:: GetCode(char ch) {

	return GetCode ( lookUp ( ch ) );
	
}


string HuffmanTree:: GetCode(int i) {

	if ( nodes [ i ].parent == 0 )
		return "";
	else
		return ( GetCode ( nodes [ i ].parent ) + 
		 ( char ) ( nodes [ i ].childType + '0' ) );


}

void HuffmanTree:: PrintTable() {

	cout << "\n\t## ENCODING TABLE FOR ZIP FILE ##\n" << endl;
	cout << "\tIndex   Char    Weight  Parent  ChildType\n" << endl;
	
	int step = 1;
	for ( int count = 0 ; count < numNodes() ; count++ ) {
		
		cout << "\t" << count;
		
		if ( nodes [ count ].ch == '\0' ) {
			cout << "\tT" << step;
			step++;
		}
		else if ( nodes [ count ].ch == '\n' )
			cout << "\tnl";
		else if ( nodes [ count ].ch == ' ' )
			cout << "\tsp";
		else 
			cout << "\t" << nodes [ count ].ch;
		
	
		cout << "\t" << GetFrequency ( count );
		cout << "\t" << nodes [ count ].parent;
		
		if ( nodes [ count ].childType == -1 ) {
			cout << "\tN/A\n";
		}
		else {
			cout << "\t" << nodes [ count ].childType << "\n";
		}

	}
		
	cout << "\n" << endl;
	
}

int HuffmanTree:: numNodes() {

	return numChars;

}

int HuffmanTree:: numBits() {

	int maxParent = numChars - 1 ;
	int minParent = maxParent;

	for ( int i = 0 ; i < numChars - 1  ; i++ ) 
		if ( nodes [ i ].parent < minParent )
			minParent = nodes [ i ].parent;

	int height = maxParent - minParent;
	int numBits = 0;

	for ( int i = minParent ; i < maxParent ; i++ ) { 
		
		for ( int j = 0 ; j < numChars - 1 ; j++ ) {

			if ( nodes [ j ].parent == i ) {

				numBits += height * ( nodes [ j ].weight );

			}

		}
		height--;

	}
	return numBits;

}

int HuffmanTree:: getSmallestFreq ( vector < HNode >& temp , int size ) {

	int smallestLoc = 0;
	while ( temp [ smallestLoc ].weight == -1 )
		smallestLoc++;

	for ( int count = 0 ; count < size ; count++ ) 
		if ( ( temp [ count ].weight < temp [ smallestLoc ].weight ) && (temp [ count ].weight != -1 ) )
			smallestLoc = count;

	int smallestFreq = temp [ smallestLoc ].weight;
	temp [ smallestLoc ].weight = -1; 

	return smallestFreq;	

}

int HuffmanTree:: getSmallestLoc ( vector < HNode >& temp , int size ) {

	int smallestLoc = 0;
	while ( temp [ smallestLoc ].weight == -1 )
		smallestLoc++;

	for ( int count = 0 ; count < size ; count++ )
		if ( ( temp [ count ].weight < temp [ smallestLoc ].weight ) && ( temp [ count ].weight != -1 ) )
			smallestLoc = count;
	
	temp [ smallestLoc ].weight = -1;
	return smallestLoc;

}


void HuffmanTree:: build() {

	vector < HNode > temp = nodes;

	int startingSize = numNodes();	
	int smallestLoc = getSmallestLoc ( temp , startingSize );
	int secondSmallestLoc = getSmallestLoc ( temp , startingSize );
	int previousTotal = nodes [ smallestLoc ].weight + nodes [ secondSmallestLoc ].weight;

	insert ( '\0' , previousTotal );
	
	nodes [ numChars - 1 ].parent = numChars;	
	nodes [ smallestLoc ].parent = startingSize;
	nodes [ smallestLoc ].childType = 1;
	nodes [ secondSmallestLoc ].parent = startingSize;
	nodes [ secondSmallestLoc ].childType = 0;
	
	for ( int count = 0 ; count < startingSize - 6 ; count++ ) {
	
		int currSmallestLoc = getSmallestLoc ( temp , startingSize );
		
		nodes [ currSmallestLoc ].childType = 1;
		
		insert ( '\0' , ( nodes [ currSmallestLoc ].weight + previousTotal ) );
		
		nodes [ startingSize + count ].childType = 0;
		nodes [ currSmallestLoc ].parent = numChars - 1;
		nodes [ numChars - 1 ].parent = numChars;
		
		previousTotal = nodes [ currSmallestLoc ].weight + previousTotal;

	}
	
	nodes [ numChars - 1 ].childType = 0;

	int currSmallestLoc = getSmallestLoc ( temp , startingSize );
	
	nodes [ currSmallestLoc ].childType = 1;
	
	insert ( '\0' , ( nodes [ currSmallestLoc ].weight + previousTotal ) );

	nodes [ numChars - 1 ].parent = numChars + 1;
	nodes [ currSmallestLoc ].parent = numChars - 1;
	nodes [ numChars - 1 ].childType = 0;	

	previousTotal = nodes [ currSmallestLoc ].weight + previousTotal;

	int otherSmallestLoc = getSmallestLoc ( temp , startingSize );
	int otherSecondSmallestLoc = getSmallestLoc ( temp , startingSize );
	int otherTotal = ( nodes [ otherSmallestLoc ].weight ) + ( nodes [ otherSecondSmallestLoc ].weight );
	
	nodes [ otherSmallestLoc ].childType = 1;
	nodes [ otherSecondSmallestLoc ].childType = 0;
	
	insert ( '\0' , otherTotal );
	
	nodes [ otherSmallestLoc ].parent = numChars - 1;
	nodes [ otherSecondSmallestLoc ].parent = numChars - 1;
	nodes [ numChars - 1 ].childType = 0;

	int finalSmallestLoc = getSmallestLoc ( temp , startingSize );
	insert ( '\0' , ( previousTotal + nodes [ finalSmallestLoc ].weight ) );
 	nodes [ finalSmallestLoc ].parent = numChars - 1;
	nodes [ finalSmallestLoc ].childType = 1;
	insert ( '\0' , ( previousTotal + nodes [ finalSmallestLoc ].weight + otherTotal ) );
	
	nodes [ numChars - 1 ].parent = 0;
	nodes [ numChars - 2 ].parent = numChars - 1;
	nodes [ numChars - 2 ].childType = 0;
	nodes [ numChars - 2 ].childType = 1;
	nodes [ numChars - 3 ].parent = numChars - 1;	
	
}

	


