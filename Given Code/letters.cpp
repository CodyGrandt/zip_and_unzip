
#include <iostream>
#include <cctype>

using namespace std;

void CountLetters (int Letters[]);
void PrintLetters (const int Letters[]);

const int NumLetters = 256;

int main()
{
	int Letters[NumLetters]; 

	CountLetters (Letters);
	PrintLetters (Letters);
	return 0;
}

void CountLetters (int Letters[])
{
	char ch;

	for (char ch = char(0);  ch <= char(126);  ch++)
		Letters[ch] = 0;

	cin.get(ch);
	while (cin)
	{
		Letters[ch] += 1;
		cin.get(ch);
	}
}


void PrintLetters (const int Letters[])
{
	for (char ch = char(0);  ch <= char(126);  ch++)
	  if ( (Letters[ch] != 0) && (ch != '\n') && (ch != ' ') )
	    cout << "Char " << ch << " appearances  : " << Letters[ch] << endl;
	  else if ( (Letters[ch] != 0) && (ch == '\n') )
	    cout << "Char nl appearances : " << Letters[ch] << endl;
	  else if ( (Letters[ch] != 0) && (ch == ' ') )
	    cout << "Char sp appearances : " << Letters[ch] << endl;
}
