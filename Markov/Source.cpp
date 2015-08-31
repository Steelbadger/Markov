#include <fstream>
#include <iostream>
#include "Markov.h"

int main()
{
	std::ifstream file("inputFile.txt");
	std::ofstream fileOut("outputFile.txt");
	std::string test((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();

	Markov markov;
	markov.SetOrder(3);
	markov.ParseText(test);
	markov.SeedGenerator(15);
	std::string final = markov.GetMarkovString(5410);

	std::cout << final;
	fileOut << final;

	fileOut.close();
	return 0;
}