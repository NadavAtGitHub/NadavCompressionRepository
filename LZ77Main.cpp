#include "LZ77.h"


int main()
{
	std::string Source = "E:\\Visual Studio projects\\Lempel-Ziv-77\\Alice.txt";
	std::string Storage = "E:\\Visual Studio projects\\Lempel-Ziv-77\\Temp.DAT";
	std::string New = "E:\\Visual Studio projects\\Lempel-Ziv-77\\Decoded.txt";

	std::string Source2 = "E:\\Visual Studio projects\\Lempel-Ziv-77\\LargeText.txt";
	std::string Storage2 = "E:\\Visual Studio projects\\Lempel-Ziv-77\\Temp2.DAT";
	std::string New2 = "E:\\Visual Studio projects\\Lempel-Ziv-77\\Decoded2.txt";
	ByteCompress(Source, Storage);
	ByteDecompress(Storage, New);

	ByteCompress(Source2, Storage2);
	ByteDecompress(Storage2, New2);
	return 0;
}