#include "precompile.h"
#include "kurs.h"

kurs::kurs(const std::string& id, const std::string& typ, const std::string& namn, int poäng, char betyg) : 
	id_{ id },
	typ_{ typ },
	namn_{ namn },
	poäng_{ poäng },
	betyg_{ betyg }
{}

void kurs::set_id(const std::string& id)
{
	id_ = id;
}

void kurs::set_typ(const std::string& typ)
{
	typ_ = typ;
}

void kurs::set_namn(const std::string& namn)
{
	namn_ = namn;
}

void kurs::set_poäng(int poäng)
{
	poäng_ = poäng;
}

void kurs::set_betyg(char betyg)
{
	betyg_ = betyg;
}

std::string kurs::get_id() const
{
	return id_;
}

std::string kurs::get_typ() const
{
	return typ_;
}

std::string kurs::get_namn() const
{
	return namn_;
}

int kurs::poäng() const
{
	return poäng_;
}

std::string kurs::get_betyg() const
{
	return std::string(1, betyg());
}

std::string kurs::get_poäng() const
{
	return std::to_string(poäng());
}

char kurs::betyg() const
{
	return betyg_;
}

double kurs::get_betygvärde() const
{
	return betygs_värden_.at(betyg_);
}

bool kurs::operator==(const kurs& krs)
{
	return
		id_ == krs.get_id() &&
		typ_ == krs.get_typ() &&
		namn_ == krs.get_namn() &&
		poäng_ == krs.poäng() &&
		betyg_ == krs.betyg();
}