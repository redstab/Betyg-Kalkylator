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

	double summa() {
		return kurs_l�ngd * betyg_v�rde;
	}

	void betyg(char input) {
		betyg_bokstav = input;
		betyg_v�rde = grade.at(betyg_bokstav);
	}

	auto operator<=>(const kurs&) const = default;

	std::string kurs_namn;
	char betyg_bokstav;
	double betyg_v�rde;
	int kurs_l�ngd;
	std::string kurs_id;
	std::string kurs_typ;


};