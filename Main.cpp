#include"Huffman.h"

// Used in the debugging process to sort the dictionary
bool DictComp(pair<char, vector<bool>> v1, pair<char, vector<bool>> v2)
{
	return (v1.second.size() < v2.second.size());
}


/* This is the optimized and corrected version of my previous Huffman 
 encoding project, which contained a number of inefficiancies and
 design errors as a result of rushed execution.*/
/// <summary>
/// Comperesses a file using Huffman encoding.
/// Huffman encoding is a method of binary encoding that changes the
/// code used to represent characters such that the characters that
/// appear often are represented using shorter codes, and the ones
/// that appear unoften are represented using longer codes. 
/// It does so by creating a type of binary tree, called a Huffman tree,
/// in which the characters are the leaves and auxilliary nodes are created
/// for the non leaf nodes. The "path" from the head to the character
/// determines its code: left child adds 1, right child adds 0.
/// 
/// There are 2 objects used:
/// Huffman node - A node in the Huffman Tree containing a character, the index
/// of its left and right children, and the number of appearances.
/// Auxilliary nodes have a character of 0 and a number of appearances equal to
/// the sum of the appearances of their children.
/// Leaves have a character between 0-255 and their left and right children
/// are equal to 0.
///
/// Huffman Tree - The Huffman tree, containing the tree length
/// and the tree itself represented using a dynamic array of Huffman
/// nodes, as pointers cannot be properly writen into a file. 
/// 
/// The program reads the file and creates a dictionary of characters and
/// their appearances.
/// It then creates a vector (Edit: changing the vector?) of character and 
/// num appearances pairs, and sorts it by decending order of number of 
/// appearances. It then creates the Huffman tree from the vector.
/// The Huffman tree is good for decoding, but is inefficient to encode
/// with. Therefore a dictionary of character and its new code is created.
/// The Huffman tree is written into the file first to enable the file to
/// be decoded, and then the file is encoded according to the dictionary 
/// and written into the file.
/// Edit: file ending explination
///	Edit: Encoding non text
/// 
/// The decompressor opens the file, reads the tree, creates a new instance of
/// a Huffman tree that matches it, and uses it to decode the message.
/// Functions and objects appear in Huffman.h
/// </summary>
/// <returns>The exit code</returns>
int main() {
	// TODO:
	/*
	1. Make compression 1 function - Done.
	2. Orginize the code - Done.
	3. Get rid of obvious waste:
	- Turn both dictionaries into lookup tables.
	- Use object references instead of values.
	- Look at the insert function in the tree.
	- Look at function inlining.
	- Look at the loops for writing and reading from file.
	- Vector pushback O(n)
	4. Change the eof method?
	5. Look at malloc - Changed to new[].
	*/
	// Testing area

	// Variable definitions
	map<char, int> CharAppearances;
	vector<HuffmanNode> MessageEnsamble;
	string Source = "E:\\Visual Studio projects\\HuffmanEfficient\\alice.txt";
	string Storage = "E:\\Visual Studio projects\\HuffmanEfficient\\temp.DAT";
	string New = "E:\\Visual Studio projects\\HuffmanEfficient\\Decoded.txt";
	CompressFile(Source, Storage);
	DecompressFile(Storage, New);
	// Check if tree is correct
	cout << "Done";
	return 0;
}
