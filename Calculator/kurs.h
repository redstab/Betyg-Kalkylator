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

enum po�ng {
	hel_kurs = 100,
	halv_kurs = 50
};

struct kurs {

	kurs(std::string namn = "", std::string identifikation = "", std::string typ = "", char b = 'F', po�ng p = hel_kurs) {
		betyg_bokstav = b;
		betyg_v�rde = grade.at(betyg_bokstav);
		kurs_l�ngd = p;
		kurs_namn = namn;
		kurs_id = identifikation;
		kurs_typ = typ;
	}

	int po�ng() {
		return kurs_l�ngd;
	}


	double summa() {
		return kurs_l�ngd * betyg_v�rde;
	}

	std::string namn() {
		return kurs_namn;
	}

	char betyg() {
		return betyg_bokstav;
	}

	void betyg(char input) {
		betyg_bokstav = input;
		betyg_v�rde = grade.at(betyg_bokstav);
	}

	void po�ng(int input) {
		kurs_l�ngd = input;
	}

	auto operator<=>(const kurs&) const = default;

	std::string kurs_namn;
	char betyg_bokstav;
	double betyg_v�rde;
	int kurs_l�ngd;
	std::string kurs_id;
	std::string kurs_typ;


};

static std::ostream& operator<<(std::ostream& os, kurs& m) {
	return os << m.namn() << " {" << std::endl << "    Betyg: "
		<< m.betyg() << std::endl << "    Po�ng: " << m.po�ng() << std::endl << "    Summa: " << m.summa() << std::endl << "}";
}