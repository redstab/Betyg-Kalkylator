#pragma once
#include "precompile.h"
#include "ui_element.h"
#include "text_header.h"
template <const int number_of_headers, const int padding> class header :
	public ui_element
{
public:
	header(const window& win, std::array<text_header, number_of_headers> header_text, point position, int max_size, int seperation);

	std::array<text, number_of_headers> get_header_texts() const;
	void set_header_texts(const std::array<text_header, number_of_headers>& header_texts);

	text_header& access_header_text(int header_index);

	std::array<point, number_of_headers> get_header_positions() const;
	
	int get_max_size() const;

	void set_max_size(int max_size);

	size get_element_size() const;

	void set_seperation(int seperation_point);

	size get_seperation() const;

	void draw_element();
private:
	int max_header_length_;
	int seperation_index_;

	std::array<text, number_of_headers> text_headers_;
	std::array<int, number_of_headers> header_lengths_;


};

