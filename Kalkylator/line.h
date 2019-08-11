#pragma once
#include "precompile.h"
#include "ui_element.h"

enum class orientation {
	vertical,
	horizontal
};

class line :
	public ui_element
{
public:
	line(const window& win, point begin, int length, orientation rotation);

	void set_length(int length);
	int get_length() const;

	size get_element_size() const;
	void draw_element() const;

private:
	int length_;
	orientation rotation_;
};

