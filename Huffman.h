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


// Object definitions
#pragma region Object definitions

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
	~HuffmanNode() { /*cout << "Node destroyed" << endl;*/ }

	// Getters:
	char	GetCharacter() { return this->Character; };
	int		GetLeft() { return this->LeftChild; };
	int		GetRight() { return this->RightChild; };
	int		GetNumAppearances() { return this->NumAppearances; };

	// Setters
	void SetCharacter(char ch) { this->Character = ch; }
	void SetLeft(int left) { this->LeftChild = left; }
	void SetRight(int right) { this->RightChild = right; }
	void SetNumAppearances(int num) { this->NumAppearances = num; }
};

void			SortVectorLastNode(vector<HuffmanNode> &MessageEnsamble);
// The tree object
class HuffmanTree {
private:
	int Length;
	// A dynamic array is used to represent the tree
	HuffmanNode * Tree;

public:
	HuffmanTree() {}
	// This constructor is for the reading
	HuffmanTree(int Length) {
		this->Length = Length;
		this->Tree = new HuffmanNode[this->Length];
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
		//this->Tree = (HuffmanNode *)malloc(this->Length * sizeof(HuffmanNode));
		this->Tree = new HuffmanNode[this->Length];

		// Index for the following for loop, is called once after loop end.
		int index;
		// Fills tree from end to beggining
		for (index = Length - 1; index > 0; index -= 2)
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
			SortVectorLastNode(MessageEnsamble);
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
	HuffmanNode * GetTree() { return this->Tree; }
	// Inserts a node into the tree at the appropriate index.
	void InsertNode(HuffmanNode node, int index)
	{
		this->Tree[index].SetRight(node.GetRight());
		this->Tree[index].SetLeft(node.GetLeft());
		this->Tree[index].SetCharacter(node.GetCharacter());
		this->Tree[index].SetNumAppearances(node.GetNumAppearances());
	}

	~HuffmanTree() {
	delete [] this->Tree;
	}
};
#pragma endregion

// Function prototypes
#pragma region Function prototypes

void			CompressFile(string &SourceFile,
	string &DestinationFile);
void			DecompressFile(string &SourceFile,
	string &DestinationFile);
bool			HuffmanComp(HuffmanNode &h1, HuffmanNode &h2);
void			SortVectorLastNode(vector<HuffmanNode> &MessageEnsamble);
void			WriteEncodingToFile(string &SourceFile, string &DestinationFile,
	HuffmanTree &Tree,
	vector<bool> *Dictionary,
	vector<bool> &eofCode);
map<char, int>	MapAppearances(string &FilePath);
vector<bool> * EncodingDictionary
(HuffmanTree &Tree, vector<bool> &eofCode);
void			FillDictionary(HuffmanTree &Tree,
	vector<bool> *Dictionary,
	int index, vector<bool> code,
	vector<bool> &eofCode);

#pragma endregion

// Functions
#pragma region Functions

/// <summary>
/// Creates a compressed version of the source file in the Destination file
/// </summary>
/// <param name="SourceFile">The source file.</param>
/// <param name="DestinationFile">The destination file.</param>
void CompressFile(string &SourceFile, string &DestinationFile)
{
	// Variable definitions
	map<char, int> AppearanceMap;
	vector<HuffmanNode> MessageEnsamble;

	// The code that signals that the file has ended
	vector<bool> eofCode;

	// Code section

	// Map of character and number of appearances
	AppearanceMap = MapAppearances(SourceFile);
	// Generating a vector of Huffman nodes to make the tree
	for (auto key : AppearanceMap)
	{
		// Edit: O(n) insertion
		MessageEnsamble.push_back(HuffmanNode(key.first, key.second));
	}
	// Sorting the vector in decending order of number of appearances
	sort(MessageEnsamble.begin(), MessageEnsamble.end(), HuffmanComp);
	// Constructing the tree
	HuffmanTree Tree(MessageEnsamble);
	// Creates the dictionary used to encode the message
	vector<bool>* Dictionary = EncodingDictionary(Tree, eofCode);
	// Writes the message encoded into the file
	WriteEncodingToFile(SourceFile, DestinationFile,
		Tree, Dictionary, eofCode);
	// Freeing memory
	delete[] Dictionary;
	return;
}

/// <summary>
/// Decompresses the file, and writes the decompressed version into the
/// destination file.
/// </summary>
/// <param name="SourceFile">The source file.</param>
/// <param name="DestinationFile">The destination file.</param>
void DecompressFile(string &SourceFile, string &DestinationFile)
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

	int TreeLength;
	// Node variables
	int LeftChild;
	int RightChild;
	char Character;
	int NumAppearances;

	// Code section
	// Opening files and checking for errors
	Source.open(SourceFile, ios::binary | ios::in);
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
	HuffmanTree Tree(TreeLength);

	// Reading the tree
	for (int i = 0; i < TreeLength; ++i)
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

