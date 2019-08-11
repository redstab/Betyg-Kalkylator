#include "precompile.h"
#include "text_header.h"

text_header::text_header(const window& win, const std::string& text, point position, int header_length) : text(win, text, position), header_length_{ header_length }{}

size text_header::get_element_size() const
{
	return { std::max(header_length_, static_cast<int>(text_.length())), 1 };
}

void text_header::set_header_length(int header_length)
{
	header_length_ = header_length;
}
