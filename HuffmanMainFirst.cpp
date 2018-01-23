
#include"Huffman.h"

// Used to sort the vector
bool HuffmanComp(HuffmanNode h1, HuffmanNode h2) 
{ 
	return (h1.GetNumAppearances() > h2.GetNumAppearances()); 
}

bool DictComp(pair<char, vector<bool>> v1, pair<char, vector<bool>> v2)
{
	return (v1.second.size() < v2.second.size());
}

map<char, vector<bool>> EncodingDictionary(HuffmanTree Tree, vector<bool> &eofCode);
void FillDictionary(HuffmanTree Tree, map<char, vector<bool>> &Dictionary,
	int index, vector<bool> code, vector<bool> &eofCode);

/// <summary>
/// Comperesses a file using Huffman encoding.
/// Functions appear in Huffman.h
/// TODO: 
/// 1. Count appearances - Done.
///	2. Make Tree - Done.
/// 3. Encode message - Done.
/// 4. Write tree and encoded message - Done.
/// 5. Figure out how to solve inefficiancy with last byte written.
/// 6. Decode message.
/// 7. Optimize code.
/// Edit list:
///	A list of the things that need to be changed.
///	1. Message ensemble O(n) insertion.
///	2. Map appearances: int may not be large enough to count the appearances.
/// 3. Writing the tree: the size of an integer is different per pc.
/// 4. Tree Constructor: malloc is oldschool c.
/// </summary>
/// <returns>The exit code</returns>
int main() {

	// Testing area

	// Variable definitions
	map<char, int> CharAppearances;
	vector<HuffmanNode> MessageEnsamble;
	string filename = "E:\\Visual Studio projects\\HuffmanEncoding2.0\\alice.txt";

	// Code section
	// Map of character and number of appearances
	CharAppearances = MapAppearances(filename);

	for (auto key : CharAppearances)
	{
		// cout << key.first << '-' << key.second << endl;
		// Edit: inneficient way to construct vector, O(n) insertion
		MessageEnsamble.push_back(HuffmanNode(key.first, key.second));
	}

	sort(MessageEnsamble.begin(), MessageEnsamble.end(), HuffmanComp);
	HuffmanTree Tree = HuffmanTree(MessageEnsamble);
	
	vector<bool> eofCode;
	map<char, vector<bool>> Dictionary = EncodingDictionary(Tree, eofCode);
	
	CompressFile(filename, "E:\\Visual Studio projects\\HuffmanEncoding2.0\\temp2.DAT",
		Tree, Dictionary, eofCode);
	DecompressFile("E:\\Visual Studio projects\\HuffmanEncoding2.0\\temp2.DAT", 
		"E:\\Visual Studio projects\\HuffmanEncoding2.0\\Decoded2.txt");
	// Check if tree is correct
	cout << "Breakpoint";
	return 0;
}

/// <summary>
/// Gets the Huffman tree for the message, creates a dictionary 
/// of character and its new code by calling the fill tree function
/// </summary>
/// <param name="Tree">The tree.</param>
/// <param name="eofCode">The vector to contain the code for end of file, 
/// called by reference.</param>
/// <returns> The dictionary to preform the encoding </returns>
map<char, vector<bool>> EncodingDictionary(HuffmanTree Tree, vector<bool> &eofCode)
{
	// Variable definitions
	map<char, vector<bool>> Dictionary;
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
void FillDictionary(HuffmanTree Tree,
	map<char, vector<bool>> &Dictionary, int index, vector<bool> code, 
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
		Dictionary[Tree.GetTree()[index].GetCharacter()] = code;
		return;
	}
	// If we haven't reached a leaf keep going
	// Add a 0 for every left turn
	code.push_back(true);
	FillDictionary(Tree, Dictionary, Tree.GetTree()[index].GetLeft(), 
		code, eofCode);
	// Add a 1 for every right turn
	code.at(code.size() - 1) = false;
	FillDictionary(Tree, Dictionary, Tree.GetTree()[index].GetRight(), 
		code, eofCode);
}