#include <iostream>
#include <filesystem>
#include <random>

#include "xorstr.h"
#include "service.h"

int main(void);
bool load_driver();

std::filesystem::path driver_path;
const std::filesystem::path o_driver_path = "driver/driver.sys";
std::string driver_name;

BOOL WINAPI control_handler(DWORD fdwCtrlType);

char generate_character()
{
	std::random_device random_device;
	std::mt19937 mersenne_generator(random_device());
	std::uniform_int_distribution<> distribution(97, 122);

	return static_cast<unsigned char>(distribution(mersenne_generator));
}

std::string random_string(size_t length)
{
	std::string str(length, 0);
	std::generate_n(str.begin(), length, generate_character);
	return str;
}