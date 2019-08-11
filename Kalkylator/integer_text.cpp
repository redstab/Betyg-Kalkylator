#include "precompile.h"
#include "integer_text.h"

integer_text::integer_text(const window& win, const std::string& text, double value, point position) : text(win, text, position), value_{ value } {}

void integer_text::set_value(double value)
{
	value_ = value;
}

double integer_text::get_value() const
{
	return value_;
}

size integer_text::get_element_size() const
{
	return { static_cast<int>(std::string(text_ + " = " + std::to_string(value_)).length()), 1 };
}

void integer_text::draw_element() const
{
	mvwprintw(window_.get_window(), position_.y, position_.x, std::string(text_ + " = " + std::to_string(value_)).c_str());
}
