#pragma once
#include "kurs.h"

class program {
public:
	program(std::vector<kurs>, double);

	double ber�kna_snitt();

	double merit();

	void merit(double);

	program& operator+=(kurs);

	program& operator+(kurs);

	program& operator-=(kurs);

	program& operator-(kurs);

private:
	std::vector<kurs> kurser;
	double snitt_betyg;
	double merit_po�ng;

};
