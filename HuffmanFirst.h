#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<map>
#include<vector>
#include<bitset>
#include<string>
#include<fstream>
#include<iostream>
#include<algorithm>
using namespace std;

// A node in the Huffman tree
class HuffmanNode {
private:
	int LeftChild;
	int RightChild;
	char Character;
	int NumAppearances;

public:
	// Constructors:
	HuffmanNode() {}
	// General- Probably won't be used.
	HuffmanNode(int LeftChild, int RightChild, char Character, int NumAppearances)
	{
		this->LeftChild = LeftChild;
		this->RightChild = RightChild;
		this->Character = Character;
		this->NumAppearances = NumAppearances;
	}
	// For auxiliary nodes with no character
	HuffmanNode(int LeftChild, int RightChild, int NumAppearances)
	{
		this->LeftChild = LeftChild;
		this->RightChild = RightChild;
		this->Character = 0;
		this->NumAppearances = NumAppearances;
	}
	// For character nodes with no children
	HuffmanNode(char Character, int NumAppearances)
	{
		this->LeftChild = 0;
		this->RightChild = 0;
		this->Character = Character;
		this->NumAppearances = NumAppearances;
	}
	// Destructor
	~HuffmanNode(){}

	// Getters:
	char	GetCharacter()		{ return this->Character; };
	int		GetLeft()			{ return this->LeftChild; };
	int		GetRight()			{ return this->RightChild; };
	int		GetNumAppearances() { return this->NumAppearances; };
};

// Function prototypes
void SortVector(vector<HuffmanNode> &MessageEnsamble);



