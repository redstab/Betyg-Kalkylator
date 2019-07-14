#pragma once
#include <compare>
#include <map>
#include <string>
#include <iostream>
#include <vector>

static std::map<char, double> grade{
	{'A', 20},
	{'B', 17.5},
	{'C', 15},
	{'D', 12.5},
	{'E', 10},
	{'F', 0}
};

enum poäng {
	hel_kurs = 100,
	halv_kurs = 50
};

struct kurs {

	kurs(std::string namn = "", std::string identifikation = "", std::string typ = "", char b = 'F', poäng p = hel_kurs) {
		betyg_bokstav = b;
		betyg_värde = grade.at(betyg_bokstav);
		kurs_längd = p;
		kurs_namn = namn;
		kurs_id = identifikation;
		kurs_typ = typ;
	}

	int poäng() {
		return kurs_längd;
	}


	double summa() {
		return kurs_längd * betyg_värde;
	}

	std::string namn() {
		return kurs_namn;
	}

	char betyg() {
		return betyg_bokstav;
	}

	void betyg(char input) {
		betyg_bokstav = input;
		betyg_värde = grade.at(betyg_bokstav);
	}

	void poäng(int input) {
		kurs_längd = input;
	}

	auto operator<=>(const kurs&) const = default;

	std::string kurs_namn;
	char betyg_bokstav;
	double betyg_värde;
	int kurs_längd;
	std::string kurs_id;
	std::string kurs_typ;


};

static std::ostream& operator<<(std::ostream& os, kurs& m) {
	return os << m.namn() << " {" << std::endl << "    Betyg: "
		<< m.betyg() << std::endl << "    Poäng: " << m.poäng() << std::endl << "    Summa: " << m.summa() << std::endl << "}";
}