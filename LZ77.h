#pragma once
#include <string>
#include <fstream>
#include <iostream>
// n - Ls
#define BUFFER_SIZE 32*1024
// Ls
#define LENGTH_OF_STRING 32
// n
#define WINDOW_SIZE BUFFER_SIZE + LENGTH_OF_STRING

// Function prototypes

void ByteCompress(std::string SourcePath, std::string DestinationPath);
void InitializeBuffer(char * Buffer);

void ByteDecompress(std::string SourcePath, std::string DestinationPath);

// Functions

/// <summary>
/// Compresses the specified source path.
/// </summary>
/// <param name="SourcePath">The source path.</param>
/// <param name="DestinationPath">The destination path.</param>
void ByteCompress(std::string SourcePath, std::string DestinationPath) 
{
#pragma region Variable definitions
	// The sliding window
	char *WindowStart = new char[WINDOW_SIZE];
	InitializeBuffer(WindowStart);

	// The string that hasn't been compressed yet
	char *StringStart = (WindowStart + BUFFER_SIZE);

	// The pointers to the current location being checked
	char *CurrentStringPointer = StringStart;
	char *BufferPointerStart = WindowStart;
	char *CurrentBufferPointer = BufferPointerStart;

	// The longest string
	char *ReproductionPointer = NULL;
	char MaxLength = 0;
	char CurrentLength = 0;
	unsigned short int JumpLength;

	// Used to figure out if to end loop
	int CharsRemaining = LENGTH_OF_STRING;
	
	// Source file
	std::ifstream Source;
	Source.open(SourcePath);
	if (!Source)
	{
		std::cerr << "An error occured in opening file " << SourcePath;
		std::cerr << " in Compress File" << std::endl;
		exit(1);
	}

	// Destination file
	std::ofstream Dest;
	Dest.open(DestinationPath, std::ios::out | std::ios::binary);
	if (!Dest)
	{
		std::cerr << "An error occured in opening file " << DestinationPath;
		std::cerr << " in Compress File" << std::endl;
		exit(1);
	}

	Source.seekg(0, Source.end);
	int SourceEndIndex = (int)Source.tellg();
	Source.seekg(0, Source.beg);
#pragma endregion

	// Code section
	
	Source.read(StringStart, LENGTH_OF_STRING * sizeof(char));
	int counter = 0;
	// Go over the file
	while (!Source.eof())
	{

		// Find longest string
		// String start is also the end of the buffer
		while (BufferPointerStart != StringStart)
		{
			// Find length of current string
			while (*CurrentBufferPointer == *CurrentStringPointer 
				&& CurrentLength < LENGTH_OF_STRING - 1 && 
				// Edit: change later as the algorithm should be able 
				// to deal with this
				CurrentLength < StringStart - BufferPointerStart)
			{
				CurrentLength++;
				CurrentBufferPointer++;
				CurrentStringPointer++;
			}
			// If string is longer
			if (CurrentLength > MaxLength)
			{
				MaxLength = CurrentLength;
				ReproductionPointer = BufferPointerStart;
				// Can't be a longer string
				if (MaxLength == LENGTH_OF_STRING - 1)
				{
					// Edit: break exists
					CurrentLength = 0;
					CurrentStringPointer = StringStart;
					break;
				}
			}
			// Reset Current String Variables
			CurrentStringPointer = StringStart;
			CurrentLength = 0;
			// Advance buffer pointer start
			BufferPointerStart++;
			CurrentBufferPointer = BufferPointerStart;
		}
		// Edit: change to remove if statement
		// Length of the jump back
		if (ReproductionPointer)
		{
			JumpLength = (int)(StringStart - ReproductionPointer);
			ReproductionPointer = NULL;
		}
		// Reproduction pointer == 0 
		else
		{
			JumpLength = 0;
		}
		
		// Writing to file

		// Write the length of the jump back
		Dest.write(reinterpret_cast<char*>(&JumpLength), sizeof(short));
		// Write the length of the string
		Dest.write(&MaxLength, sizeof(char));
		// Write next character
		Dest.write(StringStart + MaxLength, sizeof(char));

		// Progress window
		for (int i = 0; i < WINDOW_SIZE - MaxLength - 1; i++)
		{
			// Shifting out characters from buffer
			WindowStart[i] = WindowStart[i + MaxLength + 1];
		}
		
		if (counter == 23766 || counter == 23767)
		{
			std::cout << "";
		}
		// Shifting new characters in
		Source.read(StringStart + LENGTH_OF_STRING - (MaxLength + 1),
			(MaxLength + 1) * sizeof(char));
		counter++;
		if ((int) Source.tellg() == SourceEndIndex)
		{
			//CharsRemaining -= MaxLength + 1;
			break;
		}
			

		BufferPointerStart = WindowStart;
		CurrentBufferPointer = BufferPointerStart;
		MaxLength = 0;
	}
	
	BufferPointerStart = WindowStart;
	CurrentBufferPointer = BufferPointerStart;
	MaxLength = 0;
	// Compressing the last part of the message
	while (CharsRemaining)
	{
		// Edit: turn into procedure
		// Find longest string
		// String start is also the end of the buffer
		while (BufferPointerStart != StringStart)
		{
			// Find length of current string
			while (*CurrentBufferPointer == *CurrentStringPointer
				&& CurrentLength < CharsRemaining - 1 &&
				// Edit: change later as the algorithm should be able 
				// to deal with this
				CurrentLength < StringStart - BufferPointerStart)
			{
				CurrentLength++;
				CurrentBufferPointer++;
				CurrentStringPointer++;
			}
			// If string is longer
			if (CurrentLength > MaxLength)
			{
				MaxLength = CurrentLength;
				ReproductionPointer = BufferPointerStart;
				// Can't be a longer string
				if (MaxLength == LENGTH_OF_STRING - 1)
				{
					// Edit: break exists
					CurrentLength = 0;
					CurrentStringPointer = StringStart;
					break;
				}
			}
			// Reset Current String Variables
			CurrentStringPointer = StringStart;
			CurrentLength = 0;
			// Advance buffer pointer start
			BufferPointerStart++;
			CurrentBufferPointer = BufferPointerStart;
		}
		// Edit: change to remove if statement
		// Length of the jump back
		if (ReproductionPointer)
		{
			JumpLength = (int)(StringStart - ReproductionPointer);
			ReproductionPointer = NULL;
		}
		// Reproduction pointer == 0 
		else
		{
			JumpLength = 0;
		}

		// Writing to file

		// Write the length of the jump back
		Dest.write(reinterpret_cast<char*>(&JumpLength), sizeof(short));
		// Write the length of the string
		Dest.write(&MaxLength, sizeof(char));
		// Write next character
		Dest.write(StringStart + MaxLength, sizeof(char));

		// Edit: make procedure && change to be more time efficient
		// Progress window
		for (int i = 0; i < WINDOW_SIZE - MaxLength - 1; i++)
		{
			// Shifting out characters from buffer
			WindowStart[i] = WindowStart[i + MaxLength + 1];
		}

		CharsRemaining -= MaxLength + 1;

		// Debugging
		std::cout << "Chars remaining - " << CharsRemaining << std::endl;
		std::cout << "Jump length - " << JumpLength;
		std::cout << "String length - " << (int)MaxLength << std::endl;

		for (char i = 0; i < MaxLength; i++)
		{
			std::cout << StringStart[i];
		}
		std::cout << "+" << *(StringStart + MaxLength) << std::endl;
		std::cout << "";


		BufferPointerStart = WindowStart;
		CurrentBufferPointer = BufferPointerStart;
		MaxLength = 0;
	}



	delete[] WindowStart;
	Source.close();
	Dest.close();
	std::cout << "Compression Done" << std::endl;
}

