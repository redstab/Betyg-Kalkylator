#pragma once
#include "precompile.h"
#include "kurs.h"
class program
{
public:
	program(const std::vector<kurs>& kurser);

	void set_kurser(const std::vector<kurs>& kurser);
	void set_merit(double merit);

	double* get_merit();
	std::vector<kurs>* get_kurser();

	double* get_snitt();
	double* get_sum();

	void calculate();

	program& operator+=(const kurs& krs);

	program& operator+(const kurs& krs);

	program& operator-=(const kurs& krs);

	program& operator-(const kurs& krs);
private:
	std::vector<kurs> kurser_{};
	double merit_{};
	double snitt_{};
	double sum_{};

	const std::function<double(double, const kurs&)> summa_ = [](double result, const kurs& krs) {return result + (krs.poäng() * krs.get_betygvärde()); };
	const std::function<double(double, const kurs&)> poäng_ = [](double result, const kurs& krs) {return result + krs.poäng(); };

};

