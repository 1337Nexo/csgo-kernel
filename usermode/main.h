#include <iostream>
#include <filesystem>
#include <random>

#include "xorstr.h"
#include "service.h"

int main();

char generateCharacter()
{
	std::random_device randDevice;
	std::mt19937 mersenneGenerator(randDevice());
	std::uniform_int_distribution<> distribution(97, 122);

	return static_cast<unsigned char>(distribution(mersenneGenerator));
}

std::string randString(size_t length)
{
	std::string str(length, 0);
	std::generate_n(str.begin(), length, generateCharacter);
	return str;
}