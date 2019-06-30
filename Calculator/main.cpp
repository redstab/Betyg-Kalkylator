// Calculator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "program.h"

int main()
{

	kurs matte("Matematik 1c", 'A', hel_kurs);
	
	kurs engelska("Engelska 5", 'B', hel_kurs);

	std::vector<kurs> test = { matte, engelska };

	program a({ matte, engelska }, 0);

	a += matte;
	a += matte;
	a += matte;

	a -= matte;

	std::cout << a.beräkna_snitt() << std::endl;

	std::cin.get();
}