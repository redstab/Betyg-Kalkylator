#include "precompile.h"
#include "table_header.h"

table_header::table_header(const window& win, const std::string& tex, int data_length) : text(win, tex), data_length_{ data_length }{}

size table_header::get_element_size() const
{
	return { std::max(data_length_, static_cast<int>(text_.length())), 1 };
}

void table_header::set_data_length(int data_length)
{
	data_length_ = data_length;
}
