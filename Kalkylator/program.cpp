#include "precompile.h"
#include "program.h"

program::program(const std::vector<kurs>& kurser) : kurser_{ kurser } {}

void program::set_kurser(const std::vector<kurs>& kurser)
{
	kurser_ = kurser;
}

void program::set_merit(double merit)
{
	merit_ = merit;
}

double program::get_merit() const
{
	return merit_;
}

std::vector<kurs>* program::get_kurser()
{
	return &kurser_;
}

double program::get_snitt() const
{
	double summa = std::accumulate(kurser_.begin(), kurser_.end(), 0.0, summa_);
	double poäng = std::accumulate(kurser_.begin(), kurser_.end(), 0.0, poäng_);

	return (summa / poäng) + merit_;
	
}

double program::get_sum() const
{
	return std::accumulate(kurser_.begin(), kurser_.end(), 0.0, poäng_);
}

program& program::operator+=(const kurs& krs)
{
	kurser_.push_back(krs);
	return *this;
}

program& program::operator+(const kurs& krs)
{
	operator+=(krs);
	return *this;
}

program& program::operator-=(const kurs& krs)
{
	auto search = std::find(kurser_.begin(), kurser_.end(), krs);

	if (search != kurser_.end()) {
		kurser_.erase(search);
	}

	return *this;
}

program& program::operator-(const kurs& krs)
{
	operator-=(krs);
	return *this;
}