/// <summary>
/// Decompresses the file
/// </summary>
/// <param name="SourcePath">The source path.</param>
/// <param name="DestinationPath">The destination path.</param>
void ByteDecompress(std::string SourcePath, std::string DestinationPath)
{
#pragma region Variable definitions
	// The sliding window
	char *WindowStart = new char[BUFFER_SIZE];
	char *WindowEnd = WindowStart + BUFFER_SIZE - 1;
	InitializeBuffer(WindowStart);

	char *StringContents = new char[LENGTH_OF_STRING];

	// Reading variables
	unsigned short int	JumpSize;
	unsigned char		StringLength;
	unsigned char		NextCharacter;

	// Source file
	std::ifstream Source;
	Source.open(SourcePath, std::ios::in | std::ios::binary);
	if (!Source)
	{
		std::cerr << "An error occured in opening file " << SourcePath;
		std::cerr << " in Compress File" << std::endl;
		exit(1);
	}

	// Destination file
	std::ofstream Dest;
	Dest.open(DestinationPath, std::ios::out);
	if (!Dest)
	{
		std::cerr << "An error occured in opening file " << DestinationPath;
		std::cerr << " in Compress File" << std::endl;
		exit(1);
	}
#pragma endregion

	// Code section
	// Reading and writing
	int DebugCounter = 0;
	
	Source.read(reinterpret_cast<char*>(&JumpSize), sizeof(short int));
	Source.read((char*)&StringLength, sizeof(char));
	Source.read((char*)&NextCharacter, sizeof(char));

	while (!Source.eof())
	{
		// Reading from source

		//std::cout << "Jump - " << JumpSize << " - " << "Length - ";
		//std::cout << (int)StringLength <<"String"<<"-";
		//for (size_t i = 1; i < StringLength + 1; i++)
		//{
		//	std::cout << WindowEnd[-JumpSize + i];
		//}
		//std::cout << NextCharacter;
		//std::cout << std::endl;

		//if (DebugCounter++ == 25)
		//{
		//	for (size_t i = BUFFER_SIZE - 100; i < BUFFER_SIZE; i++)
		//	{
		//		std::cout << WindowStart[i];
		//	}
		//	DebugCounter = 0;
		//	std::cout << std::endl;
		//}


		// Writing to the destination file
		Dest.write(WindowEnd - JumpSize + 1, StringLength * sizeof(char));
		Dest.write((char*)&NextCharacter, sizeof(char));

		// Shifting buffer
		int i;
		// Saving the String
		for ( i = 1; i < StringLength + 1; i++)
		{
			StringContents[i-1] = WindowEnd[-JumpSize + i];
		}
		StringContents[i - 1] = NextCharacter;
		// Shifting buffer to the left
		for ( i = 0; i < BUFFER_SIZE - (StringLength + 1); i++)
		{
			WindowStart[i] = WindowStart[i + StringLength + 1];
		}
		// Add string to end of buffer
		for (int j=0; i < BUFFER_SIZE; i++, j++)
		{
			WindowStart[i] = StringContents[j];
		}
		// Reading again
		Source.read(reinterpret_cast<char*>(&JumpSize), sizeof(short int));
		Source.read((char*)&StringLength, sizeof(char));
		Source.read((char*)&NextCharacter, sizeof(char));
	} 

	delete[] WindowStart;
	delete[] StringContents;
	Source.close();
	Dest.close();

	std::cout << "Decompression Done" << std::endl;
}

/// <summary>
/// Initializes the buffer. Edit: change to encryption
/// </summary>
/// <param name="Buffer">The buffer.</param>
void InitializeBuffer(char * Buffer) 
{
	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		Buffer[i] = 0;
	}
}