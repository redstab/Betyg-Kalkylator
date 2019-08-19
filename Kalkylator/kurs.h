#pragma once
#include "precompile.h"

static const std::map<char, double> betygs_v�rden_{
	{'A',20},
	{'B',17.5},
	{'C',15},
	{'D',12.5},
	{'E',10},
	{'F',0}
};

class kurs
{
public:
	kurs() {}
	kurs(const std::string& id, const std::string& typ, const std::string& namn, int po�ng, char betyg);
	
	void set_id(const std::string& id);
	void set_typ(const std::string& typ);
	void set_namn(const std::string& namn);
	/*void set_po�ng(int po�ng);*/
	void set_po�ng(const std::string& po�ng);
	/*void set_betyg(char betyg);*/
	void set_betyg(const std::string& betyg);

	std::string get_id() const;
	std::string get_typ() const;
	std::string get_namn() const;
	char betyg() const;
	int po�ng() const;
	std::string get_betyg() const;
	std::string get_po�ng() const;
	double get_betygv�rde() const;

	bool operator==(const kurs& krs);

private:
	std::string id_ = "empty";
	std::string typ_ = "empty";
	std::string namn_ = "empty";
	int po�ng_ = 0;
	char betyg_ = 'F';
};