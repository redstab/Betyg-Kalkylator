#include "program.h"

program::program(std::vector<kurs> krsr, double mrit)
{
	kurser = krsr;
	merit_poäng = mrit;
}

double program::beräkna_snitt()
{

	double poäng_summa = 0;

	double betyg_summa = 0;

	for (auto i : kurser) {

		poäng_summa += i.kurs_längd;

		betyg_summa += i.summa();

	}

	if (betyg_summa != 0 && poäng_summa != 0) {
		snitt_betyg = betyg_summa / poäng_summa;
	}
	else {
		snitt_betyg = 0.f;
	}
	
	return snitt_betyg + merit_poäng;
}

double program::merit()
{
	return merit_poäng;
}

void program::merit(double m)
{
	merit_poäng = m;
}

double program::program_poäng()
{
	double summa = 0;
	for (auto krs : kurser) {
		summa += krs.kurs_längd;
	}
	return summa;
}

std::vector<kurs>* program::kurserna()
{
	return &kurser;
}

program& program::operator+=(kurs krs) {
	kurser.push_back(krs);
	return *this;
}

program& program::operator+(kurs krs) {
	operator+=(krs);
	return *this;
}

program& program::operator-=(kurs krs) {
	auto search = std::find(kurser.begin(), kurser.end(), krs);
	if (search != kurser.end()) {
		kurser.erase(search);
	}
	return *this;
}

program& program::operator-(kurs krs) {
	operator-=(krs);
	return *this;
}