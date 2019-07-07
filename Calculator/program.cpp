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

		std::cout << i << std::endl;

		po�ng_summa += i.po�ng();

		betyg_summa += i.summa();

	}

	snitt_betyg = betyg_summa / po�ng_summa;

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
		summa += krs.po�ng();
	}
	return summa;
}

std::vector<kurs> program::kurserna()
{
	return kurser;
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