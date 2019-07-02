#pragma once
#include "kurs.h"

class program {

public:
	program(std::vector<kurs>, double);

	double beräkna_snitt();

	double merit();

	void merit(double);

	double program_poäng();

	program& operator+=(kurs);

	program& operator+(kurs);

	program& operator-=(kurs);

	program& operator-(kurs);

private:
	std::vector<kurs> kurser;
	double snitt_betyg;
	double merit_poäng;

};