		// Edit: use 1 integer (read buffer) and 1 character
		Tree.GetTree()[i].SetCharacter(Character);
		Tree.GetTree()[i].SetLeft(LeftChild);
		Tree.GetTree()[i].SetRight(RightChild);
		Tree.GetTree()[i].SetNumAppearances(NumAppearances);
	}

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
	return;
}

/// <summary>
/// Comparison function used to tell the sort function what to sort
/// the Huffman nodes by.
/// </summary>
/// <param name="h1">The first Huffman node.</param>
/// <param name="h2">The second Huffman node.</param>
/// <returns></returns>
bool HuffmanComp(HuffmanNode &h1, HuffmanNode &h2)
{
	return (h1.GetNumAppearances() > h2.GetNumAppearances());
}

/// <summary>
/// Sorts a vector of Huffman nodes in which only the last node 
/// is out of place. Sorted in decending order of number of 
/// appearances.
/// </summary>
/// <param name="MessageEnsamble">The vector of Huffman nodes
/// representing the current auxilliary message ensamble.</param>
void SortVectorLastNode(vector<HuffmanNode> &MessageEnsamble)
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
/// Gets the Huffman tree for the message, creates a dictionary 
/// of character and its new code by calling the fill tree function
/// </summary>
/// <param name="Tree">The tree.</param>
/// <param name="eofCode">The vector to contain the code for end of file, 
/// called by reference.</param>
/// <returns> The dictionary to preform the encoding </returns>
vector<bool> * EncodingDictionary(HuffmanTree &Tree, vector<bool> &eofCode)
{
	// Variable definitions
	vector<bool>* Dictionary = new vector<bool>[256];
	vector<bool> code;

	// Code section

	FillDictionary(Tree, Dictionary, 0, code, eofCode);

	return Dictionary;
}

/// <summary>
/// Recursive - uses backtracking.
/// Fills the dictionary of characters and their new codes, 
/// from the Huffman tree. 
/// </summary>
/// <param name="Tree">The tree.</param>
/// <param name="Dictionary">The dictionary to fill.</param>
/// <param name="index">The index we are currently checking.</param>
/// <param name="code">The code we have accumulated for the character
/// based on the path.</param>
/// <param name="eofCode">The vector to contain the code for end of file, 
///called by reference.</param>
void FillDictionary(HuffmanTree &Tree,
	vector<bool> *Dictionary, int index, vector<bool> code,
	vector<bool> &eofCode)
{
	// Code section

	// The eof node is the only one with 0 appearances.
	if (!Tree.GetTree()[index].GetNumAppearances())
	{
		eofCode = code;
		return;
	}
	// If left is zero right is too, meaning we have reached a leaf
	if (!Tree.GetTree()[index].GetLeft())
	{
		// Chars can be negative so dictionary has 128 added to indexes
		Dictionary[Tree.GetTree()[index].GetCharacter() + 128] = code;
		return;
	}
	// If we haven't reached a leaf keep going
	// Add a 1 for every left turn
	code.push_back(true);
	// Error is here
	FillDictionary(Tree, Dictionary, Tree.GetTree()[index].GetLeft(),
		code, eofCode);
	// Add a 0 for every right turn
	code.at(code.size() - 1) = false;
	FillDictionary(Tree, Dictionary, Tree.GetTree()[index].GetRight(),
		code, eofCode);
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
void WriteEncodingToFile(string &SourceFile, string &DestinationFile,
	HuffmanTree &Tree, vector<bool> *Dictionary, vector<bool> &eofCode)
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
	// Edit: may be more efficient way to do this
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
	Dest.open(DestinationFile, ios::binary | ios::out);
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
	// Chars can be negative so dictionary has 128 added to indexes
	Code = Dictionary[ReadBuffer + 128];

	// While there is still data to encode this loop keeps reading from Source
	// encoding the data, writing it to a buffer, than when the buffer is full 
	// writing it to the file.
	while (KeepRunning)
	{
		// Writes the codes to the file, in 8 bit segments.
		// A more efficient way of counting to 8 by shifting out 8 "1"s.
		for (Counter |= 0xff; Counter; Counter <<= 1)
		{
			// Reached end of vector.
			if (VectorIndex == Code.size())
			{
				// Zeroing more efficiently
				VectorIndex ^= VectorIndex;
				// Reads a character from the file
				Source.get(ReadBuffer);
				// Reached file end, we need to write the eofCode, once.
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
					else
					{
						Code = eofCode;
						ReachedEnd = true;
					}
				}
				// Not at file end
				else
				{
					// Chars can be negative so dictionary has 128 added to indexes
					Code = Dictionary[ReadBuffer + 128];
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
/// Creates a map of the appearanceses of all the characters 
/// in the file specified by the FilePath.
///	Edit: int may not be enough for number of appearances of everything
/// </summary>
/// <param name="FilePath">The FilePath.</param>
/// <returns>A map of character, number of appeanance</returns>
map<char, int> MapAppearances(string &FilePath)
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

#pragma endregion
