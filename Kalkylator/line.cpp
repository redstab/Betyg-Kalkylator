#include "precompile.h"
#include "line.h"

line::line(const window& win, point begin, int length, orientation rotation) : ui_element(win, begin), length_{ length }, rotation_{ rotation } {}

void line::set_length(int length)
{
	length_ = length;
}

int line::get_length() const
{
	return length_;
}

size line::get_element_size() const
{
	return ((rotation_ == orientation::vertical) ? size{1, length_} : size{length_, 1});
}

void line::draw_element() const
{
	int (*line_func)(WINDOW*, int, int, chtype, int) = (rotation_ == orientation::vertical) ? mvwvline : mvwhline; ;
	
	line_func(window_.get_window(), position_.y, position_.x, 0, length_);
}
