#include "precompile.h"
#include "kurs.h"

kurs::kurs(const std::string& id, const std::string& typ, const std::string& namn, int po�ng, char betyg) : 
	id_{ id },
	typ_{ typ },
	namn_{ namn },
	po�ng_{ po�ng },
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

void kurs::set_po�ng(int po�ng)
{
	po�ng_ = po�ng;
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

int kurs::po�ng() const
{
	return po�ng_;
}

std::string kurs::get_betyg() const
{
	return std::string(1, betyg());
}

std::string kurs::get_po�ng() const
{
	return std::to_string(po�ng());
}

char kurs::betyg() const
{
	return betyg_;
}

double kurs::get_betygv�rde() const
{
	return betygs_v�rden_.at(betyg_);
}

bool kurs::operator==(const kurs& krs)
{
	return
		id_ == krs.get_id() &&
		typ_ == krs.get_typ() &&
		namn_ == krs.get_namn() &&
		po�ng_ == krs.po�ng() &&
		betyg_ == krs.betyg();
}