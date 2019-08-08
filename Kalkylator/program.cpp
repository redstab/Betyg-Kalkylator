#include "program.h"

program::program(std::vector<kurs> krsr, double mrit)
{
	kurser = krsr;
	merit_po�ng = mrit;
}

double program::ber�kna_snitt()
{

	double po�ng_summa = 0;

	double betyg_summa = 0;

	for (auto i : kurser) {

		po�ng_summa += i.kurs_l�ngd;

		betyg_summa += i.summa();

	}

	if (betyg_summa != 0 && po�ng_summa != 0) {
		snitt_betyg = betyg_summa / po�ng_summa;
	}
	else {
		snitt_betyg = 0.f;
	}
	
	return snitt_betyg + merit_po�ng;
}

double program::merit()
{
	return merit_po�ng;
}

void program::merit(double m)
{
	merit_po�ng = m;
}

double program::program_po�ng()
{
	double summa = 0;
	for (auto krs : kurser) {
		summa += krs.kurs_l�ngd;
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