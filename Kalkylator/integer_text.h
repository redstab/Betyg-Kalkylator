#pragma once
#include "precompile.h"
#include "text.h"
class integer_text :
	public text
{
public:
	integer_text(const window& win, const std::string& text, double* value, point position);

	void set_value(double value);
	double get_value() const;

	size get_element_size() const;
	void draw_element();

private:
	double* value_;
};

