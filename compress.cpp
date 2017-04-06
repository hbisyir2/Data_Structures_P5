//--------------------------------------------------------
// You can only use these headers

#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>

#include "treap_bst.h"

//--------------------------------------------------------

// implement your compress program here

int main(int argc, char *argv[])
{
	
	uint8_t bit8Zero = 0;
	uint32_t bit32Zero = 0;
	std::string wordToStore;
	uint32_t value;
	bool searchResult = false;
	uint32_t keyLocation = 0;
	if (argc != 3 && argc != 4) {
		std::cout << "Usage: compress"
			<< "<input_file> <output_file> for compress\n"
			<< "-d <input_file> <output_file> for decompress"
			<< std::endl;
		return EXIT_FAILURE;
	}
	else if (argc == 3) {
		TreapBST<std::string, uint32_t> words;
		std::ifstream inputFile(argv[1]);
		if (!inputFile.good())
		{
			std::cerr << "input file is invalid";
			return EXIT_FAILURE;
		}
		std::ofstream outputFile(argv[2], std::ios::binary);

		while (std::getline(inputFile, wordToStore, ' ')) {

			value = words.search(wordToStore, searchResult);
			if (!searchResult) {
				keyLocation++;
				words.insert(wordToStore, keyLocation);
				outputFile.write(reinterpret_cast<const char*>(&keyLocation), sizeof(keyLocation));
				outputFile.write(wordToStore.c_str(), wordToStore.length());
				outputFile.write(reinterpret_cast<const char*>(&bit8Zero), sizeof(bit8Zero));
			}
		}
		outputFile.write(reinterpret_cast<const char*>(&bit32Zero), sizeof(bit32Zero));

		// move back to the beginning of text file
		inputFile.close();
		inputFile.open(argv[1]);

		keyLocation = 1;

		while (std::getline(inputFile, wordToStore, ' ')) {
			value = words.search(wordToStore, searchResult);
			outputFile.write(reinterpret_cast<const char*>(&value), sizeof(value));
		}
	}
	else {
		TreapBST<uint32_t, std::string> words;
		std::ifstream inputFile(argv[2], std::ios::binary | std::ios::in);
		std::string checkForD = argv[1];

		if (checkForD != "-d") {
			std::cout << "Usage: compress"
				<< "<input_file> <output_file> for compress\n"
				<< "-d <input_file> <output_file> for decompress"
				<< std::endl;
			return EXIT_FAILURE;
		}

		if (!inputFile.good())
		{
			std::cerr << "input file is invalid";
			return EXIT_FAILURE;
		}

		std::ofstream outputFile(argv[3]);
		uint8_t read8 = 2;
		uint32_t read32 = 3;
		std::string fullWord;

		inputFile.read(reinterpret_cast<char*>(&read32), sizeof(read32));
		while (read32 != 0) {
			read8 = 1;
			fullWord = "";
			while (read8 != 0) {
				inputFile.read(reinterpret_cast<char*>(&read8), sizeof(read8));
				if (read8 != '\0') fullWord += read8;
			}
			words.insert(read32, fullWord);
			inputFile.read(reinterpret_cast<char*>(&read32), sizeof(read32));
		}
		inputFile.read(reinterpret_cast<char*>(&read32), sizeof(read32));

		fullWord = "";
		while (fullWord[0] != '\n') {
			fullWord = words.search(read32, searchResult);
			inputFile.read(reinterpret_cast<char*>(&read32), sizeof(read32));
			outputFile << fullWord;
			if (fullWord[0] != '\n') outputFile << " ";
		}
	}
	return EXIT_SUCCESS;
}