class HuffmanTree {
private:
	int Length;
	// A dynamic array is used to represent the tree
	HuffmanNode * Tree;

public:
	// Edit: Memory allocation is oldschool c, may need to be changed.
	HuffmanTree(){}
	// This constructor is for the reading
	HuffmanTree(int Length) {
		this->Length = Length;
		this->Tree = (HuffmanNode *)malloc(this->Length * sizeof(HuffmanNode));
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="HuffmanTree"/> class,
	/// with a full tree.
	/// </summary>
	/// <param name="MessageEnsamble">The message ensamble, sorted
	/// in decending order of number of appearances.</param>
	/// <param name="Length">The length.</param>
	HuffmanTree(vector<HuffmanNode> MessageEnsamble)
	{
		// The node to symbolize the end of the file
		HuffmanNode EndFile(0, 0, 0);
		MessageEnsamble.push_back(EndFile);
		// Basic constructor
		this->Length = MessageEnsamble.size() * 2 - 1;
		this->Tree = (HuffmanNode *)malloc(this->Length * sizeof(HuffmanNode));
		// Index for the following for loop, is called once after loop end.
		int index;
		// Fills tree from end to beggining
		for (index = Length - 1; index > 0; index-=2)
		{
			// Placing the nodes in the tree
			this->Tree[index] = MessageEnsamble.back();
			MessageEnsamble.pop_back();
			this->Tree[index - 1] = MessageEnsamble.back();
			// Turning the vector into the next auxilliary message ensemble 
			MessageEnsamble[MessageEnsamble.size() - 1] =
				HuffmanNode(index, index - 1, 
					// The sum of the appearances of its children
					this->Tree[index].GetNumAppearances() + 
					this->Tree[index - 1].GetNumAppearances());
			SortVector(MessageEnsamble);
		}
		// Adding the head of the tree if it was missed by the loop
		if (index == 0)
		{
			this->Tree[0] = MessageEnsamble[0];
		}
	}

	// Sorts the vector that only has the last element out of place, 
	// by the number of appearnces.
	

	int GetLength() { return this->Length; }
	HuffmanNode * GetTree(){ return this->Tree; }
	// Inserts a node into the tree at the appropriate index.
	void InsertNode(HuffmanNode node, int index)
	{
		this->Tree[index] = node;
	}
};

// Sorts a Huffman node vector by the number of appearances 
// Given that only the last element is out of place
void SortVector(vector<HuffmanNode> &MessageEnsamble)
{
	// Variable definitions
	HuffmanNode TempNode;

	// Code section
	// Goes over the vector from the end, swapping where appropriate
	// Basically bubble sort but when only the last element isn't sorted.
	for (int index = MessageEnsamble.size() - 1;
		index > 0 && MessageEnsamble[index].GetNumAppearances() >
		MessageEnsamble[index - 1].GetNumAppearances();
		index--)
	{
		// Swaps the nodes
		TempNode = MessageEnsamble[index - 1];
		MessageEnsamble[index - 1] = MessageEnsamble[index];
		MessageEnsamble[index] = TempNode;
	}

}

/// <summary>
/// Creates a map of the appearanceses of all the characters 
/// in the file specified by the FilePath.
///	Edit: int may not be enough for number of appearances of everything
/// </summary>
/// <param name="FilePath">The FilePath.</param>
/// <returns>A map of character, number of appeanance</returns>
map<char, int> MapAppearances(string FilePath)
{
	// Variable definitions
	map<char, int> Appearances;
	char letter;
	ifstream testFile;

	// Code section
	// Opens file
	testFile.open(FilePath);
	// If error in opening
	if (!testFile.is_open()) {
		cerr << "An error opening file";
		exit(1);
	}
	// Reads file 1 character at a time, counts appearances
	while (!testFile.eof())
	{
		testFile.get(letter);
		Appearances[letter]++;
	}
	// Closes file
	testFile.close();
	// Return the map
	return(Appearances);
}


/// <summary>
/// Compresses the source file, writes compressed version into the 
/// destination file.
/// </summary>
/// <param name="SourceFile">The source file.</param>
/// <param name="DestinationFile">The destination file.</param>
/// <param name="Tree">The Huffman tree.</param>
/// <param name="Dictionary">The encoding dictionary.</param>
/// <param name="eofCode">The vector to contain the code for end of file.</param>
void CompressFile(string SourceFile, string DestinationFile, HuffmanTree Tree, 
	map<char, vector<bool>> Dictionary, vector<bool> eofCode) 
{
	// Variable definitions
	ifstream Source;
	ofstream Dest;
	vector<bool> Code;
	char ReadBuffer;
	// Buffer for writing integers that represent the tree
	int TreeWriteBuffer;
	char WriteBuffer = 0;
	char Counter = 0;
	int	 VectorIndex = 0;
	bool KeepRunning = true;
	bool ReachedEnd = false;

	// Code section
	// Opening files + checking for errors in opening
	Source.open(SourceFile);
	if (!Source)
	{
		cerr << "An error occured in opening file " << SourceFile;
		cerr << " in Write To File";
		exit(1);
	}
	Dest.open(DestinationFile, ios::binary|ios::out);
	if (!Dest)
	{
		cerr << "An error occured in opening file " << DestinationFile;
		cerr << " in Write To File";
		exit(1);
	}
	// Writing the HuffmanTree, legth first to ease reading.
	TreeWriteBuffer = Tree.GetLength();
	Dest.write(reinterpret_cast<char*>(&TreeWriteBuffer), sizeof(int));
	for (int i = 0; i < Tree.GetLength(); i++)
	{
		// Debugging print
		if (i<10)
		{
			cout << '0';
		}
		cout << i << " - ";
		cout << "L-" << Tree.GetTree()[i].GetLeft();
		cout << "\tR-" << Tree.GetTree()[i].GetRight();
		cout << "\tC-" << Tree.GetTree()[i].GetCharacter();
		cout << "\tN-" << Tree.GetTree()[i].GetNumAppearances() << endl;
		if (Source.eof())
		{
			cout << "Error";
			break;
		}

		// Write left
		TreeWriteBuffer = Tree.GetTree()[i].GetLeft();
		Dest.write(reinterpret_cast<char*>(&TreeWriteBuffer), sizeof(int));
		// Write right
		TreeWriteBuffer = Tree.GetTree()[i].GetRight();
		Dest.write(reinterpret_cast<char*>(&TreeWriteBuffer), sizeof(int));
		// Write the character
		WriteBuffer = Tree.GetTree()[i].GetCharacter();
		Dest.write(&WriteBuffer, sizeof(char));
		// Write the number of appearances
		TreeWriteBuffer = Tree.GetTree()[i].GetNumAppearances();
		Dest.write(reinterpret_cast<char*>(&TreeWriteBuffer), sizeof(int));
	}

	Source >> ReadBuffer;
	Code = Dictionary[ReadBuffer];
	// While there is still data to encode this loop keeps reading from Source
	// encoding the data, writing it to a buffer, than when the buffer is full 
	// writing it to the file.
	while (KeepRunning)
	{
		// Fills the buffer with the Code, writing every 8 bits
		// or when end is reached.
		for ( Counter |= 0xff; Counter; Counter <<= 1)
		{
			// Loop condition explination:
			// Counter is 0 before each start so |=0xff makes it 0xff. 
			// (1111,1111). The shifting of the bits matches the shifting in the 
			// writing buffer and is slightly more efficiant than a counter.
			// It has a bit shifted out every time a bit is shifted into the 
			// write buffer.  
			// When all the bits have been shifted out of the counter,
			// the write buffer is full.

			// Reached end of vector.
			if (VectorIndex == Code.size())
			{
				// Zeroing more efficiently
				VectorIndex ^= VectorIndex;
				// Reads from file, breaks when file ends
				Source.get(ReadBuffer);
				//  Reached file end, write file ending
				if (Source.eof())
				{
					// If we already wrote the eof code
					if (ReachedEnd)
					{
						// Fill the buffer with eof properly
						for (; Counter; Counter <<= 1)
						{
							WriteBuffer <<= 1;
						}
						KeepRunning = false;
						break;
					}
					// If we didn't write eofCode yet
					Code = eofCode;
					ReachedEnd = true;
				}
				// Not at file end
				else
				{
					Code = Dictionary[ReadBuffer];
				}
			}
			// Shift the buffer one to the left
			WriteBuffer <<= 1;
			// If true turn on last bit, else keep it off.
			// Increment index.
			WriteBuffer |= (Code[VectorIndex++]) ? 1 : 0;
		}
		// Write the buffer
		Dest.write(&WriteBuffer, sizeof(char));
		// Zeroes the write buffer slightly more efficiently
		WriteBuffer ^= WriteBuffer;
	}
	

	// closing files.
	Source.close();
	Dest.close();
	cout << "Done writing";
}


/// <summary>
/// Decompresses the file, and writes the decompressed version into the
/// destination file.
/// </summary>
/// <param name="SourceFile">The source file.</param>
/// <param name="DestinationFile">The destination file.</param>
void DecompressFile(string SourceFile, string DestinationFile)
{
	// Variable definitions
	ifstream Source;
	ofstream Dest;
	// Index for seaching the tree
	int Index = 0;
	char Counter = 0;
	// For reading into from the file
	int ReadBuffer;
	bool KeepRunning = true;
	// Tree variables
	HuffmanTree Tree;
	int TreeLength;
	// Node variables
	HuffmanNode TempNode;
	int LeftChild;
	int RightChild;
	char Character;
	int NumAppearances;


	// Code section
	// Opening files and checking for errors
	Source.open(SourceFile, ios::binary|ios::in);
	if (!Source)
	{
		cerr << "An error occured in opening file " << SourceFile;
		cerr << " in Decompress File";
		exit(1);
	}
	Dest.open(DestinationFile);
	if (!Dest)
	{
		cerr << "An error occured in opening file " << DestinationFile;
		cerr << " in Decompress File";
		exit(1);
	}

	// Reding the tree length
	Source.read(reinterpret_cast<char*>(&TreeLength), sizeof(int));
	Tree = HuffmanTree(TreeLength);
	cout << endl;
	// Reading the tree
	for (int i = 0; i < TreeLength; i++)
	{
		
		// Reading the parameters for each node
		// Read left child
		Source.read(reinterpret_cast<char*>(&LeftChild), sizeof(int));
		// Read right child
		Source.read(reinterpret_cast<char*>(&RightChild), sizeof(int));
		// Read character
		Character ^= Character;
		
		Source.read(&Character, sizeof(char));
		

		// Read number of appearances
		Source.read(reinterpret_cast<char*>(&NumAppearances), sizeof(int));
		// Adding the new node to the tree
		TempNode = HuffmanNode(LeftChild, RightChild, Character, NumAppearances);
		Tree.InsertNode(TempNode, i);
		// Debugging print
		if (i<10)
		{
			cout << '0';
		}
		cout << i << " - ";
		cout <<"L-"<<LeftChild << "\tR-" << RightChild << "\tC-" << Character;
		cout << "\tN-" << NumAppearances << endl;
		if (Source.eof())
		{
			cout << "Error";
			break;
		}
	}
	string test;
	
	// Reading the file and decoding it
	int nCharWritten = 0;
	while (KeepRunning)
	{
		Source.read(&Character, sizeof(char));
		for (Counter |= 0xff; Counter; Counter <<= 1)
		{
			// Reached character
			if (!Tree.GetTree()[Index].GetLeft())
			{
				// End of file character with 0 appearances
				if (!Tree.GetTree()[Index].GetNumAppearances())
				{
					// End the decompression
					KeepRunning = false;
					break;
				}

				// Write character
				Dest << Tree.GetTree()[Index].GetCharacter();
				// Debugging from here
				test.push_back(Tree.GetTree()[Index].GetCharacter());
				nCharWritten++;
				if (nCharWritten -2== Tree.GetTree()[0].GetNumAppearances())
				{
					cout << "Breakpoint";
					//KeepRunning = false;
					//break;
				}// Untill here

				// Zeroing index
				Index ^= Index;
			}
			// If the leftomost bit is on in character
			if (Character & 0x80)
			{
				// Go to left child
				Index = Tree.GetTree()[Index].GetLeft();
			}
			// Leftmost bit is off
			else
			{
				// Go to right child
				Index = Tree.GetTree()[Index].GetRight();
			}
			Character <<= 1;
		}
	}

	// Closing files
	Source.close();
	Dest.close();

}